//
// Created by Administrator on 2018/9/7.
//

#ifndef FFMPEGPLAYER_VIDEOCHANNEL_H
#define FFMPEGPLAYER_VIDEOCHANNEL_H


#include "BaseChannel.h"

extern "C" {
#include <libswscale/swscale.h>
};

typedef void (*RenderFrameCallback)(uint8_t *,int,int,int);

class VideoChannel : public BaseChannel {

public:
    VideoChannel(int id, AVCodecContext *avCodecContext);

    ~VideoChannel();

    void play();

    void decode();

    void render();

    void setRenderFrameCallback(RenderFrameCallback callback) {
        this->callback = callback;
    }

private:
    pthread_t pid_decode;
    pthread_t pid_render;
    SafeQueue<AVFrame*> frames;
    SwsContext *swsContext = 0;
    RenderFrameCallback callback;
};


#endif //FFMPEGPLAYER_VIDEOCHANNEL_H
