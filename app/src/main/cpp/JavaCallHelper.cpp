//
// Created by Administrator on 2018/9/7.
//

#include "JavaCallHelper.h"
#include "macro.h"

JavaCallHelper::JavaCallHelper(JavaVM *vm, JNIEnv *env, jobject instance) {
    this->vm = vm;
    this->env = env;
    // 一旦涉及到jobject 跨方法 跨线程 就需要创建全局引用
    this->instance = env->NewGlobalRef(instance);
    jclass clazz = env->GetObjectClass(instance);
    this->onErrorId = env->GetMethodID(clazz, "onError", "(I)V");
    this->onPrepasedId = env->GetMethodID(clazz, "onPrepared", "()V");
}

JavaCallHelper::~JavaCallHelper() {
    env->DeleteGlobalRef(instance);
}

void JavaCallHelper::onError(int thread, int errorCode) {
    if (thread == THREAD_MAIN) {
        env->CallVoidMethod(instance, onErrorId, errorCode);
    } else if (thread == THREAD_CHILD) {
        //子线程中, 重新获取属于子线程的JNIEnv
        JNIEnv *env;
        vm->AttachCurrentThread(&env, 0);
        env->CallVoidMethod(instance, onErrorId, errorCode);
        vm->DetachCurrentThread();
    }
}

void JavaCallHelper::onPrepared(int thread) {
    if (thread == THREAD_MAIN) {
        env->CallVoidMethod(instance, onPrepasedId);
    } else if (thread == THREAD_CHILD) {
        //子线程中, 重新获取属于子线程的JNIEnv
        JNIEnv *env;
        vm->AttachCurrentThread(&env, 0);
        env->CallVoidMethod(instance, onPrepasedId);
        vm->DetachCurrentThread();
    }
}
