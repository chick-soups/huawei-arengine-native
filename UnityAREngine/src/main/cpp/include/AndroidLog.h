//
// Created by 周旭华 on 2024/4/27.
//

#ifndef HUAWEI_ARENGINE_NATIVE_DEMO_ANDROIDLOG_H
#define HUAWEI_ARENGINE_NATIVE_DEMO_ANDROIDLOG_H
#include <android/log.h>

#ifndef LOG_INFO
#define LOG_INFO(tag,...) \
    __android_log_print(ANDROID_LOG_INFO, tag, __VA_ARGS__)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(tag,...) \
    __android_log_print(ANDROID_LOG_ERROR, tag, __VA_ARGS__)
#endif


#endif //HUAWEI_ARENGINE_NATIVE_DEMO_ANDROIDLOG_H
