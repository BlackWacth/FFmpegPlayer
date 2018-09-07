//
// Created by Administrator on 2018/9/7.
//

#include <pthread.h>
#include "DNFFmpeg.h"
#include "macro.h"


void *task_prepare(void *args) {
    DNFFmpeg *ffmpeg = static_cast<DNFFmpeg *>(args);
    ffmpeg->_prepare();
    return 0;
}

DNFFmpeg::DNFFmpeg(JavaCallHelper *callHelper, const char *dataSource) {
    LOG_I("DNFFmpeg::构造函数");
    this->callHelper = callHelper;
    this->dataSource = new char[strlen(dataSource)];
    strcpy(this->dataSource, dataSource);
}

DNFFmpeg::~DNFFmpeg() {
    LOG_I("DNFFmpeg::析构函数");
    DELETE(dataSource);
    DELETE(callHelper);
}

void DNFFmpeg::prepare() {
    LOG_I("DNFFmpeg::prepare");
    pthread_create(&pid, 0, task_prepare, this);
}

void DNFFmpeg::_prepare() {
    LOG_I("DNFFmpeg::_prepare::子线程");
    // 1、初始化网络
    avformat_network_init();

    // 2、打开媒体地址
    formatContext = NULL;
    int ret = avformat_open_input(&formatContext, dataSource, 0, 0);
    if (ret != 0) {
        LOG_E("打开媒体失败: %s", av_err2str(ret));
        callHelper->onError(THREAD_CHILD, FFMPEG_CAN_NOT_OPEN_URL);
        return;
    }

    // 3、查找媒体中的音视频流
    ret = avformat_find_stream_info(formatContext, 0);
    if (ret < 0) {
        LOG_E("查找音视频流失败: %s", av_err2str(ret));
        callHelper->onError(THREAD_CHILD, FFMPEG_CAN_NOT_FIND_STREAMS);
        return;
    }
    //nb_streams: 流的个数
    for (int i = 0; i < formatContext->nb_streams; ++i) {
        // 单个流，可能是音频，也可能是视频
        AVStream *stream = formatContext->streams[i];
        // 解码参数：包含了这段流的各种参数信息(宽、高、码率、帧率)
        AVCodecParameters *codecpar = stream->codecpar;

        // 4、通过当前流使用的编码方式，查找解码器
        AVCodec *dec = avcodec_find_decoder(codecpar->codec_id);
        if (dec == NULL) {
            LOG_E("查找解码器失败: %s", av_err2str(ret));
            callHelper->onError(THREAD_CHILD, FFMPEG_FIND_DECODER_FAIL);
            return;
        }

        // 5、创建解码器上下文
        AVCodecContext *context3 = avcodec_alloc_context3(dec);
        if (context3 == NULL) {
            LOG_E("创建解码器上下文失败: %s", av_err2str(ret));
            callHelper->onError(THREAD_CHILD, FFMPEG_ALLOC_CODEC_CONTEXT_FAIL);
            return;
        }

        // 6、把解码参数赋值给上下文中
        // context->width = codecpar->width;
        // context->height = codecpar->height;
        ret = avcodec_parameters_to_context(context3, codecpar);
        if (ret < 0) {
            LOG_E("创建解码器上下文参数失败: %s", av_err2str(ret));
            callHelper->onError(THREAD_CHILD, FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL);
            return;
        }

        // 7、打开解码器
        ret = avcodec_open2(context3, dec, 0);
        if (ret != 0) {
            LOG_E("打开解码器失败: %s", av_err2str(ret));
            callHelper->onError(THREAD_CHILD, FFMPEG_OPEN_DECODER_FAIL);
            return;
        }

        if (codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioChannel = new AudioChannel;
        } else if (codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoChannel = new VideoChannel;
        }
    }

    if (!audioChannel && !videoChannel) {
        LOG_E("没有音频和视频");
        callHelper->onError(THREAD_CHILD, FFMPEG_NOMEDIA);
        return;
    }

    callHelper->onPrepared(THREAD_CHILD);
}
