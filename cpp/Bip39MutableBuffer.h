#pragma once

#include <jsi/jsi.h>

using namespace facebook;

namespace RNBip39 {
class Bip39MutableBuffer : public jsi::MutableBuffer {
public:
  Bip39MutableBuffer(size_t size) : size_(size), data_(new uint8_t[size]) {}
  ~Bip39MutableBuffer() {
    delete[] data_;
  }

  size_t size() const override {
    return size_;
  }
  uint8_t* data() override {
    return data_;
  }

private:
  size_t size_;
  uint8_t* data_;
};
} // namespace RNBip39
