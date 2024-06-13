export type WordLists =
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

export type WordCount = 12 | 15 | 18 | 21 | 24;

export interface IBip39Native {
  getDefaultWordlist: () => WordLists;
  setDefaultWordlist: (value: WordLists) => void;
  generateMnemonic: (
    wordCount?: WordCount,
    rng?: ArrayBuffer,
    wordlist?: WordLists
  ) => string;
  validateMnemonic: (mnemonic: string, wordlist?: WordLists) => boolean;
  mnemonicToSeed: (mnemonic: string, password?: string) => ArrayBuffer;
  mnemonicToSeedHex: (mnemonic: string, password?: string) => string;
  mnemonicToEntropy: (mnemonic: string, wordlist?: WordLists) => string;
  entropyToMnemonic: (
    entropy: string | ArrayBuffer,
    wordlist?: WordLists
  ) => string;
}

declare global {
  var bip39: IBip39Native;
}

export const { bip39 } = globalThis;
