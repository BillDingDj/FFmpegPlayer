//
// Created by pcboy on 2021/9/16.
//

#ifndef FFMPEGPLAYER_FFMPEGPLAYER_H
#define FFMPEGPLAYER_FFMPEGPLAYER_H

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include <decoder/video/VideoDecoder.h>
#include "LogUtil.h"
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
    FFmpegPlayer(){};
    ~FFmpegPlayer(){};

    void Init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface);
    void UnInit();

    void Play();
    void Pause();
    void Stop();
    void SeekToPosition(float position);
    long GetMediaParams(int paramType);

private:
    JNIEnv *GetJNIEnv(bool *isAttach);
    jobject GetJavaObj();
    JavaVM *GetJavaVM();

    static void PostMessage(void *context, int msgType, float msgCode);

    JavaVM *m_JavaVM = nullptr;
    jobject m_JavaObj = nullptr;

    VideoDecoder *m_VideoDecoder = nullptr;
//    AudioDecoder *m_AudioDecoder = nullptr;

    VideoRender *m_VideoRender = nullptr;
//    AudioRender *m_AudioRender = nullptr;
};


#endif //FFMPEGPLAYER_FFMPEGPLAYER_H
