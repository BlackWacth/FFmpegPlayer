package com.bruce.ffmpegplayer;

import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class DNPlayer implements SurfaceHolder.Callback {

    static {
        System.loadLibrary("native-lib");
    }

    private String mDataSource;
    private SurfaceHolder mSurfaceHolder;
    private OnPreparedListener mOnPreparedListener;

    public String getDataSource() {
        return mDataSource;
    }

    public void setDataSource(String dataSource) {
        mDataSource = dataSource;
    }

    public void setSurfaceView(SurfaceView surfaceView) {
        mSurfaceHolder = surfaceView.getHolder();
        mSurfaceHolder.addCallback(this);
    }

    public OnPreparedListener getOnPreparedListener() {
        return mOnPreparedListener;
    }

    public void setOnPreparedListener(OnPreparedListener onPreparedListener) {
        mOnPreparedListener = onPreparedListener;
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public void prepare() {
        native_prepare(mDataSource);
    }



    public void start() {

    }

    public void stop() {

    }

    public void release() {
        mSurfaceHolder.removeCallback(this);
    }

    public void onError(int errorCode){
        Log.i("hzw", "Java接到回调:"+errorCode);
    }

    public void onPrepared() {
        if (mOnPreparedListener != null) {
            mOnPreparedListener.onPrepared();
        }
    }

    public native void native_prepare(String dataSource);
}
