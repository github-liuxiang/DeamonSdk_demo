package polylink.sdk.pl;

import android.app.Activity;
import android.app.Application;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Build;

import polylink.sdk.pl.c.ISdkClient;
import polylink.sdk.pl.c.PLSdkClient;
import polylink.sdk.pl.c.SdkConfigurations;
import polylink.sdk.pl.c.onePixel.OnePixelReceiver;
import polylink.sdk.pl.c.receiver.PLReceiver;

/**
 * Created by Liuxiang on 2018/11/30.
 * 保活控制
 */

public class DeamonSdkCtrl {
    public static DeamonSdkCtrl deamonSdkCtrl;
    private Context context;
    private SdkConfigurations.SdkConfiguration configuration1;
    private SdkConfigurations.SdkConfiguration configuration2;
    private ISdkClient mDaemonClient;
    public static DeamonSdkCtrl getInstance() {
        if (deamonSdkCtrl == null) {
            synchronized (DeamonSdkCtrl.class) {
                if (deamonSdkCtrl == null)
                    deamonSdkCtrl = new DeamonSdkCtrl();
            }

        }
        return deamonSdkCtrl;
    }
    public void Init(Context context){
        if (context instanceof Activity)return;
        this.context=context;
        int sdk = Build.VERSION.SDK_INT;
        if (sdk>23)return;
        if (configuration1==null||configuration2==null)return ;
        SdkConfigurations.SdkListener listener = new MyDaemonListener();
        SdkConfigurations sdkConfigurations = new SdkConfigurations(configuration1, configuration2, listener);
        mDaemonClient = new PLSdkClient(sdkConfigurations);
        mDaemonClient.onAttachBaseContext(context);
    }
    public void start(Activity activity) {
        if (activity==null)return;
        int sdk = Build.VERSION.SDK_INT;
        if (sdk>23){
            if (configuration1==null||context==null)return ;
            Intent intent = new Intent();
            ComponentName component = new ComponentName(activity.getPackageName(), configuration1.SERVICE_NAME);
            intent.setComponent(component);
            activity.startService(intent);
            OnePixelReceiver  onePixelReceiver = new OnePixelReceiver();
            onePixelReceiver.registerReceiver(context);
        }else {
            if (configuration1==null)return ;
            Intent intent = new Intent();
            ComponentName component = new ComponentName(activity.getPackageName(), configuration1.SERVICE_NAME);
            intent.setComponent(component);
            activity.startService(intent);
        }
    }

    public DeamonSdkCtrl setProcess1(String Processname1, String ServiceCanonicalName1, String ReceiverCanonicalName1) {
        configuration1 = new SdkConfigurations.SdkConfiguration(
                Processname1,
                ServiceCanonicalName1,
                ReceiverCanonicalName1);
        return this;
    }

    public DeamonSdkCtrl setProcess2(String Processname2, String ServiceCanonicalName2, String ReceiverCanonicalName2) {
        configuration2 = new SdkConfigurations.SdkConfiguration(
                Processname2,
                ServiceCanonicalName2,
                ReceiverCanonicalName2);
        return this;
    }

    class MyDaemonListener implements SdkConfigurations.SdkListener{
        @Override
        public void onPersistentStart(Context context) {

        }

        @Override
        public void onDaemonAssistantStart(Context context) {
        }

        @Override
        public void onWatchDaemonDaed() {

        }
    }

    /**
     * [获取应用程序版本名称信息]
     * @param context
     * @return 当前应用的版本名称
     */
    public static synchronized String getPackageName(Context context) {
        try {
            PackageManager packageManager = context.getPackageManager();
            PackageInfo packageInfo = packageManager.getPackageInfo(
                    context.getPackageName(), 0);
            return packageInfo.packageName;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
}
