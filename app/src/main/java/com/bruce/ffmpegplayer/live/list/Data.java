package com.bruce.ffmpegplayer.live.list;

import java.util.List;

public class Data {

    private List<Items> items;

    public void setItems(List<Items> items) {
        this.items = items;
    }

    public List<Items> getItems() {
        return items;
    }

    @Override
    public String toString() {
        return "Data{" +
                "items=" + items +
                '}';
    }
}