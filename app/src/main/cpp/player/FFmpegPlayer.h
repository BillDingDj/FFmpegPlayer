//
// Created by pcboy on 2021/9/16.
//

#ifndef FFMPEGPLAYER_FFMPEGPLAYER_H
#define FFMPEGPLAYER_FFMPEGPLAYER_H

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/log.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/time.h>
#include <libavcodec/jni.h>
};
#define LOG_TAG = "FFmpegPlayer"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,__VA_ARGS__)

class FFmpegPlayer {

public:
    long Init(JNIEnv *env, char* url, jobject surface);

    void play();

    void stop();

    void pause();

    void seekToPosition(float p);

    void destroy();
private:
    ANativeWindow *m_Window = nullptr;
    char* m_Url = nullptr;

    AVFormatContext *m_AvFormatContext = nullptr;
    int m_VideoIndex = -1;
    AVCodec *m_AvCodec = nullptr;
    AVCodecContext *m_AvCodecContext = nullptr;

    AVFrame *m_AvFrame = nullptr;
    AVPacket *m_AvPacket = nullptr;

    int64_t m_Duration = 0;
};


#endif //FFMPEGPLAYER_FFMPEGPLAYER_H
