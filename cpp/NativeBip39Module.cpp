//
//  NativeBip39Module.cpp
//  react-native-worklets-core
//
//  Created by Marc Rousavy on 15.05.24.
//

#include "NativeBip39Module.h"
#include "JsiBip39Api.h"
#include <memory>
#include <utility>

namespace facebook::react {

NativeBip39Module::NativeBip39Module(std::shared_ptr<CallInvoker> jsInvoker) : NativeBip39CxxSpec(jsInvoker), _jsCallInvoker(jsInvoker) {}

NativeBip39Module::~NativeBip39Module() {}

jsi::Object NativeBip39Module::createBip39Api(jsi::Runtime& runtime) {
  // Create new instance of the Worklets API (JSI module)
  auto bip39 = std::make_shared<RNBip39::JsiBip39Api>();

  return jsi::Object::createFromHostObject(runtime, bip39);
}

} // namespace facebook::react
