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

#include "JsiHostObject.h"
#include "TypedArray.h"
#include "bit_opts.h"
#include "langs.h"
#include "mnemonic.h"
#include "random.h"
#include "toolbox.h"

namespace RNBip39 {

namespace jsi = facebook::jsi;

static uint8_t char2int(const char input) {
  if (input >= '0' && input <= '9')
    return input - '0';
  if (input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if (input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  throw std::invalid_argument("Invalid input string");
}

static std::vector<uint8_t> HexToBytes(const std::string hex) {
  if (hex.size() % 2 != 0) {
    throw std::invalid_argument("Invalid input string, length must be multple of 2");
  }
  std::vector<uint8_t> ret = std::vector<uint8_t>();
  size_t start_at = 0;
  if (hex.rfind("0x", 0) == 0 || hex.rfind("0x", 0) == 0) {
    start_at = 2;
  }

  for (size_t i = start_at; i < hex.size(); i += 2) {
    ret.push_back(char2int(hex[i]) * 16 + char2int(hex[i + 1]));
  }
  return ret;
}

static std::string HexStr(const std::vector<uint8_t>& data) {
  std::stringstream s;
  s << std::hex;
  for (size_t i = 0; i < data.size(); ++i)
    s << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
  return s.str();
}

// std::vector<uint8_t> intToVector(uint32_t value) {
//   std::vector<uint8_t> bytes;
//   for (size_t i = 0; i < sizeof(value); ++i) {
//     // Shift the integer's bits to the right and get the lowest byte.
//     bytes.push_back((value >> (i * 8)) & 0xFF);
//   }
//   return bytes;
// }

class JsiBip39Api : public RNJsi::JsiHostObject {
public:
  static const char* Bip39Name;
  static void installApi(jsi::Runtime& runtime);
  static void invalidateInstance();
  static std::shared_ptr<JsiBip39Api> getInstance();

  JSI_HOST_FUNCTION(getDefaultWordlist) {
    return jsi::String::createFromUtf8(runtime, _wordlist);
  };

  JSI_HOST_FUNCTION(setDefaultWordlist) {
    if (count == 0) {
      throw jsi::JSError(runtime, "setDefaultWordlist expects the wordlist name parameter.");
    }

    if (!arguments[0].isString()) {
      throw jsi::JSError(runtime, "setDefaultWordlist expects the wordlist name parameter as a string.");
    }

    std::string lang = arguments[0].asString(runtime).utf8(runtime);

    auto it = bip39::langs.find(lang);
    if (!bip39::utils::LangExists(lang)) {
      throw jsi::JSError(runtime, "Invalid lang name");
    }

    _wordlist = lang;
    return jsi::Value::undefined();
  };
  // Generate mnemonic
  JSI_HOST_FUNCTION(generateMnemonic) {
    // Default values
    int wordCount = 12; // Default entropy size
    std::vector<uint8_t> ent;
    std::string wordlist = _wordlist;

    // Check and set the entropy size if provided
    if (count > 0 && arguments[0].isNumber()) {
      wordCount = arguments[0].asNumber();
    }

    // Check and set rng data if provided
    if (count > 1 && !arguments[1].isNull() && !arguments[1].isUndefined()) {
      auto object = arguments[1].asObject(runtime);
      if (!isTypedArray(runtime, object)) {
        throw jsi::JSError(runtime, "Second argument must be a Uint8Array representing rng!");
      }
      auto typedArray = getTypedArray(runtime, object);
      ent = typedArray.toVector(runtime);
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
    std::string wordlist = _wordlist;

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
    std::string wordlist = _wordlist; // Default wordlist

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
    std::string wordlist = _wordlist; // Default wordlist
    std::vector<uint8_t> entropy;

    if (count > 0) {
      // Check if the first argument is a string or a Uint8Array
      if (arguments[0].isString()) {
        std::string entropyHex = arguments[0].asString(runtime).utf8(runtime);
        entropy = HexToBytes(entropyHex); // Convert hex string to bytes
      } else if (isTypedArray(runtime, arguments[0].asObject(runtime))) {
        auto typedArray = getTypedArray(runtime, arguments[0].asObject(runtime));
        entropy = typedArray.toVector(runtime); // Use the Uint8Array directly as bytes
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

    auto seed = bip39::Mnemonic::CreateSeed(mnemonics, password);
    auto byteArray = TypedArray<TypedArrayKind::Uint8Array>(runtime, seed.size());
    auto arrayBuffer = byteArray.getBuffer(runtime);
    memcpy(arrayBuffer.data(runtime), seed.data(), seed.size());

    return byteArray;
  };

  // Convert mnemonic to seed in hex format
  JSI_HOST_FUNCTION(mnemonicToSeedHex) {
    auto [mnemonics, password] = processMnemonicAndPasswordArguments(runtime, arguments, count);

    auto seed = bip39::Mnemonic::CreateSeed(mnemonics, password);
    auto seedHex = HexStr(seed);

    return jsi::String::createFromUtf8(runtime, seedHex);
  };

  JSI_EXPORT_FUNCTIONS(JSI_EXPORT_FUNC(JsiBip39Api, setDefaultWordlist), JSI_EXPORT_FUNC(JsiBip39Api, getDefaultWordlist),
                       JSI_EXPORT_FUNC(JsiBip39Api, generateMnemonic), JSI_EXPORT_FUNC(JsiBip39Api, validateMnemonic),
                       JSI_EXPORT_FUNC(JsiBip39Api, mnemonicToEntropy), JSI_EXPORT_FUNC(JsiBip39Api, entropyToMnemonic),
                       JSI_EXPORT_FUNC(JsiBip39Api, mnemonicToSeed), JSI_EXPORT_FUNC(JsiBip39Api, mnemonicToSeedHex));

  JsiBip39Api() : JsiHostObject(), _wordlist("english") {}

private:
  std::string _wordlist;
  static std::shared_ptr<JsiBip39Api> instance;
};
} // namespace RNBip39
