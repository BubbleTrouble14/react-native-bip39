import { expect } from 'chai';
import { describe, it } from '../MochaRNAdapter';
import type { WordLists } from '@ronickg/react-native-bip39';
import { bip39 } from '@ronickg/react-native-bip39';
import vectors1 from './vectors1.json';

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

describe('Vectors Long', function () {
  function checkList(wordlist: WordLists, vectors: string[]): void {
    vectors.forEach((v, i) => {
      // describe(
      //   'Test Vector for ' + wordlist + '(' + i + '): ' + v[0],
      //   function () {
      var ventropy = v[0]!;
      var vmnemonic = v[1]!;
      var vseedHex = v[2]!;

      it('mnemonicToEntropy returns correct entropy', function () {
        expect(bip39.mnemonicToEntropy(vmnemonic, wordlist)).to.equal(ventropy);
      });

      it('mnemonicToSeedHex returns correct seed hex', async function () {
        const seedHex = bip39.mnemonicToSeedHex(vmnemonic, 'TREZOR');
        expect(seedHex).to.equal(vseedHex);
      });

      it('mnemonicToSeed returns correct seed buffer', async function () {
        const seed = bip39.mnemonicToSeed(vmnemonic, 'TREZOR');
        expect(Buffer.from(seed).toString('hex')).to.equal(vseedHex);
      });

      it('entropyToMnemonic returns correct mnemonic', function () {
        expect(bip39.entropyToMnemonic(ventropy, wordlist)).to.equal(vmnemonic);
      });

      it('generateMnemonic returns RNG entropy unmodified', function () {
        const rng = hexStringToByteArray(ventropy);
        expect(bip39.generateMnemonic(undefined, rng, wordlist)).to.equal(
          vmnemonic
        );
      });

      it('validateMnemonic returns true for valid mnemonic', function () {
        expect(bip39.validateMnemonic(vmnemonic, wordlist)).to.be.true;
      });
    });
    // });
  }

  for (const lang in vectors1) {
    checkList(lang as WordLists, vectors1[lang]);
  }
});
