import { expect } from 'chai';
import { bip39 } from '@ronickg/react-native-bip39';
import { describe, it } from '../MochaRNAdapter';

describe('Wordlist', function () {
  it('setDefaultWordlist changes default wordlist', function () {
    const english = bip39.getDefaultWordlist();
    expect(english).to.equal('english');

    bip39.setDefaultWordlist('italian');
    const italian = bip39.getDefaultWordlist();
    expect(italian).to.equal('italian');

    const phraseItalian = bip39.entropyToMnemonic(
      '00000000000000000000000000000000'
    );
    expect(phraseItalian.slice(0, 5)).to.equal('abaco');

    bip39.setDefaultWordlist('english');
    const phraseEnglish = bip39.entropyToMnemonic(
      '00000000000000000000000000000000'
    );
    expect(phraseEnglish.slice(0, 7)).to.equal('abandon');
  });

  it('setDefaultWordlist throws on unknown wordlist', function () {
    const english = bip39.getDefaultWordlist();
    expect(english).to.equal('english');

    try {
      expect(() => bip39.setDefaultWordlist('abcdefghijklmnop')).to.throw(
        TypeError,
        /Could not find wordlist for language "abcdefghijklmnop/
      );
    } catch (e) {}
  });
});
