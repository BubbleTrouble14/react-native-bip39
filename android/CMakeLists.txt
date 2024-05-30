cmake_minimum_required(VERSION 3.13)
project(RNBip39)

set(PACKAGE_NAME "rnbip39")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(
        -fexceptions
        -frtti
        -std=c++20
        -Wall
        -Wpedantic
        -DFOLLY_NO_CONFIG=1
        -DLOG_TAG=\"ReactNative\")

find_package(ReactAndroid REQUIRED CONFIG)
find_package(fbjni REQUIRED CONFIG)
find_package(openssl REQUIRED CONFIG)
find_package(utf8proc REQUIRED CONFIG)
find_library(LOG_LIB log)

file(GLOB_RECURSE SOURCES_COMMON CONFIGURE_DEPENDS
    "../cpp/**.cpp"
)

add_library(
  ${PACKAGE_NAME}
  SHARED
  ${SOURCES_COMMON}
  cpp-adapter.cpp
  ../bip39_cpp/src/bit_opts.cpp
  ../bip39_cpp/src/byteswap.cpp
  ../bip39_cpp/src/mnemonic.cpp
  ../bip39_cpp/src/random.cpp
  ../bip39_cpp/src/sha256.cpp
  ../bip39_cpp/src/toolbox.cpp
  ../bip39_cpp/src/langs.cpp
  "./build/generated/source/codegen/jni/react/renderer/components/RNBip39Spec/RNBip39SpecJSI-generated.cpp"
)

# Specifies a path to native header files.
target_include_directories(
  ${PACKAGE_NAME}
  PRIVATE
  ../bip39_cpp/include
  ../cpp
  "./build/generated/source/codegen/jni/react/renderer/components/RNBip39Spec"
)

target_link_libraries(
  ${PACKAGE_NAME}
  ReactAndroid::turbomodulejsijni
  ReactAndroid::jsi                      # <-- RN: JSI
  ReactAndroid::reactnativejni           # <-- RN: React Native JNI bindings
  ReactAndroid::react_nativemodule_core  # <-- RN: React Native native module core
  ${LOG_LIB}                             # <-- Logcat logger
  android                                # <-- Android JNI core
  fbjni::fbjni                    # <-- fbjni
  # react_codegen_RNBip39Spec       # <-- Generated Specs from CodeGen
  # hermes-engine::libhermes        # <-- Hermes Prefab
  openssl::crypto
  openssl::ssl
  utf8proc::utf8proc
)
