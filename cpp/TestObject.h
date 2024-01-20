#pragma once

#include <chrono>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include <jsi/jsi.h>

#include "TypedArray.h"
#include "JsiHostObject.h"

#include "bit_opts.h"
#include "langs.h"
#include "mnemonic.h"
#include "random.h"
#include "toolbox.h"
namespace RNTest
{

  namespace jsi = facebook::jsi;

  class JsiTestApi : public RNJsi::JsiHostObject
  {
  public:
    static const char *TestApiName;
    static void installApi(jsi::Runtime &runtime);
    static void invalidateInstance();
    static std::shared_ptr<JsiTestApi> getInstance();

    JSI_HOST_FUNCTION(setDefaultWordlist)
    {
      if (count == 0)
      {
        throw jsi::JSError(
            runtime, "setDefaultWordlist expects the wordlist name parameter.");
      }

      if (!arguments[0].isString())
      {
        throw jsi::JSError(runtime,
                           "setDefaultWordlist expects the wordlist name "
                           "parameter as a string.");
      }

      wordlist = arguments[0].asString(runtime).utf8(runtime);
      return jsi::Value::undefined();
    };

    JSI_HOST_FUNCTION(getDefaultWordlist)
    {
      return jsi::String::createFromUtf8(runtime, wordlist);
    };

    JSI_HOST_FUNCTION(mnemonicToSeed)
    {
      // Check if the first argument (mnemonic) is provided and is a string
      if (count < 1 || !arguments[0].isString())
      {
        throw jsi::JSError(
            runtime,
            "First argument must be a string representing a mnemonic!");
      }
      std::string mnemonics = arguments[0].asString(runtime).utf8(runtime);

      // Initialize password as an empty string
      std::string password = "";

      // Check and set the language if provided
      if (count > 1)
      {
        // Check if the second argument is not undefined/null
        if (!arguments[1].isNull() && !arguments[1].isUndefined())
        {
          // Check if the second
          if (!arguments[1].isString())
          {
            throw jsi::JSError(runtime, "Second argument must be a string "
                                        "representing the password!");
          }

          password = arguments[1].asString(runtime).utf8(runtime);
        }
      }

      // Generate the seed using the mnemonic and password
      auto seed = bip39::Mnemonic::CreateSeed(mnemonics, password);

      auto byteArray =
          TypedArray<TypedArrayKind::Uint8Array>(runtime, seed.size());
      auto arrayBuffer = byteArray.getBuffer(runtime);

      memcpy(arrayBuffer.data(runtime), seed.data(), seed.size());

      return byteArray;
    };

    // JSI_PROPERTY_GET(getDefaultWordlist) { return jsi::String::createFromUtf8(runtime, wordlist); }

    // JSI_EXPORT_PROPERTY_GETTERS(JSI_EXPORT_PROP_GET(JsiTestApi, getDefaultWordlist))
    JSI_EXPORT_FUNCTIONS(JSI_EXPORT_FUNC(JsiTestApi, setDefaultWordlist),
                         JSI_EXPORT_FUNC(JsiTestApi, getDefaultWordlist),
                         JSI_EXPORT_FUNC(JsiTestApi, mnemonicToSeed))

    JsiTestApi() : JsiHostObject(), wordlist("english") {}

  private:
    std::string wordlist;
    static std::shared_ptr<JsiTestApi> instance;
  };
} // namespace RNSkia
