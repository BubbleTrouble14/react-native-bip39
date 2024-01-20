import * as React from 'react';
import { Root } from './navigators/Root';
import { TestApi, Bip39 } from '@ronickg/react-native-bip39';

export default function App() {
  // React.useEffect(() => {
  //   TestApi.setDefaultWordlist('czech');
  //   console.log(TestApi.getDefaultWordlist());
  //   console.log(TestApi.generateMnemonic(12));
  //   console.log(TestApi.generateMnemonic(12));
  //   console.log(TestApi.generateMnemonic(12));
  //   console.log(TestApi.generateMnemonic(12));
  // }, []);

  // const mnemonic = TestApi.generateMnemonic(12);
  // console.log(mnemonic); // Outputs a 12-word mnemonic
  // const mnemonic1 = TestApi.generateMnemonic(12, undefined, 'english');
  // console.log(mnemonic1); // Outputs a 12-word mnemonic
  // const isValid = TestApi.validateMnemonic(mnemonic);
  // console.log(isValid); // true if valid mnemonic

  // const seed = TestApi.mnemonicToSeed(mnemonic);
  // console.log(seed); // Uint8Array of the seed

  // const seedHex = TestApi.mnemonicToSeedHex(mnemonic);
  // console.log(seedHex); // Hex string of the seed
  return <Root />;
}
