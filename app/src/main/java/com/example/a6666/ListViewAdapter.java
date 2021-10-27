package com.example.a6666;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class ListViewAdapter extends BaseAdapter {
    private LayoutInflater flater;
    private List<File>mDatas;

    public ListViewAdapter(Context context) {
        flater = LayoutInflater.from(context);
        mDatas = new ArrayList<File>();
    }

    public void updateFiles(List<File> files) {
        mDatas.clear();
        mDatas.addAll(files);
    }

    public int getCount() {
        return (null == mDatas || mDatas.isEmpty()) ? 0 : mDatas.size();
    }

    @Override
    public File getItem(int pos) {
        if (null != mDatas && pos < mDatas.size()) {
            return mDatas.get(pos);
        }
        return null;
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    @Override
    public View getView(int position,View convertView, ViewGroup parent) {
        if (null == convertView) {
            //为每一个子项加载布局
            convertView = flater.inflate(R.layout.view_list_item, null);
        }
        //获取 item 布局文件中的控件
        ImageView imageView = (ImageView) convertView.findViewById(R.id.lvew_img);
        TextView txtTitle = (TextView) convertView.findViewById(R.id.lview_title);

        File file = getItem(position);//根据position获取数据
        if (null!=file) {
            imageView.setImageResource(file.isDirectory() ? R.drawable.avatar_01 : R.drawable.ic_launcher_background);
            txtTitle.setText(file.getName());
        }
        return convertView;
    }
}
