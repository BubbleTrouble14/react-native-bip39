/* eslint-disable @typescript-eslint/no-shadow */
import { useState, useCallback } from 'react';
import type * as MochaTypes from 'mocha';
import type { Suites } from '../types/TestSuite';
import { rootSuite } from '../testing/MochaRNAdapter';

import '../testing/Tests/vectorsShort';
import '../testing/Tests/vectorsLong';
import '../testing/Tests/wordlist';
import '../testing/Tests/utf8Passwords';
import '../testing/Tests/invalidEntropy';
import '../testing/Tests/validateMnemonic';

export const useTestList = (): [
  Suites,
  (description: string) => void,
  () => void,
  () => void
] => {
  const [suites, setSuites] = useState<Suites>(getInitialSuites);

  const toggle = useCallback(
    (description: string) => {
      setSuites((suites) => {
        const currentSuite = suites[description];
        const newTests = {
          ...suites,
          [description]: {
            ...suites[description],
            value: !currentSuite!.value,
            count: currentSuite!.count, // Provide a default value if count is undefined
          },
        };

        return newTests;
      });
    },
    [setSuites]
  );
  // const toggle = useCallback(
  //   (description: string) => {
  //     setSuites((tests) => {
  //       tests[description]!.value = !tests[description]!.value;
  //       console.log(
  //         tests,
  //         !tests[description]!.value,
  //         (tests[description]!.value = !tests[description]!.value)
  //       );
  //       return tests;
  //     });
  //   },
  //   [setSuites]
  // );

  const clearAll = useCallback(() => {
    setSuites((suites) => {
      Object.entries(suites).forEach(([_, suite]) => {
        suite.value = false;
      });
      return { ...suites };
    });
  }, [setSuites]);

  const checkAll = useCallback(() => {
    setSuites((suites) => {
      Object.entries(suites).forEach(([_, suite]) => {
        suite.value = true;
      });
      return { ...suites };
    });
  }, [setSuites]);

  return [suites, toggle, clearAll, checkAll];
};

const getInitialSuites = () => {
  let suites: Suites = {};

  // interrogate the loaded mocha suites/tests via a temporary runner
  const runner = new Mocha.Runner(rootSuite) as MochaTypes.Runner;
  runner.suite.suites.map((s) => {
    suites[s.title] = { value: false, count: s.total() };
  });

  // return count-enhanced list and totals
  return suites;
};
