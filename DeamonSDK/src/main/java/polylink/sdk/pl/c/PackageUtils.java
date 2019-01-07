package polylink.sdk.pl.c;

import android.content.ComponentName;
import android.content.Context;
import android.content.pm.PackageManager;

public class PackageUtils {

	public static void setComponentDefault(Context context, String componentClassName){
		PackageManager pm = context.getPackageManager();
		ComponentName componentName = new ComponentName(context.getPackageName(), componentClassName);
		pm.setComponentEnabledSetting(componentName, PackageManager.COMPONENT_ENABLED_STATE_DEFAULT, PackageManager.DONT_KILL_APP);
	}


	public static boolean isComponentDefault(Context context, String componentClassName){
		PackageManager pm = context.getPackageManager();
		ComponentName componentName = new ComponentName(context.getPackageName(), componentClassName);
		return pm.getComponentEnabledSetting(componentName) == PackageManager.COMPONENT_ENABLED_STATE_DEFAULT;
	}
}
