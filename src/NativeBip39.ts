import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

export interface Spec extends TurboModule {
  install(): boolean;
}

const Bip39Installer = TurboModuleRegistry.getEnforcing<Spec>('Bip39');

console.log('Loading react-native-bip39...');

if (global.bip39 === undefined || global.bip39 == null) {
  if (Bip39Installer == null || typeof Bip39Installer.install !== 'function') {
    console.error(
      'Native Bip39 Module cannot be found! Make sure you correctly ' +
        'installed native dependencies and rebuilt your app.'
    );
  } else {
    // Install the module
    const result = Bip39Installer.install();
    if (result !== true) {
      console.error(
        `Native Bip39 Module failed to correctly install JSI Bindings! Result: ${result}`
      );
    } else {
      console.log('Bip39 loaded successfully');
    }
  }
} else {
  console.log('react-native-bip39 installed.');
}
