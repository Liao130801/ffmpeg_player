#include <jni.h>
#include <string>
#include <android/native_window_jni.h>//
// Created by HUAWEI on 2021/7/23.
//
#include <pthread.h>
#include <cstring>
extern "C"{
#include <libavcodec/avcodec.h>
};
extern "C"{
#include <libswscale/swscale.h>
}
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/time.h>
}


