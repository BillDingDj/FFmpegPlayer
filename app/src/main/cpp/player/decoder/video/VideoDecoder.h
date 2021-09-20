//
// Created by admin on 2021/9/18.
//

#ifndef FFMPEGPLAYER_VIDEODECODER_H
#define FFMPEGPLAYER_VIDEODECODER_H
extern "C" {
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavcodec/jni.h>
};

#include <render/video/VideoRender.h>
#include "decoder/DecoderBase.h"
#define IMAGE_FORMAT_RGBA           0x01
#define IMAGE_FORMAT_NV21           0x02
#define IMAGE_FORMAT_NV12           0x03
#define IMAGE_FORMAT_I420           0x04

#define IMAGE_FORMAT_RGBA_EXT       "RGB32"
#define IMAGE_FORMAT_NV21_EXT       "NV21"
#define IMAGE_FORMAT_NV12_EXT       "NV12"
#define IMAGE_FORMAT_I420_EXT       "I420"

class VideoDecoder : public DecoderBase{
public:

    virtual ~VideoDecoder() {
        UnInit();
    }

    int GetVideoWidth() {
        return m_VideoWidth;
    }

    int GetVideoHeight() {
        return m_VideoHeight;
    }

    void SetVideoRender(VideoRender *render) {
        m_VideoRender = render;
    }

    VideoDecoder(char *url) {
        Init(url, AVMEDIA_TYPE_VIDEO);
    }

private:
    virtual void OnDecoderReady();
    virtual void OnDecoderDone();
    virtual void OnFrameAvailable(AVFrame *frame);

    const AVPixelFormat DST_PIXEL_FORMAT = AV_PIX_FMT_RGBA;

    int m_VideoWidth = 0;
    int m_VideoHeight = 0;

    int m_RenderWidth = 0;
    int m_RenderHeight = 0;

    AVFrame  *m_RGBFrame = nullptr;
    uint8_t *m_FrameBuffer = nullptr;

    SwsContext *m_SwsContext = nullptr;
    VideoRender *m_VideoRender = nullptr;

};

#endif //FFMPEGPLAYER_VIDEODECODER_H
