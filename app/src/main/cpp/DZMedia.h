//
// Created by hcDarren on 2019/7/6.
//

#ifndef VIDEOPLAYER_DZMEDIA_H
#define VIDEOPLAYER_DZMEDIA_H

#include "DZJNICall.h"
#include "DZPacketQueue.h"
#include "DZPlayerStatus.h"
#include "DZConstDefine.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

class DZMedia {
public:
    int streamIndex = -1;
    AVCodecContext *pCodecContext = NULL;
    DZJNICall *pJniCall = NULL;
    DZPacketQueue *pPacketQueue = NULL;
    DZPlayerStatus *pPlayerStatus = NULL;
    /**
     * 整个视频的时长
     */
    int duration = 0;
    /**
     * 记录当前播放时间
     */
    double currentTime = 0;

    /**
     * 上次更新的时间（回调到 java 层）
     */
    double lastUpdateTime = 0;

    /**
     * 时间基
     */
    AVRational timeBase;
public:
    DZMedia(int streamIndex, DZJNICall *pJniCall, DZPlayerStatus *pPlayerStatus);

    ~DZMedia();

public:
    virtual void play() = 0;

    void analysisStream(ThreadMode threadMode, AVFormatContext *pFormatContext);

    virtual void privateAnalysisStream(ThreadMode threadMode, AVFormatContext *pFormatContext) = 0;

    virtual void release();

    void callPlayerJniError(ThreadMode threadMode, int code, char *msg);

private:
    void publicAnalysisStream(ThreadMode threadMode, AVFormatContext *pFormatContext);
};


#endif //VIDEOPLAYER_DZMEDIA_H
