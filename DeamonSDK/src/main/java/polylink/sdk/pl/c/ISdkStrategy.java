package polylink.sdk.pl.c;

import android.content.Context;
import android.os.Build;

import polylink.sdk.pl.c.strategy.SdkStrategy21;
import polylink.sdk.pl.c.strategy.SdkStrategy22;
import polylink.sdk.pl.c.strategy.SdkStrategy23;
import polylink.sdk.pl.c.strategy.SdkStrategyUnder21;
import polylink.sdk.pl.c.strategy.SdkStrategyXiaomi;


public interface ISdkStrategy {

	boolean onInitialization(Context context);


	void onPersistentCreate(Context context, SdkConfigurations configs);


	void onDaemonAssistantCreate(Context context, SdkConfigurations configs);


	void onDaemonDead();




	public static class Fetcher {

		private static ISdkStrategy mDaemonStrategy;


		static ISdkStrategy fetchStrategy() {
			if (mDaemonStrategy != null) {
				return mDaemonStrategy;
			}
			int sdk = Build.VERSION.SDK_INT;
			switch (sdk) {
				case 23:
					mDaemonStrategy = new SdkStrategy23();
					break;

				case 22:
					mDaemonStrategy = new SdkStrategy22();
					break;

				case 21:
					if("MX4 Pro".equalsIgnoreCase(Build.MODEL)){
						mDaemonStrategy = new SdkStrategyUnder21();
					}else{
						mDaemonStrategy = new SdkStrategy21();
					}
					break;
//				case 24:
//				case 25:
//				case 26:{
//					mDaemonStrategy = new SdkStrategy23();
//				}
//				break;
				default:
					if(Build.MODEL != null && Build.MODEL.toLowerCase().startsWith("mi")){
						mDaemonStrategy = new SdkStrategyXiaomi();
					}else if(Build.MODEL != null && Build.MODEL.toLowerCase().startsWith("a31")){
						mDaemonStrategy = new SdkStrategy21();
					}else{
						mDaemonStrategy = new SdkStrategyUnder21();
					}
					break;
			}
			return mDaemonStrategy;
		}
	}
}
