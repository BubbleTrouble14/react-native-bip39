#include "JsiBip39.h"
#include "TypedArray.h"
#include <jsi/jsi.h>
#include <string>
#include <thread>
#include <vector>

#include <map>
#include <string_view>

#include "bit_opts.h"
#include "langs.h"
#include "mnemonic.h"
#include "random.h"
#include "toolbox.h"
#include <algorithm>
#include <array>
#include <cstring>
#include <iomanip>
#include <sstream>

using namespace facebook::jsi;
using namespace std;

static uint8_t char2int(const char input)
{
    if (input >= '0' && input <= '9')
        return input - '0';
    if (input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if (input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    throw std::invalid_argument("Invalid input string");
}

static std::vector<uint8_t> HexToBytes(const std::string hex)
{
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

static std::string HexStr(const std::vector<uint8_t>& data)
{
    std::stringstream s;
    s << std::hex;
    for (size_t i = 0; i < data.size(); ++i)
        s << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    return s.str();
}

std::vector<uint8_t> intToVector(uint32_t value)
{
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < sizeof(value); ++i) {
        // Shift the integer's bits to the right and get the lowest byte.
        bytes.push_back((value >> (i * 8)) & 0xFF);
    }
    return bytes;
}

// Constructor
JsiBip39::JsiBip39() { }

// Destructor
JsiBip39::~JsiBip39() { }

std::vector<jsi::PropNameID> JsiBip39::getPropertyNames(jsi::Runtime& rt)
{
    std::vector<jsi::PropNameID> result;
    result.push_back(jsi::PropNameID::forUtf8(rt, std::string("generateMnemonic")));
    result.push_back(jsi::PropNameID::forUtf8(rt, std::string("validateMnemonic")));
    result.push_back(jsi::PropNameID::forUtf8(rt, std::string("mnemonicToEntropy")));
    result.push_back(jsi::PropNameID::forUtf8(rt, std::string("entropyToMnemonic")));
    result.push_back(jsi::PropNameID::forUtf8(rt, std::string("mnemonicToSeed")));
    result.push_back(jsi::PropNameID::forUtf8(rt, std::string("mnemonicToSeedHex")));
    return result;
}

jsi::Value JsiBip39::get(jsi::Runtime& runtime, const jsi::PropNameID& propNameId)
{
    auto propName = propNameId.utf8(runtime);
    auto funcName = "Bip39." + propName;
    // throw jsi::JSError(runtime, "Called hello");

    if (propName == "generateMnemonic") {
        return jsi::Function::createFromHostFunction(runtime, jsi::PropNameID::forAscii(runtime, funcName), 3,
            [](jsi::Runtime& runtime, const jsi::Value& Value, const jsi::Value* arguments,
                size_t count) -> jsi::Value {
                // Default values
                int entropySize = 12; // Default entropy size
                std::string lang = "english"; // Default language
                std::vector<uint8_t> ent;

                // Check and set the language if provided
                if (count > 0) {
                    // Check if the second argument is not undefined/null
                    if (!arguments[0].isNull() && !arguments[0].isUndefined()) {
                        // Check if the second
                        if (!arguments[0].isNumber()) {
                            throw jsi::JSError(
                                runtime, "First argument must be a number representing the entropy size!");
                        }

                        entropySize = arguments[0].asNumber();
                    }
                }

                // Check and set the language if provided
                if (count > 1) {
                    // Check if the second argument is not undefined/null
                    if (!arguments[1].isNull() && !arguments[1].isUndefined()) {
                        // Check if the second
                        auto object = arguments[1].asObject(runtime);
                        if (!isTypedArray(runtime, object)) {
                            throw jsi::JSError(runtime, "Second argument must be a Uint8Array representing rng!");
                        }

                        auto typedArray = getTypedArray(runtime, object);
                        ent = typedArray.toVector(runtime);
                    } else {
                        // generate entropy
                        int bits_ent = bip39::Mnemonic::GetEntBitsByNumMnemonicSentences(entropySize);
                        int num_bytes = bits_ent / 8;
                        bip39::RandomBytes rnd(num_bytes);
                        ent = rnd.Random();
                    }
                }

                // Check and set the language if provided
                if (count > 2) {
                    // Check if the second argument is not undefined/null
                    if (!arguments[2].isNull() && !arguments[2].isUndefined()) {
                        // Check if the second
                        if (!arguments[2].isString()) {
                            throw jsi::JSError(runtime, "Third argument must be a string representing the language!");
                        }

                        lang = arguments[2].asString(runtime).utf8(runtime);
                    }
                }

                if (!bip39::Mnemonic::IsValidNumMnemonicSentences(entropySize)) {
                    throw std::runtime_error("invalid number of mnemonic sentences");
                }

                // create mnemonic
                bip39::Mnemonic mnemonic(ent);
                auto word_list = mnemonic.GetWordList(std::string(lang));
                std::string mnemonic_sentences = bip39::GenerateWords(word_list, bip39::GetDelimiterByLang(lang));

                return jsi::String::createFromUtf8(runtime, mnemonic_sentences);
            });
    }

    if (propName == "validateMnemonic") {
        return jsi::Function::createFromHostFunction(runtime, jsi::PropNameID::forAscii(runtime, funcName), 2,
            [](jsi::Runtime& runtime, const jsi::Value& Value, const jsi::Value* arguments,
                size_t count) -> jsi::Value {
                // Default values
                std::string lang = "english"; // Default language

                if (count > 0) {
                    if (!arguments[0].isString()) {
                        throw jsi::JSError(runtime, "First argument must be a string representing a mnemoic!");
                    }
                }

                // Check and set the language if provided
                if (count > 1) {
                    // Check if the second argument is not undefined/null
                    if (!arguments[1].isNull() && !arguments[1].isUndefined()) {
                        // Check if the second
                        if (!arguments[1].isString()) {
                            throw jsi::JSError(runtime, "Second argument must be a string representing the language!");
                        }

                        lang = arguments[1].asString(runtime).utf8(runtime);
                    }
                }

                std::string mnemonics = arguments[0].asString(runtime).utf8(runtime);

                auto valid = bip39::Mnemonic::IsValidMnemonic(mnemonics, lang);
                return jsi::Value(valid);
            });
    }

    if (propName == "mnemonicToEntropy") {
        return jsi::Function::createFromHostFunction(runtime, jsi::PropNameID::forAscii(runtime, funcName), 2,
            [](jsi::Runtime& runtime, const jsi::Value& Value, const jsi::Value* arguments,
                size_t count) -> jsi::Value {
                // Default values
                std::string lang = "english"; // Default language

                if (count > 0) {
                    if (!arguments[0].isString()) {
                        throw jsi::JSError(runtime, "First argument must be a string representing a mnemoic!");
                    }
                }

                // Check and set the language if provided
                if (count > 1) {
                    // Check if the second argument is not undefined/null
                    if (!arguments[1].isNull() && !arguments[1].isUndefined()) {
                        // Check if the second
                        if (!arguments[1].isString()) {
                            throw jsi::JSError(runtime, "Second argument must be a string representing the language!");
                        }

                        lang = arguments[1].asString(runtime).utf8(runtime);
                    }
                }

                // std::string errorMessage = "Invalid language: " + lang;
                // throw jsi::JSError(runtime, errorMessage);

                std::string mnemonics = arguments[0].asString(runtime).utf8(runtime);
                std::vector<std::string> word_list
                    = bip39::ParseWords(mnemonics, bip39::GetDelimiterByLang(lang)); // assuming space is the delimiter

                bip39::Mnemonic mnemonic(word_list, lang);

                auto entropy = mnemonic.GetEntropyData();

                auto entropyHex = HexStr(entropy);

                return jsi::String::createFromUtf8(runtime, entropyHex);
            });
    }

    if (propName == "entropyToMnemonic") {
        return jsi::Function::createFromHostFunction(runtime, jsi::PropNameID::forAscii(runtime, funcName), 2,
            [](jsi::Runtime& runtime, const jsi::Value& Value, const jsi::Value* arguments,
                size_t count) -> jsi::Value {
                // Default values
                std::string lang = "english"; // Default language
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
                        throw jsi::JSError(
                            runtime, "Invalid entropy: size must be a multiple of 4 bytes and not exceed 1024 bytes");
                    }
                }
                // Check and set the language if provided
                if (count > 1) {
                    // Check if the second argument is not undefined/null
                    if (!arguments[1].isNull() && !arguments[1].isUndefined()) {
                        // Check if the second
                        if (!arguments[1].isString()) {
                            throw jsi::JSError(runtime, "Second argument must be a string representing the language!");
                        }

                        lang = arguments[1].asString(runtime).utf8(runtime);
                    }
                }

                // create mnemonic
                bip39::Mnemonic mnemonic(entropy); // Use the byte vector directly
                auto word_list = mnemonic.GetWordList(std::string(lang));
                std::string mnemonic_sentences = bip39::GenerateWords(word_list, bip39::GetDelimiterByLang(lang));
                return jsi::String::createFromUtf8(runtime, mnemonic_sentences);
            });
    }

    if (propName == "mnemonicToSeed") {
        return jsi::Function::createFromHostFunction(runtime, jsi::PropNameID::forAscii(runtime, funcName), 2,
            [](jsi::Runtime& runtime, const jsi::Value& Value, const jsi::Value* arguments,
                size_t count) -> jsi::Value {
                // Check if the first argument (mnemonic) is provided and is a string
                if (count < 1 || !arguments[0].isString()) {
                    throw jsi::JSError(runtime, "First argument must be a string representing a mnemonic!");
                }
                std::string mnemonics = arguments[0].asString(runtime).utf8(runtime);

                // Initialize password as an empty string
                std::string password = "";

                // Check and set the language if provided
                if (count > 1) {
                    // Check if the second argument is not undefined/null
                    if (!arguments[1].isNull() && !arguments[1].isUndefined()) {
                        // Check if the second
                        if (!arguments[1].isString()) {
                            throw jsi::JSError(runtime, "Second argument must be a string representing the password!");
                        }

                        password = arguments[1].asString(runtime).utf8(runtime);
                    }
                }

                // Generate the seed using the mnemonic and password
                auto seed = bip39::Mnemonic::CreateSeed(mnemonics, password);

                auto byteArray = TypedArray<TypedArrayKind::Uint8Array>(runtime, seed.size());
                auto arrayBuffer = byteArray.getBuffer(runtime);

                memcpy(arrayBuffer.data(runtime), seed.data(), seed.size());

                return byteArray;
            });
    }

    if (propName == "mnemonicToSeedHex") {
        return jsi::Function::createFromHostFunction(runtime, jsi::PropNameID::forAscii(runtime, funcName), 2,
            [](jsi::Runtime& runtime, const jsi::Value& Value, const jsi::Value* arguments,
                size_t count) -> jsi::Value {
                // Check if the first argument (mnemonic) is provided and is a string
                if (count < 1 || !arguments[0].isString()) {
                    throw jsi::JSError(runtime, "First argument must be a string representing a mnemonic!");
                }
                std::string mnemonics = arguments[0].asString(runtime).utf8(runtime);

                // Initialize password as an empty string
                std::string password = "";

                // Check and set the language if provided
                if (count > 1) {
                    // Check if the second argument is not undefined/null
                    if (!arguments[1].isNull() && !arguments[1].isUndefined()) {
                        // Check if the second
                        if (!arguments[1].isString()) {
                            throw jsi::JSError(runtime, "Second argument must be a string representing the password!");
                        }

                        password = arguments[1].asString(runtime).utf8(runtime);
                    }
                }
                // Generate the seed using the mnemonic and password
                auto seed = bip39::Mnemonic::CreateSeed(mnemonics, password);

                // Convert the seed to a hexadecimal string
                auto seedHex = HexStr(seed);

                return jsi::String::createFromUtf8(runtime, seedHex);
            });
    }

    throw jsi::JSError(runtime, "Unknown property: " + propName);
}
