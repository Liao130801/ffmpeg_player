package com.example.a6666;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import com.bumptech.glide.Glide;

public class sActivity extends Activity {
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.s);

        Button b1=findViewById(R.id.jia);

        b1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                 playpcm();
            }
        });
        Button b2=findViewById(R.id.jiazai);
        ImageView img=findViewById(R.id.View);
        b2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Glide.with(getApplicationContext().getApplicationContext()).load("/storage/emulated/0/Pictures/icon_file.png").into(img);
            }
        });
    }
    static {
        System.loadLibrary("native-lib");
    }

    public native void playpcm();

}
