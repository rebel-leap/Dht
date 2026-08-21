#include <vector>
#include "dht/exceptions.hpp"
#include "core/color/color_types.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

namespace Dht {

std::vector<std::vector<RGB>> extractFrames(const std::string& path, int& width, int& height, int targetFps) {
    AVFormatContext* formatCtx = nullptr;
    if (avformat_open_input(&formatCtx, path.c_str(), nullptr, nullptr) != 0) {
        throw DhtError("Cannot open video file: " + path);
    }

    if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
        avformat_close_input(&formatCtx);
        throw DhtError("Cannot find stream info");
    }

    int videoStreamIdx = -1;
    for (unsigned int i = 0; i < formatCtx->nb_streams; i++) {
        if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIdx = i;
            break;
        }
    }

    if (videoStreamIdx == -1) {
        avformat_close_input(&formatCtx);
        throw DhtError("No video stream found");
    }

    AVCodecParameters* codecParams = formatCtx->streams[videoStreamIdx]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
    if (!codec) {
        avformat_close_input(&formatCtx);
        throw DhtError("Video codec not found");
    }

    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
    if (!codecCtx) {
        avformat_close_input(&formatCtx);
        throw DhtError("Failed to allocate video codec context");
    }

    if (avcodec_parameters_to_context(codecCtx, codecParams) < 0) {
        avcodec_free_context(&codecCtx);
        avformat_close_input(&formatCtx);
        throw DhtError("Failed to copy video codec params");
    }

    if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
        avcodec_free_context(&codecCtx);
        avformat_close_input(&formatCtx);
        throw DhtError("Failed to open video codec");
    }

    width = codecCtx->width;
    height = codecCtx->height;

    SwsContext* swsCtx = sws_getContext(width, height, codecCtx->pix_fmt, width, height, AV_PIX_FMT_RGB24, SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!swsCtx) {
        avcodec_free_context(&codecCtx);
        avformat_close_input(&formatCtx);
        throw DhtError("Failed to create sws context");
    }

    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();
    AVFrame* rgbFrame = av_frame_alloc();
    if (!packet || !frame || !rgbFrame) {
        sws_freeContext(swsCtx);
        av_packet_free(&packet);
        av_frame_free(&frame);
        av_frame_free(&rgbFrame);
        avcodec_free_context(&codecCtx);
        avformat_close_input(&formatCtx);
        throw DhtError("Failed to allocate frames/packet");
    }

    int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24, width, height, 1);
    std::vector<uint8_t> rgbBuffer(bufferSize);
    av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, rgbBuffer.data(), AV_PIX_FMT_RGB24, width, height, 1);

    std::vector<std::vector<RGB>> frames;
    double fps = av_q2d(formatCtx->streams[videoStreamIdx]->r_frame_rate);
    int frameInterval = (int)(fps / targetFps);
    if (frameInterval < 1) frameInterval = 1;
    int frameCount = 0;

    while (av_read_frame(formatCtx, packet) >= 0) {
        if (packet->stream_index == videoStreamIdx) {
            if (avcodec_send_packet(codecCtx, packet) == 0) {
                while (avcodec_receive_frame(codecCtx, frame) == 0) {
                    if (frameCount % frameInterval == 0) {
                        sws_scale(swsCtx, frame->data, frame->linesize, 0, height, rgbFrame->data, rgbFrame->linesize);

                        std::vector<RGB> pixels(width * height);
                        for (int y = 0; y < height; y++) {
                            for (int x = 0; x < width; x++) {
                                size_t idx = y * width * 3 + x * 3;
                                pixels[y * width + x] = RGB(rgbBuffer[idx], rgbBuffer[idx + 1], rgbBuffer[idx + 2]);
                            }
                        }
                        frames.push_back(pixels);
                    }
                    frameCount++;
                }
            }
        }
        av_packet_unref(packet);
    }

    sws_freeContext(swsCtx);
    av_packet_free(&packet);
    av_frame_free(&frame);
    av_frame_free(&rgbFrame);
    avcodec_free_context(&codecCtx);
    avformat_close_input(&formatCtx);

    return frames;
}

}
