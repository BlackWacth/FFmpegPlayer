package com.bruce.ffmpegplayer.live.room;


public class Data {

    private Info info;

    public void setInfo(Info info) {
        this.info = info;
    }

    public Info getInfo() {
        return info;
    }

    @Override
    public String toString() {
        return "Data{" +
                "info=" + info +
                '}';
    }
}