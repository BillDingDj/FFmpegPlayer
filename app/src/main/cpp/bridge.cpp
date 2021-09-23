#include <jni.h>
#include <cstdio>
#include <cstring>
#include <android/native_window.h>
#include "FFmpegPlayer.h"
//
// Created by BillDing on 2021/9/14.
//
#ifdef __cplusplus
extern "C" {
#endif

long nativeInit(JNIEnv *env, jobject thiz, jstring url,
                jint render_type, jobject surface) {
    const char *m_Url = env->GetStringUTFChars(url, nullptr);
    FFmpegPlayer *player = new FFmpegPlayer();
    player->Init(env, thiz, const_cast<char *>(m_Url), render_type, surface);
    return reinterpret_cast<jlong>(player);
}

void nativePlay(JNIEnv *env, jobject thiz, jlong handler) {
    auto *player = reinterpret_cast<FFmpegPlayer *>(handler);
    if (player == nullptr) {
        return;
    }
    player->Play();
}

void nativeSeekToPosition(JNIEnv *env, jobject thiz, jlong handler, jfloat position) {
    auto *player = reinterpret_cast<FFmpegPlayer *>(handler);
    if (player == nullptr) {
        return;
    }
    player->SeekToPosition(position);
}

void nativePause(JNIEnv *env, jobject thiz, jlong handler) {
    auto *player = reinterpret_cast<FFmpegPlayer *>(handler);
    if (player == nullptr) {
        return;
    }
    player->Pause();
}

void nativeStop(JNIEnv *env, jobject thiz, jlong handler) {
    auto *player = reinterpret_cast<FFmpegPlayer *>(handler);
    if (player == nullptr) {
        return;
    }
    player->Stop();
}

void nativeUnInit(JNIEnv *env, jobject thiz, jlong handler) {
    auto *player = reinterpret_cast<FFmpegPlayer *>(handler);
    if (player == nullptr) {
        return;
    }
    player->UnInit();
}
void GLInit(JNIEnv *env, jobject thiz) {

}

void GLUnInit(JNIEnv *env, jobject thiz) {

}

void GLSetImageData(JNIEnv *env, jobject thiz, jint format, jint width, jint height,
                    jbyteArray bytes) {

}

void GLOnSurfaceCreate(JNIEnv *env, jobject thiz) {

}

void GLOnSurfaceChanged(JNIEnv *env, jobject thiz, jint width, jint height) {

}

void GLOnDrawFrames(JNIEnv *env, jobject thiz) {

}

static JNINativeMethod methods[] = {
        {"native_Init",                "(Ljava/lang/String;ILjava/lang/Object;)J", (void *) nativeInit},
        {"native_Play",                "(J)V",                                     (void *) nativePlay},
        {"native_SeekToPosition",      "(JF)V",                                    (void *) nativeSeekToPosition},
        {"native_Pause",               "(J)V",                                     (void *) nativePause},
        {"native_Stop",                "(J)V",                                     (void *) nativeStop},
        {"native_UnInit",              "(J)V",                                     (void *) nativeUnInit},
        {"native_GL_Init",             "()V",                                      (void *) GLInit},
        {"native_GL_OnInit",           "()V",                                      (void *) GLUnInit},
        {"native_GL_SetImageData",     "(III[B)V",                                 (void *) GLSetImageData},
        {"native_GL_OnSurfaceCreate",  "()V",                                      (void *) GLOnSurfaceCreate},
        {"native_GL_OnSurfaceChanged", "(II)V",                                    (void *) GLOnSurfaceChanged},
        {"native_GL_OnDrawFrames",     "()V",                                      (void *) GLOnDrawFrames},
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {

    JNIEnv *env = nullptr;
    if (jvm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_EVERSION;
    }

    const char *java_class = "com/building/ffmpegplayer/NativeBrigde";
    jclass clazz = env->FindClass(java_class);

    if (clazz == nullptr) {
        return JNI_ERR;
    }

    if (env->RegisterNatives(clazz, methods, (int) sizeof(methods) / sizeof(methods[0])) < 0) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_2;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {

}

#ifdef __cplusplus
}
#endif