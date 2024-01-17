import { expect } from 'chai';
import { bip39 } from '@ronickg/react-native-bip39';
import { describe, it } from '../MochaRNAdapter';
import vectors from './vectors.json';

export function registerUtf8Passwords() {
  describe('UTF8 Passwords Tests', function () {
    vectors.japanese.forEach(function (v, index) {
      var vmnemonic = v[1]!;
      var vseedHex = v[2]!;
      var password = '㍍ガバヴァぱばぐゞちぢ十人十色';
      var normalizedPassword =
        'メートルガバヴァぱばぐゞちぢ十人十色';

      it(`mnemonicToSeedSync normalizes passwords for Japanese vector ${index}`, function () {
        expect(bip39.mnemonicToSeedHex(vmnemonic, password)).to.equal(vseedHex);
      });

      it(`mnemonicToSeedSync leaves normalized passwords as-is for Japanese vector ${index}`, function () {
        expect(bip39.mnemonicToSeedHex(vmnemonic, normalizedPassword)).to.equal(
          vseedHex
        );
      });
    });
  });
}
