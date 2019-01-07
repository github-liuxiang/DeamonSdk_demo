package polylink.sdk.pl.c;

import android.content.Context;

public class NativeDosBase {
	/**
	 * used for native
	 */
	protected Context mContext;

    public NativeDosBase(Context context){
        this.mContext = context;
    }

    /**
     * native call back
     */
	protected void onDaemonDead(){
		ISdkStrategy.Fetcher.fetchStrategy().onDaemonDead();
    }

}
