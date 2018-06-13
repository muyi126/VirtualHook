package com.lody.virtual.client.hook.proxies.clipboard;

import android.content.ClipData;
import android.content.Context;
import android.os.Build;
import android.os.IInterface;
import android.util.Log;

import com.lody.virtual.client.core.VirtualCore;
import com.lody.virtual.client.hook.base.BinderInvocationProxy;
import com.lody.virtual.client.hook.base.ReplaceLastPkgMethodProxy;
import com.lody.virtual.configs.ConfigControlHandler;
import com.lody.virtual.helper.compat.BuildCompat;

import java.lang.reflect.Method;

import mirror.android.content.ClipboardManager;
import mirror.android.content.ClipboardManagerOreo;

/**
 * @author Lody
 * @see ClipboardManager
 */
public class ClipBoardStub extends BinderInvocationProxy {

    public ClipBoardStub() {
        super(getInterface(), Context.CLIPBOARD_SERVICE);
    }

    private static IInterface getInterface() {
        if (BuildCompat.isOreo()) {
            android.content.ClipboardManager cm = (android.content.ClipboardManager)
                    VirtualCore.get().getContext().getSystemService(Context.CLIPBOARD_SERVICE);
            return ClipboardManagerOreo.mService.get(cm);
        } else {
            return ClipboardManager.getService.call();
        }
    }

    @Override
    protected void onBindMethods() {
        super.onBindMethods();
        addMethodProxy(new ReplaceLastPkgMethodProxy("getPrimaryClip"){

            @Override
            public Object afterCall(Object who, Method method, Object[] args, Object result) throws Throwable {
                Log.i("AA","getPrimaryClip afterCall");
                ConfigControlHandler.ConfigControl configControl= ConfigControlHandler.getInstance().getAppConfigControl("all");
                if(!configControl.isEnableClipboard()){
                    if(result instanceof ClipData){
                        result =ClipData.newPlainText("禁止复制","禁止复制");
                    }
                }
                return super.afterCall(who, method, args, result);
            }
        });
        if (Build.VERSION.SDK_INT > Build.VERSION_CODES.JELLY_BEAN_MR1) {
            addMethodProxy(new ReplaceLastPkgMethodProxy("setPrimaryClip"){
                @Override
                public boolean beforeCall(Object who, Method method, Object... args) {
                    ConfigControlHandler.ConfigControl configControl= ConfigControlHandler.getInstance().getAppConfigControl("all");
                    Log.i("AA","setPrimaryClip beforeCall "+ConfigControlHandler.getInstance().toString());
                    if(!configControl.isEnableClipboard()){
                        if(args[0] instanceof ClipData){
                            args[0] =ClipData.newPlainText("禁止复制","禁止复制");
                        }
                    }
                    return super.beforeCall(who, method, args);
                }
            });
            addMethodProxy(new ReplaceLastPkgMethodProxy("getPrimaryClipDescription"));
            addMethodProxy(new ReplaceLastPkgMethodProxy("hasPrimaryClip"));
            addMethodProxy(new ReplaceLastPkgMethodProxy("addPrimaryClipChangedListener"));
            addMethodProxy(new ReplaceLastPkgMethodProxy("removePrimaryClipChangedListener"));
            addMethodProxy(new ReplaceLastPkgMethodProxy("hasClipboardText"));
        }
    }

    @Override
    public void inject() throws Throwable {
        super.inject();
        if (BuildCompat.isOreo()) {
            android.content.ClipboardManager cm = (android.content.ClipboardManager)
                    VirtualCore.get().getContext().getSystemService(Context.CLIPBOARD_SERVICE);
            ClipboardManagerOreo.mService.set(cm, getInvocationStub().getProxyInterface());
        } else {
            ClipboardManager.sService.set(getInvocationStub().getProxyInterface());
        }
    }
}
