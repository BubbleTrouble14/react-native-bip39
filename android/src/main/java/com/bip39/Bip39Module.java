package com.bip39;

import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.module.annotations.ReactModule;
import com.facebook.react.turbomodule.core.CallInvokerHolderImpl;
import com.facebook.react.turbomodule.core.interfaces.CallInvokerHolder;

import java.lang.ref.WeakReference;

@ReactModule(name = Bip39Module.NAME)
public class Bip39Module extends Bip39Spec {
  public static final String NAME = "Bip39";
  private final WeakReference<ReactApplicationContext> weakReactContext;

  Bip39Module(ReactApplicationContext context) {
    super(context);
    this.weakReactContext = new WeakReference<>(context);
  }

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  static {
    System.loadLibrary("rnbip39");
  }

  public static native boolean nativeInstall(long jsiRuntimeRef, CallInvokerHolder jsCallInvokerHolder);

  @ReactMethod(isBlockingSynchronousMethod = true)
  public boolean install() {
    try {
      ReactApplicationContext context = weakReactContext.get();
      if (context == null) {
        Log.e(NAME, "React Application Context was null!");
        return false;
      }

      long jsiRuntimeRef = context.getJavaScriptContextHolder().get();
      CallInvokerHolder jsCallInvokerHolder = context.getCatalystInstance().getJSCallInvokerHolder();
      return nativeInstall(jsiRuntimeRef, jsCallInvokerHolder);
    } catch (Exception exception) {
      Log.e(NAME, "Failed to initialize react-native-worklets-core!", exception);
      return false;
    }
  }
}
