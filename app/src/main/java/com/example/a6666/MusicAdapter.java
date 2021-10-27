package com.example.a6666;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.ArrayList;

public class MusicAdapter extends BaseAdapter {
    Context context;
    LayoutInflater mInflater;
    ArrayList<MusicContent> arrayList;

    public MusicAdapter(Context context, ArrayList<MusicContent> arrayList) {
        this.context = context;
        this.arrayList = arrayList;
        this.mInflater = LayoutInflater.from(context);
    }

    @Override
    public int getCount() {
        return arrayList.size();
    }

    @Override
    public Object getItem(int position) {
        return arrayList.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder viewHolder;
        if (convertView == null) {
            convertView = mInflater.inflate(R.layout.tv, null);
            TextView tv = (TextView) convertView.findViewById(R.id.tv);
            viewHolder = new ViewHolder(tv);
            convertView.setTag(viewHolder);
        }
        viewHolder = (ViewHolder) convertView.getTag();
        MusicContent musicContent = (MusicContent) getItem(position);
        viewHolder.textView.setText(musicContent.get_data());
        return convertView;
    }
}

class ViewHolder {
    TextView textView;

    public ViewHolder(TextView textView) {
        this.textView = textView;
    }
}

