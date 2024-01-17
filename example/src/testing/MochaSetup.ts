import 'mocha';
import type * as MochaTypes from 'mocha';
import type { RowItemType } from '../navigators/children/TestingScreen/RowItemType';
import { clearTests, rootSuite } from './MochaRNAdapter';
import { Alert } from 'react-native';

export function testLib(
  addTestResult: (testResult: RowItemType) => void,
  testRegistrators: Array<() => void> = []
) {
  // console.log('setting up mocha');

  const {
    EVENT_RUN_BEGIN,
    EVENT_RUN_END,
    EVENT_TEST_FAIL,
    EVENT_TEST_PASS,
    EVENT_SUITE_BEGIN,
    EVENT_SUITE_END,
  } = Mocha.Runner.constants;

  clearTests();
  var runner = new Mocha.Runner(rootSuite) as MochaTypes.Runner;

  let indents = -1;
  // Initialize counters
  let passCount = 0;
  let failCount = 0;
  const indent = () => Array(indents).join('  ');
  runner
    .once(EVENT_RUN_BEGIN, () => {})
    .on(EVENT_SUITE_BEGIN, (suite: MochaTypes.Suite) => {
      const name = suite.fullTitle();
      if (name !== '') {
        addTestResult({
          indentation: indents,
          description: name,
          key: Math.random().toString(),
          type: 'grouping',
        });
      }
      indents++;
    })
    .on(EVENT_SUITE_END, () => {
      indents--;
    })
    .on(EVENT_TEST_PASS, (test: MochaTypes.Runnable) => {
      addTestResult({
        indentation: indents,
        description: test.fullTitle(),
        key: Math.random().toString(),
        type: 'correct',
      });
      passCount++;
      console.log(`${indent()}pass: ${test.fullTitle()}`);
    })
    .on(EVENT_TEST_FAIL, (test: MochaTypes.Runnable, err: Error) => {
      addTestResult({
        indentation: indents,
        description: test.fullTitle(),
        key: Math.random().toString(),
        type: 'incorrect',
        errorMsg: err.message,
      });
      failCount++;
      console.log(
        `${indent()}fail: ${test.fullTitle()} - error: ${err.message}`
      );
    })
    .once(EVENT_RUN_END, () => {
      Alert.alert(
        'Test Results Summary',
        `Passed: ${passCount}\nFailed: ${failCount}`,
        [{ text: 'OK', onPress: () => console.log('OK Pressed') }]
      );
      console.log(
        `Tests completed. Passed: ${passCount}, Failed: ${failCount}`
      );
    });

  testRegistrators.forEach((register) => {
    register();
  });
  runner.run();

  return () => {
    console.log('aborting');
    runner.abort();
  };
}
