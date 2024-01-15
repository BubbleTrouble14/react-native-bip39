cmake_minimum_required(VERSION 3.4.1)
project(Bip39)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 11)

add_library(react-native-bip39            SHARED
            ../cpp/react-native-bip39.cpp
            cpp-adapter.cpp
)

# Specifies a path to native header files.
include_directories(
            ../cpp
)
