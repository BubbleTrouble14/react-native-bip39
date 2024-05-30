#pragma once

#include <RNBip39SpecJSI.h>
#include <memory>

namespace facebook::react {

// The TurboModule itself
class NativeBip39Module : public NativeBip39CxxSpec<NativeBip39Module> {
public:
  explicit NativeBip39Module(std::shared_ptr<CallInvoker> jsInvoker);
  ~NativeBip39Module();

  jsi::Object createBip39Api(jsi::Runtime& runtime);

private:
  std::shared_ptr<CallInvoker> _jsCallInvoker;
};

} // namespace facebook::react
