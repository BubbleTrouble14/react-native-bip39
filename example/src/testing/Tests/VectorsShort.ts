import { expect } from 'chai';
import { describe, it } from '../MochaRNAdapter';
import type { WordLists } from '@ronickg/react-native-bip39';
import { bip39 } from '@ronickg/react-native-bip39';
import vectors from './vectors.json';

function hexStringToByteArray(hexString: string) {
  if (hexString.length % 2 !== 0) {
    throw 'Must have an even number of hex digits to convert to bytes';
  } /* w w w.  jav  a2 s .  c o  m*/
  var numBytes = hexString.length / 2;
  var byteArray = new Uint8Array(numBytes);
  for (var i = 0; i < numBytes; i++) {
    byteArray[i] = parseInt(hexString.substr(i * 2, 2), 16);
  }
  return byteArray;
}

describe('Vectors Short', function () {
  function testVector(
    description: string,
    wordlist: WordLists | undefined,
    password: string,
    v: string[],
    i: number
  ) {
    var ventropy = v[0]!;
    var vmnemonic = v[1]!;
    var vseedHex = v[2]!;

    it(
      'Test Vector for ' + description + '(' + i + '): ' + ventropy,
      function () {
        expect(bip39.mnemonicToEntropy(vmnemonic, wordlist)).to.equal(ventropy);
        const seedHex = bip39.mnemonicToSeedHex(vmnemonic, password);
        expect(seedHex).to.equal(vseedHex);
        const seed = bip39.mnemonicToSeed(vmnemonic, password);
        expect(Buffer.from(seed).toString('hex')).to.equal(vseedHex);
        expect(bip39.entropyToMnemonic(ventropy, wordlist)).to.equal(vmnemonic);
        const rng = hexStringToByteArray(ventropy);
        expect(bip39.generateMnemonic(undefined, rng, wordlist)).to.equal(
          vmnemonic
        );
        expect(bip39.validateMnemonic(vmnemonic, wordlist)).to.be.true;
      }
    );
  }

  describe('English', function () {
    vectors.english.forEach((v, i) => {
      testVector('English', undefined, 'TREZOR', v, i);
    });
  });

  describe('Japanese', function () {
    vectors.japanese.forEach(function (v, i) {
      testVector(
        'Japanese',
        'japanese',
        '㍍ガバヴァぱばぐゞちぢ十人十色',
        v,
        i
      );
    });
  });
});
