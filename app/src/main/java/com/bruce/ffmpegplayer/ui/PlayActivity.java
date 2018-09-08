package com.bruce.ffmpegplayer.ui;

import android.content.res.Configuration;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.widget.Toast;

import com.bruce.ffmpegplayer.R;
import com.bruce.ffmpegplayer.player.DNPlayer;

public class PlayActivity extends AppCompatActivity {

    private DNPlayer mDNPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager
                .LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_play);
        SurfaceView surfaceView = findViewById(R.id.sv_surface_view);
        mDNPlayer = new DNPlayer();
        mDNPlayer.setSurfaceView(surfaceView);
        mDNPlayer.setDataSource(getIntent().getStringExtra("url"));
        mDNPlayer.setOnPreparedListener(() -> {
            runOnUiThread(() -> Toast.makeText(PlayActivity.this, "初始化完毕", Toast.LENGTH_SHORT).show());
            mDNPlayer.start();
        });
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
            getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager
                    .LayoutParams.FLAG_FULLSCREEN);
        } else {
            getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        }
        setContentView(R.layout.activity_play);
        SurfaceView surfaceView = findViewById(R.id.sv_surface_view);
        mDNPlayer.setSurfaceView(surfaceView);
    }

    @Override
    protected void onResume() {
        super.onResume();
        mDNPlayer.prepare();
    }

    @Override
    protected void onStop() {
        super.onStop();
        if (mDNPlayer != null) {
            mDNPlayer.stop();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mDNPlayer != null) {
            mDNPlayer.release();
            mDNPlayer = null;
        }
    }
}
