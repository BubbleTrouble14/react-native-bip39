#import "Bip39.h"

#import "TypedArray.h"
#import <React/RCTBridge+Private.h>
#import <React/RCTBridge.h>
#import <React/RCTUtils.h>
#import <ReactCommon/RCTTurboModule.h>

@implementation Bip39

@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()

- (void)setBridge:(RCTBridge *)bridge {
  _bridge = bridge;
}

void installApi(std::shared_ptr<facebook::react::CallInvoker> callInvoker,
                facebook::jsi::Runtime &jsiRuntime) {

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
  // destroyed, the Object gets destroyed and the cache gets invalidated.
  auto propNameIdCache = std::make_shared<InvalidateCacheOnDestroy>(jsiRuntime);
  jsiRuntime.global().setProperty(
      jsiRuntime, "bip39PropNameIdCache",
      jsi::Object::createFromHostObject(jsiRuntime, propNameIdCache));
}

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install) {
  RCTCxxBridge *cxxBridge = (RCTCxxBridge *)_bridge;
  if (cxxBridge.runtime != nullptr) {
    auto callInvoker = cxxBridge.jsCallInvoker;
    facebook::jsi::Runtime *jsRuntime =
        (facebook::jsi::Runtime *)cxxBridge.runtime;

    auto &runtime = *jsRuntime;

    installApi(callInvoker, runtime);
    return @true;
  }
  return @false;
}

// Don't compile this code when we build for the old architecture.
#ifdef RCT_NEW_ARCH_ENABLED
- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params {
  // Call installApi with c++ brigde or something similar.
  RCTCxxBridge *cxxBridge = (RCTCxxBridge *)_bridge;
  auto callInvoker = cxxBridge.jsCallInvoker;
  facebook::jsi::Runtime *jsRuntime =
      (facebook::jsi::Runtime *)cxxBridge.runtime;

  auto &runtime = *jsRuntime;

  installApi(callInvoker, runtime);

  return std::make_shared<facebook::react::NativeBip39SpecJSI>(params);
}
#endif

@end
