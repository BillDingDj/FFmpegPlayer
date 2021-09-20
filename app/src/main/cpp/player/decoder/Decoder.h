//
// Created by admin on 2021/9/17.
//

#ifndef FFMPEGPLAYER_DECODER_H
#define FFMPEGPLAYER_DECODER_H
#include "libswscale/swscale.h"
typedef void (*MessageCallback)(void*, int, float);
class Decoder {
public:
    virtual void Start() = 0;
    virtual void Pause() = 0;
    virtual void Stop() = 0;
    virtual float GetDuration() = 0;
    virtual void SeekToPosition(float position) = 0;
    virtual float GetCurrentPosition() = 0;
    virtual void SetMessageCallback(void* context, MessageCallback callback) = 0;
};

#endif //FFMPEGPLAYER_DECODER_H
