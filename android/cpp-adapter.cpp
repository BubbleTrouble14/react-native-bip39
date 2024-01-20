#include <jni.h>

#include <ReactCommon/CallInvokerHolder.h>
#include <jsi/jsi.h>

#include "JsiBip39Api.h"
#include "TypedArray.h"
#include "bit_opts.h"
#include "langs.h"
#include "mnemonic.h"
#include "random.h"
#include "toolbox.h"

using namespace facebook; // NOLINT

extern "C" JNIEXPORT jboolean JNICALL Java_com_bip39_Bip39Module_nativeInstall(
    JNIEnv *env, jclass obj, jlong jsiRuntimeRef, jobject jsCallInvokerHolder)
{
  auto jsiRuntime{reinterpret_cast<jsi::Runtime *>(jsiRuntimeRef)};
  auto jsCallInvoker{jni::alias_ref<react::CallInvokerHolder::javaobject>{
      reinterpret_cast<react::CallInvokerHolder::javaobject>(
          jsCallInvokerHolder)} -> cthis()
                         ->getCallInvoker()};

  // Install the bip39 API
  RNBip39::JsiBip39Api::installApi(*jsiRuntime);

  return true;
}
