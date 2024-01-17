#ifdef __cplusplus
#import "JsiBip39.h"
#endif

#ifdef RCT_NEW_ARCH_ENABLED
#import "RNBip39Spec.h"

@interface Bip39 : NSObject <NativeBip39Spec>
#else
#import <React/RCTBridgeModule.h>

@interface Bip39 : NSObject <RCTBridgeModule>
#endif

@end
