package com.bruce.ffmpegplayer;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private SurfaceView mSurfaceView;
    private DNPlayer mDNPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mSurfaceView = findViewById(R.id.sv_surface_view);

        mDNPlayer = new DNPlayer();
        mDNPlayer.setDataSource("rtmp://live.hkstv.hk.lxdns.com/live/hks");
        mDNPlayer.setSurfaceView(mSurfaceView);
        mDNPlayer.setOnPreparedListener(() -> runOnUiThread(() -> Toast.makeText(this, "准备完毕!!!", Toast.LENGTH_SHORT).show()));
    }

    public void onPlay(View view) {
        mDNPlayer.prepare();
    }
}
