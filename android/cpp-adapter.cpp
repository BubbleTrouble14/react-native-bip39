#include <jni.h>

#include <ReactCommon/CallInvokerHolder.h>
#include <jsi/jsi.h>

#include "JsiBip39.h"
#include "bit_opts.h"
#include "langs.h"
#include "mnemonic.h"
#include "random.h"
#include "toolbox.h"

using namespace facebook; // NOLINT

void install(jsi::Runtime& jsiRuntime)
{
    auto createBip39Instance = jsi::Function::createFromHostFunction(jsiRuntime,
        jsi::PropNameID::forAscii(jsiRuntime, "createBip39Instance"), 0,
        [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* arguments,
            size_t count) -> jsi::Value {
            if (count != 0) {
                throw jsi::JSError(runtime, "Bip39.createNewInstance(..) expects 0 arguments!");
            }

            auto instance = std::make_shared<JsiBip39>();
            return jsi::Object::createFromHostObject(runtime, instance);
        });

    jsiRuntime.global().setProperty(jsiRuntime, "createBip39Instance", std::move(createBip39Instance));

    // auto generateMnemonic = jsi::Function::createFromHostFunction(jsiRuntime,
    //     jsi::PropNameID::forAscii(jsiRuntime, "generateMnemonic"), 2,
    //     [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* arguments,
    //         size_t count) -> jsi::Value {
    //         // Default values
    //         int entropySize = 24; // Default entropy size
    //         std::string lang = "english"; // Default language

    //         if (count > 0) {
    //             if (!arguments[0].isString()) {
    //                 throw jsi::JSError(runtime, "First argument must be a string representing a mnemoic!");
    //             }
    //         }

    //         if (count > 1) {
    //             if (!arguments[1].isString()) {
    //                 throw jsi::JSError(runtime, "Second argument must be a string representing the language!");
    //             }
    //             lang = arguments[1].asString(runtime).utf8(runtime);
    //         }

    //         if (!bip39::Mnemonic::IsValidNumMnemonicSentences(entropySize)) {
    //             throw std::runtime_error("invalid number of mnemonic sentences");
    //         }
    //         // generate entropy
    //         int bits_ent = bip39::Mnemonic::GetEntBitsByNumMnemonicSentences(entropySize);
    //         int num_bytes = bits_ent / 8;
    //         bip39::RandomBytes rnd(num_bytes);
    //         auto ent = rnd.Random();
    //         // create mnemonic
    //         bip39::Mnemonic mnemonic(ent);
    //         auto word_list = mnemonic.GetWordList(std::string(lang));
    //         // std::string mnemonic_sentences = bip39::GenerateWords(word_list, bip39::GetDelimiterByLang(lang));
    //         std::string mnemonic_sentences = bip39::GenerateWords(word_list, bip39::GetDelimiterByLang(lang));
    //         // auto valid = bip39::Mnemonic::IsValidMnemonic(mnemonic_sentences, lang);
    //         // return jsi::Value(valid);
    //         return jsi::String::createFromUtf8(runtime, mnemonic_sentences);
    //     });

    // jsiRuntime.global().setProperty(jsiRuntime, "generateMnemonic", std::move(generateMnemonic));
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_bip39_Bip39Module_nativeInstall(
    JNIEnv* env, jclass obj, jlong jsiRuntimeRef, jobject jsCallInvokerHolder)
{
    auto jsiRuntime { reinterpret_cast<jsi::Runtime*>(jsiRuntimeRef) };
    auto jsCallInvoker { jni::alias_ref<react::CallInvokerHolder::javaobject> {
        reinterpret_cast<react::CallInvokerHolder::javaobject>(jsCallInvokerHolder) } -> cthis()
                             ->getCallInvoker() };

    install(*jsiRuntime);

    return true;
}
