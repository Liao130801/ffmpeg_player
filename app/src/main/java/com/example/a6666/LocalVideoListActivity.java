package com.example.a6666;


import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.util.List;

import androidx.appcompat.app.AppCompatActivity;

public class LocalVideoListActivity extends AppCompatActivity {
    private ListViewAdapter mAdapter;
    private TextView mTitle;
    private FileMgr fileMgr;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.list);
        //用于显示当前文本的路径
        mTitle=(TextView)findViewById(R.id.txt_view);
        //获取ListView控件
        ListView listView = (ListView) findViewById(R.id.list_view);
        //自定义的Adapter
        mAdapter = new ListViewAdapter(this);
        init();
        listView.setAdapter(mAdapter);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int pos, long id) {
                File file=mAdapter.getItem(pos);
                Bundle data=new Bundle();
                data.putString("url",file.getAbsolutePath());
                Intent intent =new Intent(LocalVideoListActivity.this,newActivity.class);
                intent.putExtras(data);
                startActivity(intent);
            }
        });
    }
    private void init() {
        fileMgr = new FileMgr();
        //判断外部存储是否可用
        if (!Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            Toast.makeText(this, "the external storage is not avaliable", Toast.LENGTH_SHORT).show();
            return;
        }
        File rootFile = new File("/storage/emulated/0/Pictures");
        mTitle.setText(rootFile.getAbsolutePath());
        //获取当前目录的子文件列表
        List<File> files = fileMgr.getSubFiles(rootFile);
        mAdapter.updateFiles(files);
    }

    private void change(File file) {
        if (!file.isDirectory()) {
            return;
        }
        //更新路径的显示
        mTitle.setText(file.getAbsolutePath());
        //获取新的文件列表
        List<File> files = fileMgr.getSubFiles(file);
        //更新文件和视图显示
        mAdapter.updateFiles(files);
        mAdapter.notifyDataSetChanged();
    }
}
