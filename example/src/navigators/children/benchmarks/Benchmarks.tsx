import React, { useEffect } from 'react';
import { View, Text, StyleSheet, Button } from 'react-native';
import type { RootStackParamList } from '../../RootProps';
import type { NativeStackScreenProps } from '@react-navigation/native-stack';
import { bip39 } from 'react-native-bip39';

// function getTime(f: () => void): number {
//   const before = global.performance.now();
//   f();
//   const after = global.performance.now();
//   return after - before;
// }

function getTime(f: () => void, iterations: number = 100): number {
  let total = 0;
  for (let i = 0; i < iterations; i++) {
    const before = global.performance.now();
    f();
    const after = global.performance.now();
    total += after - before;
  }
  return total / iterations; // Return the average time
}

function startBenchmarking() {
  const mnemonic =
    'field pull pulse resist good travel ice boat earn matter excite window image sugar alpha review elder romance midnight priority reveal modify quit attend';
  const entropy =
    'f585c11aec520db57dd353c69554b21a89b20fb0650966fa0a9d6f74fd989d8f';

  const timeGenerateMnemonic = getTime(() => bip39.generateMnemonic(24));
  const timeValidateMnemonic = getTime(() => bip39.validateMnemonic(mnemonic));
  const timeMnemonicToSeed = getTime(() => bip39.mnemonicToSeed(mnemonic));
  const timeMnemonicToSeedHex = getTime(() =>
    bip39.mnemonicToSeedHex(mnemonic)
  );
  const timeMnemonicToEntropy = getTime(() =>
    bip39.mnemonicToEntropy(mnemonic)
  );
  const timeEntropyToMnemonic = getTime(() => bip39.entropyToMnemonic(entropy));

  console.log('-----Average time of 100 iterations-----');
  console.log(`${timeGenerateMnemonic.toFixed(3)}ms -> Generate Mnemonic`);
  console.log(`${timeValidateMnemonic.toFixed(3)}ms -> Validate Mnemonic`);
  console.log(`${timeMnemonicToSeed.toFixed(3)}ms -> Mnemonic to Seed`);
  console.log(`${timeMnemonicToSeedHex.toFixed(3)}ms -> Mnemonic to Seed Hex`);
  console.log(`${timeMnemonicToEntropy.toFixed(3)}ms -> Mnemonic to Entropy`);
  console.log(`${timeEntropyToMnemonic.toFixed(3)}ms -> Entropy to Mnemonic`);
  console.log('');
}

type BenchmarksProps = NativeStackScreenProps<RootStackParamList, 'Benchmarks'>;

export const Benchmarks: React.FC<BenchmarksProps> = () => {
  return (
    <View style={styles.container}>
      <Text> Testing performance - You can see results in logs! </Text>
      <Button
        title="Benchmark"
        onPress={() => {
          startBenchmarking();
        }}
      />
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    gap: 16,
  },
});
