#pragma once

#include <jsi/jsi.h>
#include <string>
#include <vector>

namespace RNBip39 {

namespace jsi = facebook::jsi;

uint8_t char2int(const char input);
std::vector<uint8_t> HexToBytes(const std::string hex);
std::string HexStr(const std::vector<uint8_t>& data);
std::vector<uint8_t> arrayBufferToVector(const jsi::ArrayBuffer& buffer, jsi::Runtime& runtime);

} // namespace RNBip39
