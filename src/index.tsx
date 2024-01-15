import { NativeModules, Platform } from 'react-native';

const LINKING_ERROR =
  `The package 'react-native-bip39' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo Go\n';

// @ts-expect-error
const isTurboModuleEnabled = global.__turboModuleProxy != null;

const Bip39Module = isTurboModuleEnabled
  ? require('./NativeBip39').default
  : NativeModules.Bip39;

const Bip39 = Bip39Module
  ? Bip39Module
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

export function multiply(a: number, b: number): Promise<number> {
  return Bip39.multiply(a, b);
}
