#include <jni.h>
#include <string>
#include "JavaCallHelper.h"
#include "DNFFmpeg.h"
#include <android/native_window_jni.h>


DNFFmpeg *ffmpeg = NULL;
JavaVM *javaVM = 0;

ANativeWindow *window = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVM = vm;
    return JNI_VERSION_1_6;
}

void render(uint8_t *data, int line_size, int w, int h) {
    pthread_mutex_lock(&mutex);
    if (!window) {
        pthread_mutex_unlock(&mutex);
        return;
    }

    //设置窗口属性
    ANativeWindow_setBuffersGeometry(window, w, h, WINDOW_FORMAT_RGBA_8888);

    ANativeWindow_Buffer window_buffer;
    if (ANativeWindow_lock(window, &window_buffer, 0)) {
        ANativeWindow_release(window);
        window = 0;
        pthread_mutex_unlock(&mutex);
        return;
    }

    uint8_t *dst_data = static_cast<uint8_t *>(window_buffer.bits);
    int dst_line_size = window_buffer.stride * 4;
    for (int i = 0; i < window_buffer.height; ++i) {
        memcpy(dst_data + i * dst_line_size, data + i * line_size,
               static_cast<size_t>(dst_line_size));
    }
    ANativeWindow_unlockAndPost(window);
    pthread_mutex_unlock(&mutex);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_bruce_ffmpegplayer_player_DNPlayer_native_1prepare(JNIEnv *env, jobject instance,
                                                     jstring dataSource_) {
    const char *dataSource = env->GetStringUTFChars(dataSource_, 0);
    JavaCallHelper *helper = new JavaCallHelper(javaVM, env, instance);
    ffmpeg = new DNFFmpeg(helper, dataSource);
    ffmpeg->setRenderFrameCallback(render);
    ffmpeg->prepare();
    env->ReleaseStringUTFChars(dataSource_, dataSource);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_bruce_ffmpegplayer_player_DNPlayer_native_1start(JNIEnv *env, jobject instance) {
    ffmpeg->start();

}


extern "C"
JNIEXPORT void JNICALL
Java_com_bruce_ffmpegplayer_player_DNPlayer_native_1setSurface(JNIEnv *env, jobject instance,
                                                        jobject surface) {
    pthread_mutex_lock(&mutex);
    if (window) {
        ANativeWindow_release(window);
        window = 0;
    }
    window = ANativeWindow_fromSurface(env, surface);
    pthread_mutex_unlock(&mutex);
}