//
// Created by Administrator on 2018/9/7.
//

#include "VideoChannel.h"
#include "macro.h"

extern "C" {
#include <libavutil/imgutils.h>
}

void *decode_task(void *args) {
    VideoChannel *channel = static_cast<VideoChannel *>(args);
    channel->decode();
    return 0;
}

void *render_task(void *args) {
    VideoChannel *channel = static_cast<VideoChannel *>(args);
    channel->render();
    return 0;
}

VideoChannel::VideoChannel(int id, AVCodecContext *avCodecContext) : BaseChannel(id,
                                                                                 avCodecContext) {

}

VideoChannel::~VideoChannel() {
    frames.setReleaseCallback(releaseAVFrame);
    frames.clear();

}

void VideoChannel::play() {
    isPlaying = 1;
    pthread_create(&pid_decode, 0, decode_task, this);
    pthread_create(&pid_render, 0, render_task, this);
}

void VideoChannel::decode() {
    AVPacket *packet = 0;
    int ret;
    while (isPlaying) {
        ret = packets.pop(packet);
        if (!isPlaying) {
            break;
        }
        if (!ret) {
            LOG_E("取出 AVPacket 失败");
            continue;
        }

        //数据包发送给解码器
        ret = avcodec_send_packet(avCodecContext, packet);
        releaseAVPacket(&packet);
        if (ret != 0) {
            LOG_E("数据包发送给解码器失败");
            return;
        }

        //申请一个帧，由默认数据填充
        AVFrame *frame = av_frame_alloc();
        //解码赋值给帧
        ret = avcodec_receive_frame(avCodecContext, frame);

        if (ret == AVERROR(EAGAIN)) {
            LOG_E("解码一帧的数据不够");
            continue;
        } else if (ret != 0) {
            LOG_E("解码失败");
            break;
        }
        frames.push(frame);
    }
    releaseAVPacket(&packet);
}

void VideoChannel::render() {

    swsContext = sws_getContext(
            avCodecContext->width, avCodecContext->height, avCodecContext->pix_fmt,
            avCodecContext->width, avCodecContext->height, AV_PIX_FMT_RGBA,
            SWS_BILINEAR, 0, 0, 0
    );

    AVFrame *frame = 0;
    uint8_t *dst_data[4];
    int dst_linesize[4];
    av_image_alloc(dst_data, dst_linesize,
                   avCodecContext->width, avCodecContext->height,
                   AV_PIX_FMT_RGBA, 1);
    int ret;
    while (isPlaying) {
        ret = frames.pop(frame);
        if (!isPlaying) {
            break;
        }
        sws_scale(swsContext, reinterpret_cast<const uint8_t *const *>(frame->data),
                  frame->linesize, 0, avCodecContext->height, dst_data, dst_linesize);

        callback(dst_data[0], dst_linesize[0], avCodecContext->width, avCodecContext->height);
        releaseAVFrame(&frame);
    }
    av_freep(&dst_data[0]);
    releaseAVFrame(&frame);
}
