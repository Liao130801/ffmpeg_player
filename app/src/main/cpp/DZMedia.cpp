//
// Created by hcDarren on 2019/7/6.
//

#include "DZMedia.h"

void DZMedia::analysisStream(ThreadMode threadMode, AVFormatContext *pFormatContext) {
    publicAnalysisStream(threadMode, pFormatContext);
    privateAnalysisStream(threadMode, pFormatContext);
}

void DZMedia::release() {
    if (pPacketQueue) {
        delete (pPacketQueue);
        pPacketQueue = NULL;
    }

    if (pCodecContext != NULL) {
        avcodec_close(pCodecContext);
        avcodec_free_context(&pCodecContext);
        pCodecContext = NULL;
    }
}

DZMedia::~DZMedia() {
    release();
}

DZMedia::DZMedia(int streamIndex, DZJNICall *pJniCall, DZPlayerStatus *pPlayerStatus) {
    this->streamIndex = streamIndex;
    this->pJniCall = pJniCall;
    this->pPlayerStatus = pPlayerStatus;
    pPacketQueue = new DZPacketQueue();
}

void DZMedia::publicAnalysisStream(ThreadMode threadMode, AVFormatContext *pFormatContext) {
    // 查找解码
    AVCodecParameters *pCodecParameters = pFormatContext->streams[streamIndex]->codecpar;
    AVCodec *pCodec = avcodec_find_decoder(pCodecParameters->codec_id);
    if (pCodec == NULL) {
        LOGE("codec find audio decoder error");
        callPlayerJniError(threadMode, CODEC_FIND_DECODER_ERROR_CODE,
                "codec find audio decoder error");
        return;
    }
    // 打开解码器
    pCodecContext = avcodec_alloc_context3(pCodec);
    if (pCodecContext == NULL) {
        LOGE("codec alloc context error");
        callPlayerJniError(threadMode, CODEC_ALLOC_CONTEXT_ERROR_CODE, "codec alloc context error");
        return;
    }
    int codecParametersToContextRes = avcodec_parameters_to_context(pCodecContext,
            pCodecParameters);
    if (codecParametersToContextRes < 0) {
        LOGE("codec parameters to context error: %s", av_err2str(codecParametersToContextRes));
        callPlayerJniError(threadMode, codecParametersToContextRes,
                av_err2str(codecParametersToContextRes));
        return;
    }

    int codecOpenRes = avcodec_open2(pCodecContext, pCodec, NULL);
    if (codecOpenRes != 0) {
        LOGE("codec audio open error: %s", av_err2str(codecOpenRes));
        callPlayerJniError(threadMode, codecOpenRes, av_err2str(codecOpenRes));
        return;
    }
    duration = pFormatContext->duration;
    timeBase = pFormatContext->streams[streamIndex]->time_base;
}

void DZMedia::callPlayerJniError(ThreadMode threadMode, int code, char *msg) {
    // 释放资源
    release();
    // 回调给 java 层调用
    pJniCall->callPlayerError(threadMode, code, msg);
}
