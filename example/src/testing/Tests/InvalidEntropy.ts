import { expect } from 'chai';
import { bip39 } from '@bubble/react-native-bip39';
import { describe, it } from '../MochaRNAdapter';

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

export function registerInvalidEntropy() {
  describe('Invalid Entropy Tests', function () {
    it('throws for empty entropy', function () {
      try {
        expect(() =>
          bip39.entropyToMnemonic(hexStringToByteArray(''))
        ).to.throw(TypeError, /Invalid entropy/);
      } catch (e) {}
    });

    it("throws for entropy that's not a multitude of 4 bytes", function () {
      try {
        expect(() =>
          bip39.entropyToMnemonic(hexStringToByteArray('000000'))
        ).to.throw(TypeError, /Invalid entropy/);
      } catch (e) {}
    });

    it('throws for entropy that is larger than 1024', function () {
      try {
        expect(() =>
          bip39.entropyToMnemonic(
            hexStringToByteArray(new Array(1028 + 1).join('00'))
          )
        ).to.throw(TypeError, /Invalid entropy/);
      } catch (e) {}
    });
  });
}
