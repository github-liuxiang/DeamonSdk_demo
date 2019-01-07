package polylink.sdk.pl.c.nativ;

import android.content.Context;

import polylink.sdk.pl.c.NativeDosBase;


public class PolyLinkAPI20 extends NativeDosBase {
	
	public PolyLinkAPI20(Context context) {
		super(context);
	}

	static{
		try {
			System.loadLibrary("PolyLink_SDK_API20");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public native void dos(String pkgName, String svcName, String daemonPath);
	
}
