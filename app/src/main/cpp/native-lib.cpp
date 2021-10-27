#include <jni.h>
#include <string>
#include <stdlib.h>
//本地窗口 , 在 Native 层处理图像绘制
#include <android/native_window_jni.h>



#include <string>
#include <memory>
#include <thread>
#include <iostream>
#include <iostream>
using namespace std;
extern "C"
{
#include "libavutil/opt.h"
#include "libavutil/channel_layout.h"
#include "libavutil/common.h"
#include "libavutil/imgutils.h"
#include "libavutil/mathematics.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/fifo.h"
#include "libavcodec/avcodec.h"
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>

#include "libavformat/avio.h"
#include "libavfilter/avfiltergraph.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include  "libavutil/log.h"
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_a6666_sActivity_video(JNIEnv *env, jobject thiz) {
    AVFormatContext *inputContext = nullptr;
    AVFormatContext * outputContext;
    int64_t lastReadPacktTime ;
    AVOutputFormat *ofmt = NULL;
    AVBitStreamFilterContext * vbsf = NULL;
//定义输入、输出AVFormatContext
    AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
    AVPacket pkt;
    const char *in_filename, *out_filename;
    int ret, i;
    int frame_index = 0;
    in_filename = "/storage/emulated/0/Pictures/1.mp4";
    out_filename = "/storage/emulated/0/Pictures/1.avi";

    av_register_all();
    //输入
    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {//打开媒体文件

        goto end;
    }
    if((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0){//获取视频信息

        goto end;
    }
    //MP4中使用的是H.264编码，而H.264编码有两种封装模式
    //一种是annexb模式，它是传统模式，有startcode，SPS和PPS在ES中
    //另一种是mp4模式，一般MP4、MKV、AVI都没有startcode，SPS和PPS以及其他信
//息被封装在容器中
    //每一帧前面是这一帧的长度值，很多解码器只支持annexb模式，因此需要对MP4做转换
    //在FFmpeg中用h264_mp4toannexb_filter可以进行模式转换；使用命令 - bsf
//h264_mp4toannexb就可实现转换

    vbsf = av_bitstream_filter_init("h264_mp4toannexb");

    av_dump_format(ifmt_ctx, 0, in_filename, 0);
    //初始化输出视频码流的AVFormatContext
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
    if (!ofmt_ctx) {

        ret = AVERROR_UNKNOWN;
        goto end;
    }
    ofmt = ofmt_ctx->oformat;
    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
//通过输入的AVStream创建输出的AVStream
        AVStream *in_stream = ifmt_ctx->streams[i];
        AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream-> codec->codec);//初始化AVStream
        if (!out_stream) {

            ret = AVERROR_UNKNOWN;
            goto end;
        }
        //关键：复制AVCodecContext的设置属性
        if(avcodec_copy_context(out_stream->codec,in_stream->codec)<0){

            goto end;
        }

    }
    //输出信息
    av_dump_format(ofmt_ctx, 0, out_filename, 1);
    //打开输出文件
    if (!(ofmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
//打开输出文件
        if (ret < 0) {

            goto end;
        }
    }
    //写文件头
    if (avformat_write_header(ofmt_ctx, NULL) < 0) {

        goto end;
    }

    while (1) {
        AVStream *in_stream, *out_stream;
        //得到一个AVPacket
        ret = av_read_frame(ifmt_ctx, &pkt);
        if (ret < 0)
            break;
        in_stream = ifmt_ctx->streams[pkt.stream_index];
        out_stream = ofmt_ctx->streams[pkt.stream_index];

        //=转换PTS/DTS
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;

        if (pkt.stream_index == 0) {
            AVPacket fpkt = pkt;
            int a = av_bitstream_filter_filter(vbsf,
                                               out_stream->codec, NULL, &fpkt.data, &fpkt.size,
                                               pkt.data, pkt.size, pkt.flags & AV_PKT_FLAG_KEY);
            pkt.data = fpkt.data;
            pkt.size = fpkt.size;
        }
        //写AVPacket
        if (av_write_frame(ofmt_ctx, &pkt) < 0) {
//将AVPacket（存储视频压缩码流数据）写入文件

            break;
        }

        av_packet_unref(&pkt);
        frame_index++;
    }
    //写文件尾
    av_write_trailer(ofmt_ctx);
    end:
    avformat_close_input(&ifmt_ctx);
    /*关闭输出*/
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);

}
jobject initCreateAudioTrack(JNIEnv *env) {
    /*AudioTrack(int streamType, int sampleRateInHz, int channelConfig, int audioFormat,
            int bufferSizeInBytes, int mode)*/
    jclass jAudioTrackClass = env->FindClass("android/media/AudioTrack");
    jmethodID jAudioTackCMid = env->GetMethodID(jAudioTrackClass, "<init>", "(IIIIII)V");

    int streamType = 3;
    int sampleRateInHz = 44100;
    int channelConfig = (0x4 | 0x8);
    int audioFormat = 2;
    int mode = 1;

    // int getMinBufferSize(int sampleRateInHz, int channelConfig, int audioFormat)
    jmethodID getMinBufferSizeMid = env->GetStaticMethodID(jAudioTrackClass, "getMinBufferSize",
                                                           "(III)I");
    int bufferSizeInBytes = env->CallStaticIntMethod(jAudioTrackClass, getMinBufferSizeMid,
                                                     sampleRateInHz, channelConfig, audioFormat);


    jobject jAudioTrackObj = env->NewObject(jAudioTrackClass, jAudioTackCMid, streamType,
                                            sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode);

    // play
    jmethodID playMid = env->GetMethodID(jAudioTrackClass, "play", "()V");
    env->CallVoidMethod(jAudioTrackObj, playMid);

    return jAudioTrackObj;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_a6666_sActivity_play(JNIEnv *env, jobject thiz) {
    const char *in_filename;
    in_filename = "/storage/emulated/0/Pictures/a.mp3";
    av_register_all();
    avformat_network_init();
    AVFormatContext *pFormatContext = NULL;
    int formatOpenInputRes = 0;
    int formatFindStreamInfoRes = 0;
    int audioStramIndex = -1;
    AVCodecParameters *pCodecParameters;
    AVCodec *pCodec = NULL;
    AVCodecContext *pCodecContext = NULL;
    int codecParametersToContextRes = -1;
    int codecOpenRes = -1;
    int index = 0;
    AVPacket *pPacket = NULL;
    AVFrame *pFrame = NULL;
    jobject jAudioTrackObj;
    jmethodID jWriteMid;
    jclass jAudioTrackClass;

    formatOpenInputRes = avformat_open_input(&pFormatContext,in_filename, NULL, NULL);
    if (formatOpenInputRes != 0) {
        // 第一件事，需要回调给 Java 层(下次课讲)
        // 第二件事，需要释放资源
        // return;

        goto __av_resources_destroy;
    }

    formatFindStreamInfoRes = avformat_find_stream_info(pFormatContext, NULL);
    if (formatFindStreamInfoRes < 0) {

        goto __av_resources_destroy;
    }

    // 查找音频流的 index
    audioStramIndex = av_find_best_stream(pFormatContext, AVMediaType::AVMEDIA_TYPE_AUDIO, -1, -1,
                                          NULL, 0);
    if (audioStramIndex < 0) {

        goto __av_resources_destroy;
    }

    // 查找解码
    pCodecParameters = pFormatContext->streams[audioStramIndex]->codecpar;
    pCodec = avcodec_find_decoder(pCodecParameters->codec_id);
    if (pCodec == NULL) {

        goto __av_resources_destroy;
    }
    // 打开解码器
    pCodecContext = avcodec_alloc_context3(pCodec);
    if (pCodecContext == NULL) {

        goto __av_resources_destroy;
    }
    codecParametersToContextRes = avcodec_parameters_to_context(pCodecContext, pCodecParameters);
    if (codecParametersToContextRes < 0) {

        goto __av_resources_destroy;
    }

    codecOpenRes = avcodec_open2(pCodecContext, pCodec, NULL);
    if (codecOpenRes != 0) {

        goto __av_resources_destroy;
    }

    jAudioTrackClass = env->FindClass("android/media/AudioTrack");
    jWriteMid = env->GetMethodID(jAudioTrackClass, "write", "([BII)I");
    jAudioTrackObj = initCreateAudioTrack(env);

    pPacket = av_packet_alloc();
    pFrame = av_frame_alloc();
    while (av_read_frame(pFormatContext, pPacket) >= 0) {
        if (pPacket->stream_index == audioStramIndex) {
            // Packet 包，压缩的数据，解码成 pcm 数据
            int codecSendPacketRes = avcodec_send_packet(pCodecContext, pPacket);
            if (codecSendPacketRes == 0) {
                int codecReceiveFrameRes = avcodec_receive_frame(pCodecContext, pFrame);
                if (codecReceiveFrameRes == 0) {
                    // AVPacket -> AVFrame
                    index++;
                    

                    // write 写到缓冲区 pFrame.data -> javabyte
                    // size 是多大，装 pcm 的数据
                    // 1s 44100 点  2通道 ，2字节    44100*2*2
                    // 1帧不是一秒，pFrame->nb_samples点
                    int dataSize = av_samples_get_buffer_size(NULL, pFrame->channels,
                                                              pFrame->nb_samples, pCodecContext->sample_fmt, 0);
                    jbyteArray jPcmByteArray = env->NewByteArray(dataSize);
                    // native 创建 c 数组
                    jbyte *jPcmData = env->GetByteArrayElements(jPcmByteArray, NULL);
                    memcpy(jPcmData, pFrame->data, dataSize);
                    // 0 把 c 的数组的数据同步到 jbyteArray , 然后释放native数组
                    env->ReleaseByteArrayElements(jPcmByteArray, jPcmData, 0);
                    env->CallIntMethod(jAudioTrackObj, jWriteMid, jPcmByteArray, 0, dataSize);
                    // 解除 jPcmDataArray 的持有，让 javaGC 回收
                    env->DeleteLocalRef(jPcmByteArray);
                }
            }
        }
        // 解引用
        av_packet_unref(pPacket);
        av_frame_unref(pFrame);
    }

    // 1. 解引用数据 data ， 2. 销毁 pPacket 结构体内存  3. pPacket = NULL
    av_packet_free(&pPacket);
    av_frame_free(&pFrame);
    env->DeleteLocalRef(jAudioTrackObj);

    __av_resources_destroy:
    if (pCodecContext != NULL) {
        avcodec_close(pCodecContext);
        avcodec_free_context(&pCodecContext);
        pCodecContext = NULL;
    }

    if (pFormatContext != NULL) {
        avformat_close_input(&pFormatContext);
        avformat_free_context(pFormatContext);
        pFormatContext = NULL;
    }
    avformat_network_deinit();
}
#include <malloc.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include <jni.h>
#include "DZJNICall.h"
#include "DZFFmpeg.h"
#include "DZConstDefine.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
// 在 c++ 中采用 c 的这种编译方式
extern "C" {
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}

DZJNICall *pJniCall;
DZFFmpeg *pFFmpeg;

JavaVM *pJavaVM = NULL;

// 重写 so 被加载时会调用的一个方法
// 小作业，去了解动态注册
extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *javaVM, void *reserved) {
    pJavaVM = javaVM;
    JNIEnv *env;
    if (javaVM->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_4;
}



extern "C" JNIEXPORT void JNICALL
Java_com_example_a6666_DarrenPlayer_nPlay(JNIEnv *env, jobject instance) {
    if (pFFmpeg != NULL) {
        pFFmpeg->play();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_a6666_DarrenPlayer_nPrepare(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);
    if (pFFmpeg == NULL) {
        pJniCall = new DZJNICall(pJavaVM, env, instance);
        pFFmpeg = new DZFFmpeg(pJniCall, url);
        pFFmpeg->prepare();
    }
    env->ReleaseStringUTFChars(url_, url);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_a6666_DarrenPlayer_nPrepareAsync(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);
    if (pFFmpeg == NULL) {
        pJniCall = new DZJNICall(pJavaVM, env, instance);
        pFFmpeg = new DZFFmpeg(pJniCall, url);
        pFFmpeg->prepareAsync();
    }
    env->ReleaseStringUTFChars(url_, url);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_a6666_DarrenPlayer_setSurface(JNIEnv *env, jobject instance, jobject surface) {
    if(pFFmpeg != NULL){
        pFFmpeg->setSurface(surface);
    }
}