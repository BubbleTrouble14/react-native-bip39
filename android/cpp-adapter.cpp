#include <ReactCommon/CallInvokerHolder.h>
#include <fbjni/fbjni.h>
#include <jni.h>
#include <jsi/jsi.h>

#include "NativeBip39Module.h"
#include <ReactCommon/CxxTurboModuleUtils.h>

using namespace facebook;



JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *) {
  facebook::react::registerCxxModuleToGlobalModuleMap(
                                                        std::string(facebook::react::NativeBip39Module::kModuleName),
                                                        [&](std::shared_ptr<facebook::react::CallInvoker> jsInvoker) {
                                                            return std::make_shared<facebook::react::NativeBip39Module>(jsInvoker);
                                                        });
  return JNI_VERSION_1_6;
}
