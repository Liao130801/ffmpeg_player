//
// Created by hcDarren on 2019/6/16.
//

#ifndef MUSICPLAYER_DZFFMPEG_H
#define MUSICPLAYER_DZFFMPEG_H
#include "DZJNICall.h"
#include "DZAudio.h"
#include <pthread.h>
#include "DZConstDefine.h"
#include "DZVideo.h"

extern "C"{
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
};

class DZFFmpeg {
public:
    AVFormatContext *pFormatContext = NULL;
    char* url = NULL;
    DZJNICall *pJniCall = NULL;
    DZPlayerStatus *pPlayerStatus;
    DZAudio *pAudio = NULL;
    DZVideo *pVideo;

public:
    DZFFmpeg(DZJNICall *pJniCall, const char* url);
    ~DZFFmpeg();

public:
    void play();

    void prepare();

    void prepareAsync();

    void prepare(ThreadMode threadMode);

    void callPlayerJniError(ThreadMode threadMode, int code, char* msg);

    void release();

    void setSurface(jobject surface);


};


#endif //MUSICPLAYER_DZFFMPEG_H
