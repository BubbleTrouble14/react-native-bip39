#include <jni.h>

#include <ReactCommon/CallInvokerHolder.h>
#include <jsi/jsi.h>

#include "JsiBip39.h"
#include "bit_opts.h"
#include "langs.h"
#include "mnemonic.h"
#include "random.h"
#include "toolbox.h"
#include "TypedArray.h"

using namespace facebook; // NOLINT

void install(jsi::Runtime &jsiRuntime) {
  auto createBip39Instance = jsi::Function::createFromHostFunction(
      jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "createBip39Instance"),
      0,
      [](jsi::Runtime &runtime, const jsi::Value &thisValue,
         const jsi::Value *arguments, size_t count) -> jsi::Value {
        if (count != 0) {
          throw jsi::JSError(
              runtime, "Bip39.createNewInstance(..) expects 0 arguments!");
        }

        auto instance = std::make_shared<JsiBip39>();
        return jsi::Object::createFromHostObject(runtime, instance);
      });

  jsiRuntime.global().setProperty(jsiRuntime, "createBip39Instance",
                                  std::move(createBip39Instance));

  // Adds the PropNameIDCache object to the Runtime. If the Runtime gets
  // destroyed, the Object gets
  // destroyed and the cache gets invalidated.
  auto propNameIdCache = std::make_shared<InvalidateCacheOnDestroy>(jsiRuntime);
  jsiRuntime.global().setProperty(
      jsiRuntime, "bip39PropNameIdCache",
      jsi::Object::createFromHostObject(jsiRuntime, propNameIdCache));
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_bip39_Bip39Module_nativeInstall(
    JNIEnv *env, jclass obj, jlong jsiRuntimeRef, jobject jsCallInvokerHolder) {
  auto jsiRuntime{reinterpret_cast<jsi::Runtime *>(jsiRuntimeRef)};
  auto jsCallInvoker{jni::alias_ref<react::CallInvokerHolder::javaobject>{
      reinterpret_cast<react::CallInvokerHolder::javaobject>(
          jsCallInvokerHolder)} -> cthis()
                         ->getCallInvoker()};

  install(*jsiRuntime);

  return true;
}
