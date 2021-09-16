#include <jni.h>
#include <cstdio>
#include <cstring>
#include <android/native_window.h>
#include "FFmpegPlayer.h"
//
// Created by pcboy on 2021/9/14.
//
#ifdef __cplusplus
extern "C" {
#endif

long nativeInit(JNIEnv *env, jobject thiz, jstring url,
                jint render_type, jobject surface) {
    const char* m_Url = env->GetStringUTFChars(url, nullptr);
    FFmpegPlayer *player = new FFmpegPlayer();
    long handler = player->Init(const_cast<char *>(m_Url), surface);
    if (handler != 0) {
        return handler;
    } else {
        return 0;
    }
}

void nativePlay(JNIEnv *env, jobject thiz, jlong handler) {
    auto *player = reinterpret_cast<FFmpegPlayer*>(handler);
    if (player == nullptr) {
        return;
    }
    player->play();
}

void nativeSeekToPosition(JNIEnv *env, jobject thiz, jlong handler, jfloat positon) {
    auto *player = reinterpret_cast<FFmpegPlayer*>(handler);
    if (player == nullptr) {
        return;
    }
    player->seekToPosition(p);
}

void nativePause(JNIEnv *env, jobject thiz, jlong handler) {
    auto *player = reinterpret_cast<FFmpegPlayer*>(handler);
    if (player == nullptr) {
        return;
    }
    player->pause();
}

void nativeStop(JNIEnv *env, jobject thiz, jlong handler) {
    auto *player = reinterpret_cast<FFmpegPlayer*>(handler);
    if (player == nullptr) {
        return;
    }
    player->stop();
}

void nativeUnInit(JNIEnv *env, jobject thiz, jlong handler) {
    auto *player = reinterpret_cast<FFmpegPlayer*>(handler);
    if (player == nullptr) {
        return;
    }
    player->destroy();
}

static JNINativeMethod methods[] = {
        {"native_Init", "(Ljava/lang/String;ILjava/lang/Object;)J", (void *) nativeInit},
        {"native_Play", "(J)V", (void *) nativePlay},
        {"native_SeekToPosition", "(JF)V", (void *) nativeSeekToPosition},
        {"native_Pause", "(J)V", (void *) nativePause},
        {"native_Stop", "(J)V", (void *) nativeStop},
        {"native_UnInit", "(J)V", (void *) nativeUnInit},
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {

    JNIEnv *env = nullptr;
    if (jvm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_EVERSION;
    }

    const char *java_class = "com/building/ffmpegplayer/FFmpegPlayer";
    jclass clazz = env->FindClass(java_class);

    if (clazz == nullptr) {
        return JNI_ERR;
    }

    if (env->RegisterNatives(clazz, methods, (int) sizeof(methods) / sizeof(methods[0])) < 0) {
        return JNI_ERR;
    }

    return JNI_OK;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {

}

#ifdef __cplusplus
}
#endif