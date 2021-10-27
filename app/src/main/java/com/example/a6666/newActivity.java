package com.example.a6666;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;
import viewModel.requestMdel;

public class newActivity extends AppCompatActivity {

    private DZVideoView mVideoView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.v);

        mVideoView = findViewById(R.id.video_view);
        ViewModelProvider.Factory factory=ViewModelProvider.AndroidViewModelFactory.getInstance(getApplication());
        requestMdel model=factory.create(requestMdel.class);
        model.request();
        model.getUrl().observe(this, new Observer<String>() {
            @Override
            public void onChanged(String s) {
                mVideoView.play(model.getUrl().getValue());
            }
        });
          mVideoView.play("http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4");

    }




}
