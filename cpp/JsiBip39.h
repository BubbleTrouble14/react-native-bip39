#pragma once

#include <jsi/jsi.h>

#include <memory>
#include <string>
#include <vector>

#include "bit_opts.h"
#include "byteswap.h"
#include "langs.h"
#include "mnemonic.h"
#include "random.h"
#include "sha256.h"
#include "toolbox.h"

namespace jsi = facebook::jsi;

class JsiBip39 : public jsi::HostObject {
 public:
  JsiBip39();
  ~JsiBip39();

  jsi::Value get(jsi::Runtime &, const jsi::PropNameID &name) override;
  std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime &rt) override;
};
