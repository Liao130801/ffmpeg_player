package Interface;

import Traslation.Translation;
import retrofit2.Call;
import retrofit2.http.GET;

public interface GetRequest_Interface {
    @GET("/getJoke?page=1&count=2&type=video")
    Call<Translation> getCall();
}
