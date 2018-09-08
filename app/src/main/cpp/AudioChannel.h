//
// Created by Administrator on 2018/9/7.
//

#ifndef FFMPEGPLAYER_AUDIOCHANNEL_H
#define FFMPEGPLAYER_AUDIOCHANNEL_H


#include "BaseChannel.h"

class AudioChannel : public BaseChannel{
public:
    AudioChannel(int id, AVCodecContext *avCodecContext);

    ~AudioChannel();

    void play();

};


#endif //FFMPEGPLAYER_AUDIOCHANNEL_H
