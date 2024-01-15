#include <jni.h>
#include "react-native-bip39.h"

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_bip39_Bip39Module_nativeMultiply(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return bip39::multiply(a, b);
}
