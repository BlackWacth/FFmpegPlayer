//
// Created by Administrator on 2018/9/7.
//

#include "AudioChannel.h"

AudioChannel::AudioChannel(int id, AVCodecContext *avCodecContext) : BaseChannel(id,
                                                                                 avCodecContext) {}

AudioChannel::~AudioChannel() {

}

void AudioChannel::play() {

}
