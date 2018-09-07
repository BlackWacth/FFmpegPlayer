//
// Created by Administrator on 2018/9/7.
//

#ifndef FFMPEGPLAYER_JAVACALLHELPER_H
#define FFMPEGPLAYER_JAVACALLHELPER_H


#include <jni.h>

class JavaCallHelper {
public:
    JavaCallHelper(JavaVM *vm, JNIEnv *env, jobject instance);

    ~JavaCallHelper();

    void onError(int thread, int errorCode);

    void onPrepared(int thread);

private:
    JavaVM *vm;
    JNIEnv *env;
    jobject instance;
    jmethodID onErrorId;
    jmethodID onPrepasedId;
};


#endif //FFMPEGPLAYER_JAVACALLHELPER_H
