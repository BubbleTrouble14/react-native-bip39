import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';
import type { IBip39Native } from './types';
import type { UnsafeObject } from 'react-native/Libraries/Types/CodegenTypes';
import { ModuleNotFoundError } from './ModuleNotFoundError';

if (__DEV__) {
  console.log('Loading react-native-bip39...');
}

export interface Spec extends TurboModule {
  /**
   * Create a new instance of the Bip39 API.
   * The returned {@linkcode UnsafeObject} is a `jsi::HostObject`.
   */
  createBip39Api(): UnsafeObject;
}

let module: Spec;
try {
  // Try to find the CxxTurboModule.
  // CxxTurboModules can be autolinked on Android starting from react-native 0.74,
  // and are manually linked in Bip39OnLoad.mm on iOS.
  module = TurboModuleRegistry.getEnforcing<Spec>('Bip39Cxx');
} catch (e) {
  // User didn't enable new arch, or the module does not exist.
  throw new ModuleNotFoundError(e);
}

/**
 * The Bip39 API.
 * This object can be shared and accessed from multiple contexts,
 * however it is advised to not hold unnecessary references to it.
 */
export const bip39 = module.createBip39Api() as IBip39Native;

if (__DEV__) {
  console.log('react-native-bip39 loaded successfully!');
}
