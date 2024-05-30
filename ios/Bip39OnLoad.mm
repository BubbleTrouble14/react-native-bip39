//
//  Bip39OnLoad.mm
//  react-native-bip39
//
//  Created by Marc Rousavy on 15.05.24.
//

#import "NativeBip39Module.h"
#import <Foundation/Foundation.h>
#import <ReactCommon/CxxTurboModuleUtils.h>

@interface Bip39OnLoad : NSObject
@end

@implementation Bip39OnLoad

+ (void)load {
  facebook::react::registerCxxModuleToGlobalModuleMap(
      std::string(facebook::react::NativeBip39Module::kModuleName),
      [&](std::shared_ptr<facebook::react::CallInvoker> jsInvoker) {
        return std::make_shared<facebook::react::NativeBip39Module>(
            jsInvoker);
      });
}

@end
