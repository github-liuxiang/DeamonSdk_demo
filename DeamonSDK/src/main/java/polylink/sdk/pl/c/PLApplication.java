package polylink.sdk.pl.c;

import android.app.Application;
import android.content.Context;


public abstract class PLApplication extends Application {

	protected abstract SdkConfigurations getSdkConfigurations();



	private ISdkClient mDaemonClient;
	public PLApplication(){
		mDaemonClient = new PLSdkClient(getSdkConfigurations());
	}



	private boolean mHasAttachBaseContext = false;

	@Override
	public final void attachBaseContext(Context base) {
		if(mHasAttachBaseContext){
			return ;
		}
		mHasAttachBaseContext = true;
		super.attachBaseContext(base);
		mDaemonClient.onAttachBaseContext(base);
		attachBaseContextByDaemon(base);
	}


	public void attachBaseContextByDaemon(Context base){

	}


}
