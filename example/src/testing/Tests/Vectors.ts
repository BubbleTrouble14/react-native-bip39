import { expect } from 'chai';
import { describe, it } from '../MochaRNAdapter';
import type { WordLists } from 'react-native-bip39';
import { bip39 } from 'react-native-bip39';
import vectors from './vectors.json';
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

// def _check_list(self, language: str, vectors: List[str]) -> None:
// mnemo = Mnemonic(language)
// for v in vectors:
//     code = mnemo.to_mnemonic(bytes.fromhex(v[0]))
//     seed = Mnemonic.to_seed(code, passphrase="TREZOR")
//     xprv = Mnemonic.to_hd_master_key(seed)
//     self.assertIs(mnemo.check(v[1]), True, language)
//     self.assertEqual(v[1], code, language)
//     self.assertEqual(v[2], seed.hex(), language)
//     self.assertEqual(v[3], xprv, language)

export function registerWordListTests() {
  // We need to monkey patch sscrypto to use all the crypto functions from quick-crypto
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

    describe(
      'Test Vector for ' + description + '(' + i + '): ' + ventropy,
      function () {
        it('mnemonicToEntropy returns correct entropy', function () {
          expect(bip39.mnemonicToEntropy(vmnemonic, wordlist)).to.equal(
            ventropy
          );
        });

        it('mnemonicToSeedHex returns correct seed hex', async function () {
          const seedHex = bip39.mnemonicToSeedHex(vmnemonic, password);
          expect(seedHex).to.equal(vseedHex);
        });

        it('mnemonicToSeed returns correct seed buffer', async function () {
          const seed = bip39.mnemonicToSeed(vmnemonic, password);
          expect(Buffer.from(seed).toString('hex')).to.equal(vseedHex);
        });

        it('entropyToMnemonic returns correct mnemonic', function () {
          expect(bip39.entropyToMnemonic(ventropy, wordlist)).to.equal(
            vmnemonic
          );
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
      }
    );
  }

  // vectors.english.forEach((v, i) => {
  //   testVector('English', undefined, 'TREZOR', v, i);
  // });
  // vectors.japanese.forEach(function (v, i) {
  //   testVector('Japanese', 'japanese', '㍍ガバヴァぱばぐゞちぢ十人十色', v, i);
  // });

  function checkList(wordlist: WordLists, vectors: string[]): void {
    vectors.forEach((v, i) => {
      describe(
        'Test Vector for ' + wordlist + '(' + i + '): ' + v[0],
        function () {
          var ventropy = v[0]!;
          var vmnemonic = v[1]!;
          var vseedHex = v[2]!;

          it('mnemonicToEntropy returns correct entropy', function () {
            expect(bip39.mnemonicToEntropy(vmnemonic, wordlist)).to.equal(
              ventropy
            );
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
            expect(bip39.entropyToMnemonic(ventropy, wordlist)).to.equal(
              vmnemonic
            );
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
        }
      );
    });
  }

  for (const lang in vectors1) {
    checkList(lang as WordLists, vectors1[lang]);
  }
}
