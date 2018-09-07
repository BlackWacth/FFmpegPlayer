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

private:
    char *dataSource;
    pthread_t pid;
    AVFormatContext *formatContext;
    JavaCallHelper *callHelper;
    AudioChannel *audioChannel;
    VideoChannel *videoChannel;
};


#endif //FFMPEGPLAYER_DNFFMPEG_H
