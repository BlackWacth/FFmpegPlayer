//
// Created by Administrator on 2018/9/8.
//

#ifndef FFMPEGPLAYER_SAFEQUEUE_H
#define FFMPEGPLAYER_SAFEQUEUE_H


#include <queue>
#include <pthread.h>

using namespace std;

template<typename T>
class SafeQueue {
    typedef void (*ReleaseCallback)(T *);

    typedef void (*SyncHandle)(queue<T> &);


public:
    SafeQueue() {
        pthread_cond_init(&cond, NULL);
        pthread_mutex_init(&mutex, NULL);
    }

    ~SafeQueue() {
        pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&mutex);
    }

    void push(const T new_value) {
        pthread_mutex_lock(&mutex);
        if (work) {
            q.push(new_value);
            pthread_cond_signal(&cond);
        }
        pthread_mutex_unlock(&mutex);
    }

    int pop(T &value) {
        int ret = 0;
        pthread_mutex_lock(&mutex);
        while (work && q.empty()) {
            pthread_cond_wait(&cond, &mutex);
        }
        if (!q.empty()) {
            value = q.front();
            q.pop();
            ret = 1;
        }
        pthread_mutex_unlock(&mutex);
        return ret;
    }

    void setWork(int work) {
        pthread_mutex_lock(&mutex);
        this->work = work;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    int empty() {
        return q.empty();
    }

    int size() {
        return q.size();
    }

    void clear() {
        pthread_mutex_lock(&mutex);
        int size = SafeQueue::size();
        for (int i = 0; i < size; ++i) {
            releaseCallback(&(q.front()));
            q.pop();
        }
        pthread_mutex_unlock(&mutex);
    }

    void sync() {
        pthread_mutex_lock(&mutex);
        syncHandle(q);
        pthread_mutex_unlock(&mutex);
    }

    void setReleaseCallback(ReleaseCallback callback) {
        releaseCallback = callback;
    }

    void setSyncHandle(SyncHandle handle) {
        syncHandle = handle;
    }

private:

    pthread_cond_t cond;
    pthread_mutex_t mutex;

    queue<T> q;

    /**
     * 是否工作的标记
     * 1：工作
     * 0：不工作，不接受数据
     */
    int work;
    ReleaseCallback releaseCallback;
    SyncHandle syncHandle;

};

#endif //FFMPEGPLAYER_SAFEQUEUE_H
