//
// Created by pcboy on 2021/9/16.
//

#include <NativeRender.h>
#include "FFmpegPlayer.h"

void FFmpegPlayer::Init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface) {

    jniEnv->GetJavaVM(&m_JavaVM);
    m_JavaObj = jniEnv->NewGlobalRef(obj);

    m_VideoDecoder = new VideoDecoder(url);

    m_VideoRender = new NativeRender(jniEnv, surface);
    m_VideoDecoder->SetVideoRender(m_VideoRender);
}

void FFmpegPlayer::UnInit() {
    if(m_VideoDecoder) {
        delete m_VideoDecoder;
        m_VideoDecoder = nullptr;
    }

    if(m_VideoRender) {
        delete m_VideoRender;
        m_VideoRender = nullptr;
    }

    bool isAttach = false;
    GetJNIEnv(&isAttach)->DeleteGlobalRef(m_JavaObj);
    if(isAttach)
        GetJavaVM()->DetachCurrentThread();
}

void FFmpegPlayer::Play() {
    if(m_VideoDecoder)
        m_VideoDecoder->Start();
}

void FFmpegPlayer::Pause() {
    if(m_VideoDecoder)
        m_VideoDecoder->Pause();
}

void FFmpegPlayer::Stop() {
    if(m_VideoDecoder)
        m_VideoDecoder->Stop();
}

void FFmpegPlayer::SeekToPosition(float position) {
    if(m_VideoDecoder)
        m_VideoDecoder->SeekToPosition(position);

}

long FFmpegPlayer::GetMediaParams(int paramType) {
    return 0;
}

JNIEnv *FFmpegPlayer::GetJNIEnv(bool *isAttach) {
    JNIEnv *env;
    int status;
    if (nullptr == m_JavaVM) {
        return nullptr;
    }
    *isAttach = false;
    status = m_JavaVM->GetEnv((void **)&env, JNI_VERSION_1_4);
    if (status != JNI_OK) {
        status = m_JavaVM->AttachCurrentThread(&env, nullptr);
        if (status != JNI_OK) {
            return nullptr;
        }
        *isAttach = true;
    }
    return env;}

jobject FFmpegPlayer::GetJavaObj() {
    return m_JavaObj;
}

JavaVM *FFmpegPlayer::GetJavaVM() {
    return m_JavaVM;
}

void FFmpegPlayer::PostMessage(void *context, int msgType, float msgCode) {

}
