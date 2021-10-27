package viewModel;

import Interface.GetRequest_Interface;
import Traslation.Translation;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class requestMdel extends ViewModel {
    private MutableLiveData<String> url;
    public MutableLiveData<String> getUrl()
    {
        if(url==null)url=new MutableLiveData<>();
        return url;
    }
    public String u()
    {
        return url.getValue();
    }
    public void request() {

        //步骤4:创建Retrofit对象
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl("https://api.apiopen.top/") // 设置 网络请求 Url
                .addConverterFactory(GsonConverterFactory.create()) //设置使用Gson解析(记得加入依赖)
                .build();

        // 步骤5:创建 网络请求接口 的实例
        GetRequest_Interface request =  retrofit.create(GetRequest_Interface.class);

        //对 发送请求 进行封装(设置需要翻译的内容)
        Call<Translation> call = request.getCall();

        //步骤6:发送网络请求(异步)
        call.enqueue(new Callback<Translation>() {

            //请求成功时回调
            @Override
            public void onResponse(Call<Translation> call, Response<Translation> response) {


                   url.setValue(response.body().getResult().get(0).getVideo());


            }

            //请求失败时回调
            @Override
            public void onFailure(Call<Translation> call, Throwable throwable) {
                System.out.println("请求失败");
                System.out.println(throwable.getMessage());
            }
        });
    }
    protected void onCleared()
    {
        super.onCleared();
        url=null;
    }
}
