#include <vector>
#include "dht/exceptions.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
}

namespace Dht {

std::vector<double> extractAudio(const std::string& path) {
    AVFormatContext* formatCtx = nullptr;
    if (avformat_open_input(&formatCtx, path.c_str(), nullptr, nullptr) != 0) {
        throw DhtError("Cannot open file: " + path);
    }

    if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
        avformat_close_input(&formatCtx);
        throw DhtError("Cannot find stream info");
    }

    int audioStreamIdx = -1;
    for (unsigned int i = 0; i < formatCtx->nb_streams; i++) {
        if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamIdx = i;
            break;
        }
    }

    if (audioStreamIdx == -1) {
        avformat_close_input(&formatCtx);
        return std::vector<double>();
    }

    AVCodecParameters* codecParams = formatCtx->streams[audioStreamIdx]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
    if (!codec) {
        avformat_close_input(&formatCtx);
        throw DhtError("Audio codec not found");
    }

    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
    if (!codecCtx) {
        avformat_close_input(&formatCtx);
        throw DhtError("Failed to allocate audio codec context");
    }

    if (avcodec_parameters_to_context(codecCtx, codecParams) < 0) {
        avcodec_free_context(&codecCtx);
        avformat_close_input(&formatCtx);
        throw DhtError("Failed to copy audio codec params");
    }

    if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
        avcodec_free_context(&codecCtx);
        avformat_close_input(&formatCtx);
        throw DhtError("Failed to open audio codec");
    }

    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();
    if (!packet || !frame) {
        av_packet_free(&packet);
        av_frame_free(&frame);
        avcodec_free_context(&codecCtx);
        avformat_close_input(&formatCtx);
        throw DhtError("Failed to allocate packet/frame");
    }

    std::vector<double> samples;

    while (av_read_frame(formatCtx, packet) >= 0) {
        if (packet->stream_index == audioStreamIdx) {
            if (avcodec_send_packet(codecCtx, packet) == 0) {
                while (avcodec_receive_frame(codecCtx, frame) == 0) {
                    int channels = frame->channels;
                    int nb_samples = frame->nb_samples;
                    double* data = (double*)frame->data[0];

                    for (int i = 0; i < nb_samples; i++) {
                        double sample = 0.0;
                        for (int c = 0; c < channels; c++) {
                            sample += data[i * channels + c];
                        }
                        sample /= channels;
                        samples.push_back(sample);
                    }
                }
            }
        }
        av_packet_unref(packet);
    }

    av_packet_free(&packet);
    av_frame_free(&frame);
    avcodec_free_context(&codecCtx);
    avformat_close_input(&formatCtx);

    return samples;
}

}
