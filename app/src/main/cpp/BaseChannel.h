//
// Created by Administrator on 2018/9/8.
//

#ifndef FFMPEGPLAYER_BASECHANNEL_H
#define FFMPEGPLAYER_BASECHANNEL_H


#include "SafeQueue.h"

extern "C" {
#include <libavcodec/avcodec.h>
};


class BaseChannel {
public:
    BaseChannel(int id, AVCodecContext *avCodecContext) : id(id), avCodecContext(avCodecContext) {

    }

    virtual ~BaseChannel() {
        packets.setReleaseCallback(BaseChannel::releaseAVPacket);
        packets.clear();
    }

    static void releaseAVPacket(AVPacket **packet) {
        if (packet) {
            av_packet_free(packet);
            *packet = 0;
        }
    }

    static void releaseAVFrame(AVFrame **frame) {
        if (frame) {
            av_frame_free(frame);
            *frame = 0;
        }
    }

    virtual void play() = 0;

    int id;
    AVCodecContext *avCodecContext;
    bool isPlaying;
    SafeQueue<AVPacket *> packets;
};

#endif //FFMPEGPLAYER_BASECHANNEL_H
