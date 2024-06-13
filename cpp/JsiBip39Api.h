#pragma once

#include <jsi/jsi.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string_view>
#include <utility>
#include <vector>

#include "Bip39MutableBuffer.h"
#include "Bip39Utils.h"
#include "JsiHostObject.h"
#include "bit_opts.h"
#include "langs.h"
#include "mnemonic.h"
#include "random.h"
#include "toolbox.h"
namespace RNBip39 {

namespace jsi = facebook::jsi;
class JsiBip39Api : public RNJsi::JsiHostObject {
public:
  explicit JsiBip39Api() : _wordlist(std::make_shared<std::string>("english")) {}

  static void installApi(jsi::Runtime& runtime);

  JSI_HOST_FUNCTION(getDefaultWordlist) {
    return jsi::String::createFromUtf8(runtime, *_wordlist);
  };

  JSI_HOST_FUNCTION(setDefaultWordlist) {
    if (count == 0) {
      throw jsi::JSError(runtime, "setDefaultWordlist expects the wordlist name parameter.");
    }

    if (!arguments[0].isString()) {
      throw jsi::JSError(runtime, "setDefaultWordlist expects the wordlist name parameter as a string.");
    }

    std::string lang = arguments[0].asString(runtime).utf8(runtime);

    // auto it = bip39::langs.find(lang);
    if (!bip39::utils::LangExists(lang)) {
      throw jsi::JSError(runtime, "Invalid lang name");
    }

    *_wordlist = lang;
    return jsi::Value::undefined();
  };
  // Generate mnemonic
  JSI_HOST_FUNCTION(generateMnemonic) {
    // Default values
    int wordCount = 12; // Default entropy size
    std::vector<uint8_t> ent;
    std::string wordlist = *_wordlist;

    // Check and set the entropy size if provided
    if (count > 0 && arguments[0].isNumber()) {
      wordCount = arguments[0].asNumber();
    }

    // Check and set rng data if provided
    if (count > 1 && !arguments[1].isNull() && !arguments[1].isUndefined()) {
      // auto object = arguments[1].asObject(runtime);
      // if (!isTypedArray(runtime, object)) {
      //   throw jsi::JSError(runtime, "Second argument must be a Uint8Array representing rng!");
      // }
      // auto typedArray = getTypedArray(runtime, object);
      auto buffer = arguments[1].asObject(runtime).getArrayBuffer(runtime);

      ent = ArrayBufferToVector(buffer, runtime);
    } else {
      // generate entropy
      int bits_ent = bip39::Mnemonic::GetEntBitsByNumMnemonicSentences(wordCount);
      int num_bytes = bits_ent / 8;
      bip39::RandomBytes rnd(num_bytes);
      ent = rnd.Random();
    }

    // Check and set the wordlist if provided
    if (count > 2 && !arguments[2].isNull() && !arguments[2].isUndefined()) {
      if (!arguments[2].isString()) {
        throw jsi::JSError(runtime, "Third argument must be a string representing the wordlist!");
      }
      wordlist = arguments[2].asString(runtime).utf8(runtime);
    }

    if (!bip39::Mnemonic::IsValidNumMnemonicSentences(wordCount)) {
      throw std::runtime_error("invalid number of mnemonic sentences");
    }

    // create mnemonic
    bip39::Mnemonic mnemonic(ent);
    auto word_list = mnemonic.GetWordList(std::string(wordlist));
    std::string mnemonic_sentences = bip39::GenerateWords(word_list, bip39::GetDelimiterByLang(wordlist));

    return jsi::String::createFromUtf8(runtime, mnemonic_sentences);
  };

  // Validate mnemonic
  JSI_HOST_FUNCTION(validateMnemonic) {
    std::string wordlist = *_wordlist;

    if (count > 0) {
      if (!arguments[0].isString()) {
        throw jsi::JSError(runtime, "First argument must be a string representing a mnemoic!");
      }
    }

    // Check and set the wordlist if provided
    if (count > 1) {
      // Check if the second argument is not undefined/null
      if (!arguments[1].isNull() && !arguments[1].isUndefined()) {
        // Check if the second
        if (!arguments[1].isString()) {
          throw jsi::JSError(runtime, "Second argument must be a string "
                                      "representing the wordlist!");
        }

        wordlist = arguments[1].asString(runtime).utf8(runtime);
      }
    }

    std::string mnemonics = arguments[0].asString(runtime).utf8(runtime);

    auto valid = bip39::Mnemonic::IsValidMnemonic(mnemonics, wordlist);
    return jsi::Value(valid);
  };

  // Convert mnemonic to entropy
  JSI_HOST_FUNCTION(mnemonicToEntropy) {
    std::string wordlist = *_wordlist; // Default wordlist

    if (count > 0) {
      if (!arguments[0].isString()) {
        throw jsi::JSError(runtime, "First argument must be a string representing a mnemoic!");
      }
    }

    // Check and set the wordlist if provided
    if (count > 1) {
      // Check if the second argument is not undefined/null
      if (!arguments[1].isNull() && !arguments[1].isUndefined()) {
        // Check if the second
        if (!arguments[1].isString()) {
          throw jsi::JSError(runtime, "Second argument must be a string "
                                      "representing the wordlist!");
        }

        wordlist = arguments[1].asString(runtime).utf8(runtime);
      }
    }

    std::string mnemonics = arguments[0].asString(runtime).utf8(runtime);
    std::vector<std::string> word_list =
        bip39::ParseWords(mnemonics, bip39::GetDelimiterByLang(wordlist)); // assuming space is the delimiter

    bip39::Mnemonic mnemonic(word_list, wordlist);

    auto entropy = mnemonic.GetEntropyData();

    auto entropyHex = HexStr(entropy);

    return jsi::String::createFromUtf8(runtime, entropyHex);
  };

  // Convert entropy to mnemonic
  JSI_HOST_FUNCTION(entropyToMnemonic) {
    std::string wordlist = *_wordlist; // Default wordlist
    std::vector<uint8_t> entropy;

    if (count > 0) {
      // Check if the first argument is a string or a Uint8Array
      if (arguments[0].isString()) {
        std::string entropyHex = arguments[0].asString(runtime).utf8(runtime);
        entropy = HexToBytes(entropyHex); // Convert hex string to bytes
      } else if (arguments[0].asObject(runtime).isArrayBuffer(runtime)) {
        auto buffer = arguments[0].asObject(runtime).getArrayBuffer(runtime);
        entropy = ArrayBufferToVector(buffer, runtime); // Use the Uint8Array directly as bytes
      } else {
        throw jsi::JSError(runtime, "First argument must be a string or a Uint8Array!");
      }

      // Validate entropy size
      if (entropy.size() % 4 != 0 || entropy.size() > 1024) {
        throw jsi::JSError(runtime, "Invalid entropy: size must be a multiple of "
                                    "4 bytes and not exceed 1024 bytes");
      }
    }
    // Check and set the wordlist if provided
    if (count > 1) {
      // Check if the second argument is not undefined/null
      if (!arguments[1].isNull() && !arguments[1].isUndefined()) {
        // Check if the second
        if (!arguments[1].isString()) {
          throw jsi::JSError(runtime, "Second argument must be a string "
                                      "representing the wordlist!");
        }

        wordlist = arguments[1].asString(runtime).utf8(runtime);
      }
    }

    // create mnemonic
    bip39::Mnemonic mnemonic(entropy); // Use the byte vector directly
    auto word_list = mnemonic.GetWordList(std::string(wordlist));
    std::string mnemonic_sentences = bip39::GenerateWords(word_list, bip39::GetDelimiterByLang(wordlist));
    return jsi::String::createFromUtf8(runtime, mnemonic_sentences);
  };

  std::pair<std::string, std::string> processMnemonicAndPasswordArguments(jsi::Runtime& runtime, const jsi::Value* arguments,
                                                                          size_t count) {
    if (count < 1 || !arguments[0].isString()) {
      throw jsi::JSError(runtime, "First argument must be a string representing a mnemonic!");
    }
    std::string mnemonics = arguments[0].asString(runtime).utf8(runtime);

    std::string password = "";
    if (count > 1 && !arguments[1].isNull() && !arguments[1].isUndefined()) {
      if (!arguments[1].isString()) {
        throw jsi::JSError(runtime, "Second argument must be a string representing the password!");
      }
      password = arguments[1].asString(runtime).utf8(runtime);
    }

    return {mnemonics, password};
  }

  // Convert mnemonic to seed
  JSI_HOST_FUNCTION(mnemonicToSeed) {
    auto [mnemonics, password] = processMnemonicAndPasswordArguments(runtime, arguments, count);

    auto seed = bip39::Mnemonic::CreateSeedFromMnemonic(mnemonics, password);
    // Create a shared buffer for the face template data
    auto buffer = std::make_shared<Bip39MutableBuffer>(seed.size());
    std::memcpy(buffer->data(), seed.data(), seed.size());

    // Return as Uint8Array
    return jsi::ArrayBuffer(runtime, buffer);
  };

  // Convert mnemonic to seed in hex format
  JSI_HOST_FUNCTION(mnemonicToSeedHex) {
    auto [mnemonics, password] = processMnemonicAndPasswordArguments(runtime, arguments, count);

    auto seed = bip39::Mnemonic::CreateSeedFromMnemonic(mnemonics, password);
    auto seedHex = HexStr(seed);

    return jsi::String::createFromUtf8(runtime, seedHex);
  };

  JSI_EXPORT_FUNCTIONS(JSI_EXPORT_FUNC(JsiBip39Api, setDefaultWordlist), JSI_EXPORT_FUNC(JsiBip39Api, getDefaultWordlist),
                       JSI_EXPORT_FUNC(JsiBip39Api, generateMnemonic), JSI_EXPORT_FUNC(JsiBip39Api, validateMnemonic),
                       JSI_EXPORT_FUNC(JsiBip39Api, mnemonicToEntropy), JSI_EXPORT_FUNC(JsiBip39Api, entropyToMnemonic),
                       JSI_EXPORT_FUNC(JsiBip39Api, mnemonicToSeed), JSI_EXPORT_FUNC(JsiBip39Api, mnemonicToSeedHex));

  // JsiBip39Api() : JsiHostObject(), _wordlist("english") {}

private:
  std::shared_ptr<std::string> _wordlist;
};
} // namespace RNBip39
