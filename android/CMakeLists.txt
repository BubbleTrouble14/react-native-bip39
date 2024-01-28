project("rnbip39")
cmake_minimum_required(VERSION 3.8)

set (CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  string(APPEND CMAKE_CXX_FLAGS " -DDEBUG")
endif()

set(PACKAGE_NAME "rnbip39")

find_package(fbjni REQUIRED CONFIG)
find_package(ReactAndroid REQUIRED CONFIG)

if(${JS_RUNTIME} STREQUAL "hermes")
  find_package(hermes-engine REQUIRED CONFIG)
endif()

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
)

# Specifies a path to native header files.
target_include_directories(
  ${PACKAGE_NAME}
  PRIVATE
  ../bip39_cpp/include
  ../cpp
  "${REACT_NATIVE_DIR}/ReactAndroid/src/main/jni/react/turbomodule"
  "${REACT_NATIVE_DIR}/ReactCommon"
  "${REACT_NATIVE_DIR}/ReactCommon/callinvoker"
  "${REACT_NATIVE_DIR}/ReactCommon/runtimeexecutor"
)

# build shared lib
set_target_properties(${PACKAGE_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(
  ${PACKAGE_NAME}
  log
  android
)

find_package(openssl REQUIRED CONFIG)
find_package(utf8proc REQUIRED CONFIG)

target_link_libraries(
  ${PACKAGE_NAME}
  ReactAndroid::jsi
  ReactAndroid::reactnativejni
  fbjni::fbjni
  openssl::crypto
  openssl::ssl
  utf8proc::utf8proc
)


if(${JS_RUNTIME} STREQUAL "hermes")
  string(APPEND CMAKE_CXX_FLAGS " -DJS_RUNTIME_HERMES=1")

  # From prefab from module `com.facebook.react:hermes-android`
  set(HERMES_LIB hermes-engine::libhermes)

  target_link_libraries(
    ${PACKAGE_NAME}
    ${HERMES_LIB}
  )

  if(${HERMES_ENABLE_DEBUGGER})
    set(HERMES_EXECUTOR_LIB ReactAndroid::hermes_executor)

    target_link_libraries(
      ${PACKAGE_NAME}
      ${HERMES_EXECUTOR_LIB}
    )
  endif()
elseif(${JS_RUNTIME} STREQUAL "jsc")
  string(APPEND CMAKE_CXX_FLAGS " -DJS_RUNTIME_JSC=1")

  set(JSEXECUTOR_LIB ReactAndroid::jscexecutor)

  target_link_libraries(${PACKAGE_NAME} ${JSEXECUTOR_LIB})
else()
  message(FATAL_ERROR "Unknown JS runtime ${JS_RUNTIME}.")
endif()
