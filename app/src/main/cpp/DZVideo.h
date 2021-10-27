//
// Created by hcDarren on 2019/6/30.
//

#ifndef VIDEOPLAYER_DZVIDEO_H
#define VIDEOPLAYER_DZVIDEO_H

#include "DZMedia.h"
#include "DZAudio.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>

extern "C" {
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
};

class DZVideo : public DZMedia {
public:
    SwsContext *pSwsContext = NULL;
    uint8_t *pFrameBuffer = NULL;
    int frameSize;
    AVFrame *pRgbaFrame;
    jobject surface;
    DZAudio *pAudio;
    /**
     * 视频的延时时间
     */
    double delayTime = 0;

    /**
     * 默认情况下最合适的一个延迟时间，动态获取
     */
    double defaultDelayTime = 0.04;
public:
    DZVideo(int streamIndex, DZJNICall *pJniCall, DZPlayerStatus *pPlayerStatus, DZAudio *pAudio);

    ~DZVideo();

public:
    void play();

    void privateAnalysisStream(ThreadMode threadMode, AVFormatContext *pFormatContext);

    void release();

    void setSurface(jobject surface);

    /**
     * 视频同步音频，计算获取休眠的时间
     * @param pFrame 当前视频帧
     * @return 休眠时间（s）
     */
    double getFrameSleepTime(AVFrame *pFrame);
};


#endif //VIDEOPLAYER_DZVIDEO_H
