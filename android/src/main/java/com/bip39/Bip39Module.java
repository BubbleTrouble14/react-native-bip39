package com.bip39;

import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.jni.HybridData;
import com.facebook.proguard.annotations.DoNotStrip;
import com.facebook.react.bridge.JavaScriptContextHolder;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.module.annotations.ReactModule;
import com.facebook.react.turbomodule.core.CallInvokerHolderImpl;

@ReactModule(name = Bip39Module.NAME)
public class Bip39Module extends ReactContextBaseJavaModule {
  public static final String NAME = "Bip39";

  public Bip39Module(ReactApplicationContext reactContext) {
    super(reactContext);
  }

  @NonNull
  @Override
  public String getName() {
    return NAME;
  }


}
