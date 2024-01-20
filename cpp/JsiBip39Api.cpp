#include "JsiBip39Api.h"

#include <jsi/jsi.h>

#include <memory>
#include <string>
#include <vector>

namespace RNBip39 {

namespace jsi = facebook::jsi;

const char* JsiBip39Api::Bip39Name = "bip39";
std::shared_ptr<JsiBip39Api> JsiBip39Api::instance;

/**
 * Installs the worklet API into the provided runtime
 */
void JsiBip39Api::installApi(jsi::Runtime& runtime) {
  auto existingApi = (runtime.global().getProperty(runtime, Bip39Name));
  if (existingApi.isObject()) {
    return;
  }
  runtime.global().setProperty(runtime, Bip39Name, jsi::Object::createFromHostObject(runtime, getInstance()));

  // Adds the PropNameIDCache object to the Runtime. If the Runtime gets
  // destroyed, the Object gets
  // destroyed and the cache gets invalidated.
  auto propNameIdCache = std::make_shared<InvalidateCacheOnDestroy>(runtime);
  runtime.global().setProperty(runtime, "bip39PropNameIdCache", jsi::Object::createFromHostObject(runtime, propNameIdCache));
}

std::shared_ptr<JsiBip39Api> JsiBip39Api::getInstance() {
  if (instance == nullptr) {
    instance = std::make_shared<JsiBip39Api>();
  }
  return instance;
}

/**
 Invalidate the api instance.
 */
void JsiBip39Api::invalidateInstance() {
  instance = nullptr;
}

} // namespace RNBip39
