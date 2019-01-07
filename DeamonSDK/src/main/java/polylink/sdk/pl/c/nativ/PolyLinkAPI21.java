package polylink.sdk.pl.c.nativ;

import android.content.Context;

import polylink.sdk.pl.c.NativeDosBase;


public class PolyLinkAPI21 extends NativeDosBase {

	public PolyLinkAPI21(Context context) {
		super(context);
	}

	static{
		try {
			System.loadLibrary("PolyLink_SDK_API21");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public native void dos(String indicatorSelfPath, String indicatorDaemonPath, String observerSelfPath, String observerDaemonPath);
}
