package polylink.sdk.pl.c.strategy;

import android.content.Context;

import polylink.sdk.pl.c.ISdkStrategy;
import polylink.sdk.pl.c.SdkConfigurations;

/**
 * Created by Liuxiang on 2018/11/30.
 */

public class SdkStrategyUnder  implements ISdkStrategy {
    @Override
    public boolean onInitialization(Context context) {
        return false;
    }

    @Override
    public void onPersistentCreate(Context context, SdkConfigurations configs) {

    }

    @Override
    public void onDaemonAssistantCreate(Context context, SdkConfigurations configs) {

    }

    @Override
    public void onDaemonDead() {

    }
}
