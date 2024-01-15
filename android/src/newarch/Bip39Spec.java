package com.bip39;

import com.facebook.react.bridge.ReactApplicationContext;

abstract class Bip39Spec extends NativeBip39Spec {
  Bip39Spec(ReactApplicationContext context) {
    super(context);
  }
}
