package com.example.a6666;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.TextView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import Interface.GetRequest_Interface;
import Traslation.Translation;
import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;


public class MainActivity extends Activity {

    private static final String TAG = "FFMPEG";




    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);



        setContentView(R.layout.activity_main);
        Button m=findViewById(R.id.dong);
        Animation t= AnimationUtils.loadAnimation(this,R.anim.view_animation);
        m.startAnimation(t);

        TextView tv=findViewById(R.id.shipin);
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl("https://api.apiopen.top") // 设置 网络请求 Url
                .addConverterFactory(GsonConverterFactory.create()) //设置使用Gson解析(记得加入依赖)
                .build();

        // 步骤5:创建 网络请求接口 的实例
        GetRequest_Interface request =  retrofit.create(GetRequest_Interface.class);

        //对 发送请求 进行封装(设置需要翻译的内容)
        retrofit2.Call<Translation> call = request.getCall();

        //步骤6:发送网络请求(异步)
        call.enqueue(new retrofit2.Callback<Translation>() {

            //请求成功时回调
            @Override
            public void onResponse(retrofit2.Call<Translation> call, retrofit2.Response<Translation> response) {


                tv.setText(response.body().toString());


            }

            //请求失败时回调
            @Override
            public void onFailure(retrofit2.Call<Translation> call, Throwable throwable) {
                System.out.println("请求失败");
                System.out.println(throwable.getMessage());
            }
        });
    /**
     * 停止播放
     * @param view
     */}

    public void tiao(View view) {
        Intent intent=new Intent(MainActivity.this,newActivity.class);
        startActivity(intent);
    }
    public void tiaovideo(View view) {
        Intent intent=new Intent(MainActivity.this,LocalVideoListActivity.class);
        startActivity(intent);
    }
    public void down(View view) {
        String url = "http://clips.vorwaerts-gmbh.de/big_buck_bunny.mp4";

        final String fileName = url.split("/")[url.split("/").length - 1];
        OkHttpClient client = new OkHttpClient();
        final Request request = new Request.Builder()
                .get()
                .url(url)
                .build();
        Call call = client.newCall(request);
        call.enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                Log.e("moer", "onFailure: ");;
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                //  String dirName = Environment.getExternalStorageDirectory().getAbsolutePath() + "/LungFile";
                String dirName = "/storage/emulated/0/Pictures/Screenshots";
                File file=new File(dirName);
                if (!file.exists()) {
                    file.mkdir();
                }
                if (response != null) {
                    InputStream is = response.body().byteStream();
                    FileOutputStream fos = new FileOutputStream(new File(dirName + "/" + fileName));
                    int len = 0;
                    byte[] buffer = new byte[2048];
                    while (-1 != (len = is.read(buffer))) {
                        fos.write(buffer, 0, len);
                    }
                    fos.flush();
                    fos.close();
                    is.close();
                }
            }
        });

    }
}
