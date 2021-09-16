//
// Created by pcboy on 2021/9/16.
//

#include "FFmpegPlayer.h"


long FFmpegPlayer::Init(JNIEnv *env, char *url, jobject surface) {
    m_Url = url;
    m_Window = ANativeWindow_fromSurface(env, surface);

    m_AvFormatContext = avformat_alloc_context();

    if (avformat_open_input(&m_AvFormatContext, m_Url, nullptr, nullptr) < 0) {
        return 0;
    }

    if (avformat_find_stream_info(m_AvFormatContext, nullptr) < 0) {
        return 0;
    }

    for (int i = 0; i < m_AvFormatContext->nb_streams; ++i) {
        if (m_AvFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_VideoIndex = i;
            break;
        }
    }

    if (m_VideoIndex == -1) {
        return 0;
    }
    AVCodecParameters *codecParameters = m_AvFormatContext->streams[m_VideoIndex]->codecpar;
    m_AvCodec = avcodec_find_decoder(codecParameters->codec_id);
    if (m_AvCodec == nullptr) {
        return 0;
    }

    m_AvCodecContext = avcodec_alloc_context3(m_AvCodec);
    if (avcodec_parameters_to_context(m_AvCodecContext, codecParameters) != 0) {
        return 0;
    }

    auto result = avcodec_open2(m_AvCodecContext, m_AvCodec, nullptr);
    if (result < 0) {
        return 0;
    }
    m_Duration = m_AvFormatContext->duration;
    m_AvPacket = av_packet_alloc();
    m_AvFrame = av_frame_alloc();

    return reinterpret_cast<long>(this);
}

void FFmpegPlayer::play() {

}

void FFmpegPlayer::stop() {

}

void FFmpegPlayer::pause() {

}

void FFmpegPlayer::seekToPosition(float p) {

}

void FFmpegPlayer::destroy() {

}
