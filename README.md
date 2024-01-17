# React Native Bip39

React Native implementation of [Bitcoin BIP39](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki). It's written in TypeScript and has direct bindings to a Bip39 C++ library. The primary function of this implementation is to provide mnemonic code for generating deterministic keys.

## Installation

To use this library in your React Native project, run the following command:

```sh
yarn add @ronickg/react-native-bip39
```

or

```sh
npm install @ronickg/react-native-bip39
```

## Example

```typescript
import { bip39 } from 'your-react-native-bip39-library-name';

const mnemonic = bip39.generateMnemonic(12);
console.log(mnemonic); // Outputs a 12-word mnemonic

const isValid = bip39.validateMnemonic(mnemonic);
console.log(isValid); // true if valid mnemonic

const seed = bip39.mnemonicToSeed(mnemonic);
console.log(seed); // Uint8Array of the seed

const seedHex = bip39.mnemonicToSeedHex(mnemonic);
console.log(seedHex); // Hex string of the seed
```

## Documentation

### generateMnemonic

Generates a mnemonic phrase.

```typescript
generateMnemonic(wordCount?: WordCount, rng?: Uint8Array, wordlist?: WordLists): string
```

- **Parameters:**
  - `wordCount` (optional): Number of words in the mnemonic (e.g., 12, 15, 18, 21, or 24). Default is 12.
  - `rng` (optional): A random number generator represented as a `Uint8Array`.
  - `wordlist` (optional): The word list to use (e.g., 'chinese_simplified', 'english', etc.). Default is 'english'.
- **Returns:** A string representing the generated mnemonic.

### validateMnemonic

Validates a mnemonic phrase.

```typescript
validateMnemonic(mnemonic: string, wordlist?: WordLists): boolean
```

- **Parameters:**
  - `mnemonic`: The mnemonic phrase to validate.
  - `wordlist` (optional): The word list to use (e.g., 'chinese_simplified', 'english', etc.). Default is 'english'.
- **Returns:** A boolean indicating whether the mnemonic is valid.

### mnemonicToSeed

Converts a mnemonic phrase into a binary seed.

```typescript
mnemonicToSeed(mnemonic: string, password?: string): Uint8Array
```

- **Parameters:**
  - `mnemonic`: The mnemonic phrase to convert.
  - `password` (optional): An optional passphrase for additional security.
- **Returns:** A `Uint8Array` representing the binary seed.

### mnemonicToSeedHex

Converts a mnemonic phrase into a hexadecimal seed.

```typescript
mnemonicToSeedHex(mnemonic: string, password?: string): string
```

- **Parameters:**
  - `mnemonic`: The mnemonic phrase to convert.
  - `password` (optional): An optional passphrase for additional security.
- **Returns:** A string representing the hexadecimal seed.

### mnemonicToEntropy

Converts a mnemonic phrase into its binary entropy representation.

```typescript
mnemonicToEntropy(mnemonic: string, wordlist?: WordLists): string
```

- **Parameters:**
  - `mnemonic`: The mnemonic phrase to convert.
  - `wordlist` (optional): The word list to use (e.g., 'chinese_simplified', 'english', etc.). Default is 'english'.
- **Returns:** A string representing the binary entropy.

### entropyToMnemonic

Converts binary entropy into a mnemonic phrase.

```typescript
entropyToMnemonic(entropy: string | Uint8Array, wordlist?: WordLists): string
```

- **Parameters:**
  - `entropy`: The binary entropy to convert.
  - `wordlist` (optional): The word list to use (e.g., 'chinese_simplified', 'english', etc.). Default is 'english'.
- **Returns:** A string representing the mnemonic phrase.

### WordLists

A type representing different word lists that can be used for generating or validating mnemonics.

```typescript
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
```

### WordCount

A type representing the number of words in a mnemonic.

```typescript
type WordCount = 12 | 15 | 18 | 21 | 24;
```

## Resources

- [mattxlee/bip39_cpp](https://github.com/mattxlee/bip39_cpp) C++ Bip39 library
