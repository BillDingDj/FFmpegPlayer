//
// Created by admin on 2021/9/19.
//

#ifndef FFMPEGPLAYER_NATIVERENDER_H
#define FFMPEGPLAYER_NATIVERENDER_H

#include <jni.h>
#include <FFmpegPlayer.h>
#include "VideoRender.h"

class NativeRender : public VideoRender {
public:
    NativeRender(JNIEnv *env, jobject surface);

    virtual ~NativeRender();

    virtual void Init(int videoWidth, int videoHeight, int *dstSize);

    virtual void RenderVideoFrame(NativeImage *pImage);

    virtual void UnInit();

private:
    ANativeWindow_Buffer m_NativeWindowBuffer;
    ANativeWindow *m_NativeWindow = nullptr;
    int m_DstWidth;
    int m_DstHeight;
};


#endif //FFMPEGPLAYER_NATIVERENDER_H
