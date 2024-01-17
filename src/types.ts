// declare global {
//   function createBip39Instance(): CppBip39;
// }

interface IBip39 {
  generateMnemonic(
    wordCount?: WordCount,
    rng?: Uint8Array,
    wordlist?: WordLists
  ): string;
  validateMnemonic(mnemonic: string, wordlist?: WordLists): boolean;
  mnemonicToSeed(mnemonic: string, password?: string): Uint8Array;
  mnemonicToSeedHex(mnemonic: string, password?: string): string;
  mnemonicToEntropy(mnemonic: string, wordlist?: WordLists): string;
  entropyToMnemonic(entropy: string | Uint8Array, wordlist?: WordLists): string;
}

type CppBip39 = Pick<
  IBip39,
  | 'generateMnemonic'
  | 'validateMnemonic'
  | 'mnemonicToSeed'
  | 'mnemonicToSeedHex'
  | 'mnemonicToEntropy'
  | 'entropyToMnemonic'
>;

type WordLists =
  | 'chinese_simplified'
  | 'chinese_traditional'
  | 'czech'
  | 'english'
  | 'french'
  | 'italian'
  | 'japanese'
  | 'korean'
  | 'portuguese'
  | 'spanish';

type WordCount = 12 | 15 | 18 | 21 | 24;

const instanceBip39 = (): CppBip39 => {
  // @ts-expect-error
  if (global.createBip39Instance == null)
    throw new Error(
      'Failed to create a new Bip39 instance, the native initializer function does not exist. Are you trying to use Bip39 from different JS Runtimes?'
    );
  // @ts-expect-error
  return global.createBip39Instance();
};

class Bip39 {
  static generateMnemonic(
    wordCount?: WordCount,
    rng?: Uint8Array,
    wordlist?: WordLists
  ): string {
    return instanceBip39().generateMnemonic(wordCount, rng, wordlist);
  }

  static validateMnemonic(mnemonic: string, wordlist?: WordLists): boolean {
    return instanceBip39().validateMnemonic(mnemonic, wordlist);
  }

  static mnemonicToSeed(mnemonic: string, password?: string): Uint8Array {
    return instanceBip39().mnemonicToSeed(mnemonic, password);
  }

  static mnemonicToSeedHex(mnemonic: string, password?: string): string {
    return instanceBip39().mnemonicToSeedHex(mnemonic, password);
  }

  static mnemonicToEntropy(mnemonic: string, wordlist?: WordLists): string {
    return instanceBip39().mnemonicToEntropy(mnemonic, wordlist);
  }

  static entropyToMnemonic(
    entropy: string | Uint8Array,
    wordlist?: WordLists
  ): string {
    return instanceBip39().entropyToMnemonic(entropy, wordlist);
  }
}

export type { WordCount, WordLists };
export { Bip39 as bip39 };
// export { Bip39 };
// declare global {
//   var Bip39: Bip39;
// }

// export const { Bip39 } = globalThis;
