#include "TestObject.h"

#include <jsi/jsi.h>

#include <memory>
#include <string>
#include <vector>

namespace RNTest
{

  namespace jsi = facebook::jsi;

  const char *JsiTestApi::TestApiName = "TestApi";
  std::shared_ptr<JsiTestApi> JsiTestApi::instance;

  /**
   * Installs the worklet API into the provided runtime
   */
  void JsiTestApi::installApi(jsi::Runtime &runtime)
  {
    auto existingApi = (runtime.global().getProperty(runtime, TestApiName));
    if (existingApi.isObject())
    {
      return;
    }
    runtime.global().setProperty(
        runtime, TestApiName,
        jsi::Object::createFromHostObject(runtime, getInstance()));
  }

  std::shared_ptr<JsiTestApi> JsiTestApi::getInstance()
  {
    if (instance == nullptr)
    {
      instance = std::make_shared<JsiTestApi>();
    }
    return instance;
  }

  /**
   Invalidate the api instance.
   */
  void JsiTestApi::invalidateInstance() { instance = nullptr; }

} // namespace RNWorklet
