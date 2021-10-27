//
// Created by hcDarren on 2019/6/16.
//

#include "DZFFmpeg.h"

DZFFmpeg::DZFFmpeg(DZJNICall *pJniCall, const char *url) {
    this->pJniCall = pJniCall;
    // 赋值一份 url ，因为怕外面方法结束销毁了 url
    this->url = (char *) malloc(strlen(url) + 1);
    memcpy(this->url, url, strlen(url) + 1);

    pPlayerStatus = new DZPlayerStatus();
}

DZFFmpeg::~DZFFmpeg() {
    release();
}

void *threadReadPacket(void *context) {
    DZFFmpeg *pFFmpeg = (DZFFmpeg *) context;
    while (pFFmpeg->pPlayerStatus != NULL && !pFFmpeg->pPlayerStatus->isExit) {
        AVPacket *pPacket = av_packet_alloc();
        if (av_read_frame(pFFmpeg->pFormatContext, pPacket) >= 0) {
            if (pPacket->stream_index == pFFmpeg->pAudio->streamIndex) {
                pFFmpeg->pAudio->pPacketQueue->push(pPacket);
            } else if (pPacket->stream_index == pFFmpeg->pVideo->streamIndex) {
                pFFmpeg->pVideo->pPacketQueue->push(pPacket);
            } else {
                // 1. 解引用数据 data ， 2. 销毁 pPacket 结构体内存  3. pPacket = NULL
                av_packet_free(&pPacket);
            }
        } else {
            // 1. 解引用数据 data ， 2. 销毁 pPacket 结构体内存  3. pPacket = NULL
            av_packet_free(&pPacket);
            // 睡眠一下，尽量不去消耗 cpu 的资源，也可以退出销毁这个线程
            // break;
        }
    }
    return 0;
}

void DZFFmpeg::play() {
    // 一个线程去读取 Packet
    pthread_t readPacketThreadT;
    pthread_create(&readPacketThreadT, NULL, threadReadPacket, this);
    pthread_detach(readPacketThreadT);

    if (pAudio != NULL) {
        pAudio->play();
    }

    if (pVideo != NULL) {
        pVideo->play();
    }
}

void DZFFmpeg::callPlayerJniError(ThreadMode threadMode, int code, char *msg) {
    // 释放资源
    release();
    // 回调给 java 层调用
    pJniCall->callPlayerError(threadMode, code, msg);
}

void DZFFmpeg::release() {
    if (pFormatContext != NULL) {
        avformat_close_input(&pFormatContext);
        avformat_free_context(pFormatContext);
        pFormatContext = NULL;
    }

    avformat_network_deinit();

    if (url != NULL) {
        free(url);
        url = NULL;
    }

    if (pPlayerStatus != NULL) {
        delete (pPlayerStatus);
        pPlayerStatus = NULL;
    }

    if (pAudio != NULL) {
        delete (pAudio);
        pAudio = NULL;
    }

    if (pVideo != NULL) {
        delete (pVideo);
        pVideo = NULL;
    }
}

void DZFFmpeg::prepare() {
    prepare(THREAD_MAIN);
}

void *threadPrepare(void *context) {
    DZFFmpeg *pFFmpeg = (DZFFmpeg *) context;
    pFFmpeg->prepare(THREAD_CHILD);
    return 0;
}

void DZFFmpeg::prepareAsync() {
    // 创建一个线程去播放，多线程编解码边播放
    pthread_t prepareThreadT;
    pthread_create(&prepareThreadT, NULL, threadPrepare, this);
    pthread_detach(prepareThreadT);
}

void DZFFmpeg::prepare(ThreadMode threadMode) {
    // 讲的理念的东西，千万要注意
    av_register_all();
    avformat_network_init();
    int formatOpenInputRes = 0;
    int formatFindStreamInfoRes = 0;

    formatOpenInputRes = avformat_open_input(&pFormatContext, url, NULL, NULL);
    if (formatOpenInputRes != 0) {
        // 第一件事，需要回调给 Java 层(下次课讲)
        // 第二件事，需要释放资源
        LOGE("format open input error: %s", av_err2str(formatOpenInputRes));
        callPlayerJniError(threadMode, formatOpenInputRes, av_err2str(formatOpenInputRes));
        return;
    }

    formatFindStreamInfoRes = avformat_find_stream_info(pFormatContext, NULL);
    if (formatFindStreamInfoRes < 0) {
        LOGE("format find stream info error: %s", av_err2str(formatFindStreamInfoRes));
        // 这种方式一般不推荐这么写，但是的确方便
        callPlayerJniError(threadMode, formatFindStreamInfoRes,
                av_err2str(formatFindStreamInfoRes));
        return;
    }

    // 查找音频流的 index
    int audioStreamIndex = av_find_best_stream(pFormatContext, AVMediaType::AVMEDIA_TYPE_AUDIO, -1,
            -1, NULL, 0);
    if (audioStreamIndex < 0) {
        LOGE("format audio stream error.");
        // 这种方式一般不推荐这么写，但是的确方便
        callPlayerJniError(threadMode, FIND_STREAM_ERROR_CODE, "find audio stream error");
        return;
    }

    // 不是我的事我不干，但是大家也不要想得过于复杂
    pAudio = new DZAudio(audioStreamIndex, pJniCall, pPlayerStatus);
    pAudio->analysisStream(threadMode, pFormatContext);

    // 查找视频流的 index
    int videoStreamIndex = av_find_best_stream(pFormatContext, AVMediaType::AVMEDIA_TYPE_VIDEO, -1,
            -1, NULL, 0);
    // 如果没有视频就直接播放音频
    /* if (videoStreamIndex < 0) {
        return;
    }*/
    if (videoStreamIndex < 0) {
        LOGE("format video stream error.");
        // 这种方式一般不推荐这么写，但是的确方便
        callPlayerJniError(threadMode, FIND_STREAM_ERROR_CODE, "find video stream error");
        return;
    }
    // 不是我的事我不干，但是大家也不要想得过于复杂
    pVideo = new DZVideo(videoStreamIndex, pJniCall, pPlayerStatus, pAudio);
    pVideo->analysisStream(threadMode, pFormatContext);

    // 回调到 Java 告诉他准备好了
    pJniCall->callPlayerPrepared(threadMode);
}

void DZFFmpeg::setSurface(jobject surface) {
    if (pVideo != NULL) {
        pVideo->setSurface(surface);
    }
}
