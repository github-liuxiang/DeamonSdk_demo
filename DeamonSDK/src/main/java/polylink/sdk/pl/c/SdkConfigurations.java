package polylink.sdk.pl.c;

import android.content.Context;


public class SdkConfigurations {

	public final SdkConfiguration 	PERSISTENT_CONFIG;
	public final SdkConfiguration 	DAEMON_ASSISTANT_CONFIG;
	public final SdkListener		LISTENER;

	public SdkConfigurations(SdkConfiguration persistentConfig, SdkConfiguration sdkConfiguration){
		this.PERSISTENT_CONFIG = persistentConfig;
		this.DAEMON_ASSISTANT_CONFIG = sdkConfiguration;
		this.LISTENER = null;
	}

	public SdkConfigurations(SdkConfiguration sdkConfiguration1, SdkConfiguration sdkConfiguration, SdkListener listener){
		this.PERSISTENT_CONFIG = sdkConfiguration1;
		this.DAEMON_ASSISTANT_CONFIG = sdkConfiguration;
		this.LISTENER = listener;
	}



	public static class SdkConfiguration{

		public final String PROCESS_NAME;
		public final String SERVICE_NAME;
		public final String RECEIVER_NAME;

		public SdkConfiguration(String processName, String serviceName, String receiverName){
			this.PROCESS_NAME = processName;
			this.SERVICE_NAME = serviceName;
			this.RECEIVER_NAME = receiverName;
		}
	}

	public interface SdkListener {
		void onPersistentStart(Context context);
		void onDaemonAssistantStart(Context context);
		void onWatchDaemonDaed();
	}
}
