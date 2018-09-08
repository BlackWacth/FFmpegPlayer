package com.bruce.ffmpegplayer.live;

import com.bruce.ffmpegplayer.live.list.LiveList;
import com.bruce.ffmpegplayer.live.room.Room;

import io.reactivex.Flowable;
import retrofit2.Retrofit;
import retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory;
import retrofit2.converter.gson.GsonConverterFactory;

public class LiveManager {

    private final LiveService mLiveService;
    private volatile static LiveManager sLiveManager;

    private LiveManager() {
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl("http://api.m.panda.tv/")
                .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        mLiveService = retrofit.create(LiveService.class);
    }

    public static LiveManager getInstance() {
        if (sLiveManager == null) {
            synchronized (LiveManager.class) {
                if (sLiveManager == null) {
                    sLiveManager = new LiveManager();
                }
            }
        }
        return sLiveManager;
    }

    public Flowable<LiveList> getLiveList(String cate) {

        return mLiveService.getLiveList(cate, 1, 10, "3.3.1.5978");
    }

    public Flowable<Room> getLiveRoom(String id) {
        return mLiveService.getLiveRoom(id, "3.3.1.5978", 1, "json", "android");
    }
}
