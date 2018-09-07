#include <jni.h>
#include <string>
#include "JavaCallHelper.h"
#include "DNFFmpeg.h"


DNFFmpeg *ffmpeg = NULL;
JavaVM *javaVM = 0;

int JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVM = vm;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_bruce_ffmpegplayer_DNPlayer_native_1prepare(JNIEnv *env, jobject instance,
                                                      jstring dataSource_) {
    const char *dataSource = env->GetStringUTFChars(dataSource_, 0);
    JavaCallHelper *helper = new JavaCallHelper(javaVM, env, instance);
    ffmpeg = new DNFFmpeg(helper, dataSource);
    ffmpeg->prepare();
    env->ReleaseStringUTFChars(dataSource_, dataSource);
}
