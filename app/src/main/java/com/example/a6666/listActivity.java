package com.example.a6666;

import android.app.Activity;
import android.content.ContentResolver;
import android.database.Cursor;
import android.os.Bundle;
import android.provider.MediaStore;
import android.widget.ListView;

import java.util.ArrayList;


public class listActivity extends Activity
{

    ContentResolver resolver;
    ArrayList<MusicContent> arrayList;
    ListView lv;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.list);
        lv = (ListView) findViewById(R.id.list_view);
        arrayList = new ArrayList<>();
        resolver = getContentResolver();

        /**
         * 参数一：Uri uri
         * 参数二：String[] projection  列名
         * 参数三：String selection 查询条件
         * 参数四：String[] selectionArgs  查询条件的值
         * 参数五：String sortOrder 排序
         */
        Cursor cursor = resolver.query(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI, null, null, null, null);
        if (cursor != null) {
            while (cursor.moveToNext()) {
                String _date = cursor.getString(cursor.getColumnIndex("_data"));
                String _display_name = cursor.getString(cursor.getColumnIndex("_display_name"));
                String mime_type = cursor.getString(cursor.getColumnIndex("mime_type"));
                String title = cursor.getString(cursor.getColumnIndex("title"));
                String artist = cursor.getString(cursor.getColumnIndex("artist"));
                String album = cursor.getString(cursor.getColumnIndex("album"));
                long _size = cursor.getInt(cursor.getColumnIndex("_data"));
                long date_added = cursor.getInt(cursor.getColumnIndex("_data"));
                long duration = cursor.getInt(cursor.getColumnIndex("_data"));
                MusicContent musicContent = new MusicContent(_date, _display_name, mime_type, title, artist, album, _size, date_added, duration);
                arrayList.add(musicContent);
            }
        }
        MusicAdapter musicAdapter = new MusicAdapter(this, arrayList);
        lv.setAdapter(musicAdapter);
    }


}
