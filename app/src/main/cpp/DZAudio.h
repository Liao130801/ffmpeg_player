//
// Created by hcDarren on 2019/6/22.
//

#ifndef MUSICPLAYER_DZAUDIO_H
#define MUSICPLAYER_DZAUDIO_H

#include <pthread.h>
#include "DZJNICall.h"
#include "DZConstDefine.h"
#include "DZPacketQueue.h"
#include "DZPlayerStatus.h"
#include "DZMedia.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
};


class DZAudio : public DZMedia {
public:
    AVFormatContext *pFormatContext = NULL;
    SwrContext *pSwrContext = NULL;
    uint8_t *resampleOutBuffer = NULL;
public:
    DZAudio(int audioStreamIndex, DZJNICall *pJniCall, DZPlayerStatus *pPlayerStatus);

    ~DZAudio();

    void play();

    void initCrateOpenSLES();

    int resampleAudio();

    void privateAnalysisStream(ThreadMode threadMode, AVFormatContext *pFormatContext);

    void release();
};


#endif //MUSICPLAYER_DZAUDIO_H
