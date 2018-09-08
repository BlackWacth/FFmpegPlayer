package com.bruce.ffmpegplayer.ui;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.bruce.ffmpegplayer.R;
import com.bruce.ffmpegplayer.live.list.Items;
import com.bruce.ffmpegplayer.live.list.LiveList;
import com.bumptech.glide.Glide;

import java.util.ArrayList;
import java.util.List;

public class LiveAdapter extends RecyclerView.Adapter<LiveAdapter.LiveHolder> implements View.OnClickListener {

    private LayoutInflater mLayoutInflater;
    private List<Items> mItems;
    private OnItemClickListener mOnItemClickListener;

    public LiveAdapter(Context context) {
        mLayoutInflater = LayoutInflater.from(context);
        mItems = new ArrayList<>();
    }

    @NonNull
    @Override
    public LiveHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
        View view = mLayoutInflater.inflate(R.layout.item_room, viewGroup, false);
        //点击
        view.setOnClickListener(this);
        return new LiveHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull LiveHolder liveHolder, int i) {
        Items items = this.mItems.get(i);
        liveHolder.title.setText(items.getName());
        Glide.with(liveHolder.picture)
                .load(items.getPictures().getImg())
                .into(liveHolder.picture);
        //设置标签
        liveHolder.itemView.setTag(items.getId());
    }

    @Override
    public int getItemCount() {
        return mItems == null ? 0 : mItems.size();
    }

    @Override
    public void onClick(View v) {
        if (mOnItemClickListener != null) {
            mOnItemClickListener.onItemClick((String) v.getTag());
        }
    }

    public void setLiveList(LiveList liveList) {
        mItems.clear();
        mItems.addAll(liveList.getData().getItems());
    }

    public OnItemClickListener getOnItemClickListener() {
        return mOnItemClickListener;
    }

    public void setOnItemClickListener(OnItemClickListener onItemClickListener) {
        mOnItemClickListener = onItemClickListener;
    }

    class LiveHolder extends RecyclerView.ViewHolder {
        ImageView picture;
        TextView title;

        public LiveHolder(@NonNull View itemView) {
            super(itemView);
            picture = itemView.findViewById(R.id.picture);
            title = itemView.findViewById(R.id.title);
        }
    }

    public interface OnItemClickListener {
        void onItemClick(String id);
    }
}
