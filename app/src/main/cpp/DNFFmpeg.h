//
// Created by Administrator on 2018/9/7.
//

#ifndef FFMPEGPLAYER_DNFFMPEG_H
#define FFMPEGPLAYER_DNFFMPEG_H

#include "JavaCallHelper.h"
#include "AudioChannel.h"
#include "VideoChannel.h"

extern "C" {
#include <libavformat/avformat.h>
};

class DNFFmpeg {

public:
    DNFFmpeg(JavaCallHelper *callHelper, const char *dataSource);

    ~DNFFmpeg();

    void prepare();

    void _prepare();

    void start();

    void _start();

    void setRenderFrameCallback(RenderFrameCallback callback) {
        this->callback = callback;
    }

private:
    char *dataSource;
    pthread_t pid;
    pthread_t pid_play;
    bool isPlaying;

    JavaCallHelper *callHelper;
    AVFormatContext *formatContext = 0;
    AudioChannel *audioChannel = 0;
    VideoChannel *videoChannel = 0;
    RenderFrameCallback callback;
};


#endif //FFMPEGPLAYER_DNFFMPEG_H
