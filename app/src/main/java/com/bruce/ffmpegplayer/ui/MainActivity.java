package com.bruce.ffmpegplayer.ui;

import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.TabLayout;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;

import com.bruce.ffmpegplayer.R;
import com.bruce.ffmpegplayer.live.LiveManager;
import com.bruce.ffmpegplayer.live.list.LiveList;
import com.bruce.ffmpegplayer.live.room.Room;
import com.bruce.ffmpegplayer.live.room.Videoinfo;
import com.trello.rxlifecycle2.android.ActivityEvent;
import com.trello.rxlifecycle2.components.support.RxAppCompatActivity;

import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.schedulers.Schedulers;
import io.reactivex.subscribers.DisposableSubscriber;

public class MainActivity extends RxAppCompatActivity implements TabLayout.BaseOnTabSelectedListener, LiveAdapter.OnItemClickListener {

    private TabLayout mTabLayout;
    private RecyclerView mRecyclerView;
    private LiveAdapter mLiveAdapter;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initTabLayout();

        initRecyclerView();


    }

    private void initRecyclerView() {
        mRecyclerView = findViewById(R.id.rv_recycler_view);
        LinearLayoutManager linearLayoutManager = new LinearLayoutManager(this);
        linearLayoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        mRecyclerView.setLayoutManager(linearLayoutManager);
        mLiveAdapter = new LiveAdapter(this);
        mLiveAdapter.setOnItemClickListener(this);
        mRecyclerView.setAdapter(mLiveAdapter);

        refreshData(mTabLayout.getTabAt(0));
    }

    private void initTabLayout() {
        mTabLayout = findViewById(R.id.tl_tab_layout);
        mTabLayout.addTab(addTab("lol", "英雄联盟"));
        mTabLayout.addTab(addTab("acg", "二次元"));
        mTabLayout.addTab(addTab("food", "美食"));
        mTabLayout.addOnTabSelectedListener(this);
    }

    private TabLayout.Tab addTab(String tag, String title) {
        TabLayout.Tab tab = mTabLayout.newTab();
        tab.setTag(tag);
        tab.setText(title);
        return tab;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mTabLayout != null) {
            mTabLayout.removeOnTabSelectedListener(this);
            mTabLayout.removeAllTabs();
        }
    }

    @Override
    public void onTabSelected(TabLayout.Tab tab) {
        refreshData(tab);
    }

    private void refreshData(TabLayout.Tab tab) {
        LiveManager.getInstance()
                .getLiveList(tab.getTag().toString())
                .compose(this.bindUntilEvent(ActivityEvent.DESTROY))
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new DisposableSubscriber<LiveList>() {
                    @Override
                    public void onNext(LiveList liveList) {
                        mLiveAdapter.setLiveList(liveList);
                        mLiveAdapter.notifyDataSetChanged();
                    }

                    @Override
                    public void onError(Throwable t) {

                    }

                    @Override
                    public void onComplete() {

                    }
                });
    }

    @Override
    public void onTabUnselected(TabLayout.Tab tab) {

    }

    @Override
    public void onTabReselected(TabLayout.Tab tab) {

    }

    @Override
    public void onItemClick(String id) {
        Log.i("hzw", "id = " + id);
        LiveManager.getInstance()
                .getLiveRoom(id)
                .compose(this.bindUntilEvent(ActivityEvent.DESTROY))
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new DisposableSubscriber<Room>() {
                    @Override
                    public void onNext(Room room) {
                        Log.i("hzw", "room = " + room);
                        if (room == null) {
                            return;
                        }
                        Videoinfo videoInfo = room.getData().getInfo().getVideoinfo();
                        Log.i("hzw", "videoInfo = " + videoInfo);
                        if (videoInfo == null) {
                            return;
                        }
                        StringBuilder sb = new StringBuilder("http://pl3.live.panda.tv/live_panda/");
                        sb.append(videoInfo.getRoom_key());
                        sb.append("_mid.flv?sign=");
                        sb.append(videoInfo.getSign());
                        sb.append("&time=");
                        sb.append(videoInfo.getTs());
                        Log.i("hzw", "url = " + sb.toString());

                        Intent intent = new Intent(MainActivity.this, PlayActivity.class);
                        intent.putExtra("url", sb.toString());
                        startActivity(intent);
                    }

                    @Override
                    public void onError(Throwable t) {

                    }

                    @Override
                    public void onComplete() {

                    }
                });
    }
}
