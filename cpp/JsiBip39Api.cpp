#include "JsiBip39Api.h"

#include <jsi/jsi.h>

#include <memory>
#include <string>
#include <vector>

namespace RNBip39 {

namespace jsi = facebook::jsi;

void JsiBip39Api::installApi(jsi::Runtime& runtime) {
  auto bip39Api = std::make_shared<JsiBip39Api>(runtime);
  runtime.global().setProperty(runtime, "bip39", jsi::Object::createFromHostObject(runtime, std::move(bip39Api)));

  // Adds the PropNameIDCache object to the Runtime. If the Runtime gets
  auto propNameIdCache = std::make_shared<InvalidateCacheOnDestroy>(runtime);
  runtime.global().setProperty(runtime, "bip39PropNameIdCache", jsi::Object::createFromHostObject(runtime, propNameIdCache));
}

} // namespace RNBip39
