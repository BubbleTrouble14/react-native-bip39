import * as React from 'react';
import { Root } from './navigators/Root';
import { TestApi, Bip39 } from '@ronickg/react-native-bip39';

export default function App() {
  console.log(TestApi.getDefaultWordlist());
  TestApi.setDefaultWordlist('sfsdfsd');
  console.log(TestApi.getDefaultWordlist());
  console.log(
    TestApi.mnemonicToSeed(
      'network cover grow view trophy service iron mushroom fossil mention shoulder good express try various'
    )
  );
  return <Root />;
}
