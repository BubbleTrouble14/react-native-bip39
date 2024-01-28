/* eslint-disable @typescript-eslint/no-unused-vars */
import 'mocha';
import type * as MochaTypes from 'mocha';
import { useCallback, useEffect, useState } from 'react';
import type { Suites } from '../types/TestSuite';
import type { Stats, SuiteResults, TestResult } from '../types/TestResults';
import { rootSuite } from '../testing/MochaRNAdapter';

const defaultStats = {
  start: new Date(),
  end: new Date(),
  duration: 0,
  suites: 0,
  tests: 0,
  passes: 0,
  pending: 0,
  failures: 0,
};

export const useRunTests = (): [
  SuiteResults,
  (suites: Suites) => void,
  () => void
] => {
  const [results, setResults] = useState<SuiteResults>({});
  const [runner, setRunner] = useState<MochaTypes.Runner | null>(null);

  const addResult = useCallback(
    (newResult: TestResult) => {
      setResults((prev) => {
        if (!prev[newResult.suiteName]) {
          prev[newResult.suiteName] = { results: [] };
        }
        prev[newResult.suiteName]?.results.push(newResult);
        return { ...prev };
      });
    },
    [setResults]
  );

  const runTests = (suites: Suites) => {
    setResults({});
    const currentRunner = run(addResult, suites);
    setRunner(currentRunner); // Keep track of the runner
  };

  const abortTests = () => {
    if (runner) {
      console.log('Aborting tests');
      runner.abort();
      setRunner(null); // Reset the runner reference
    }
  };

  return [results, runTests, abortTests];
};

const run = (
  addTestResult: (testResult: TestResult) => void,
  tests: Suites = {}
) => {
  const {
    EVENT_RUN_BEGIN,
    EVENT_RUN_END,
    EVENT_TEST_FAIL,
    EVENT_TEST_PASS,
    EVENT_TEST_PENDING,
    EVENT_TEST_END,
    EVENT_SUITE_BEGIN,
    EVENT_SUITE_END,
  } = Mocha.Runner.constants;

  let stats: Stats = { ...defaultStats };

  var runner = new Mocha.Runner(rootSuite) as MochaTypes.Runner;
  runner.stats = stats;

  // enable/disable tests based on checkbox value
  // enable/disable tests based on checkbox value
  runner.suite.suites.forEach((suite) => {
    const suiteName = suite.title;
    if (!tests[suiteName]?.value) {
      console.log(`skipping tests in '${suiteName}' suite`);
      suite.tests.forEach((test) => {
        try {
          test.skip();
        } catch (e) {} // do nothing w error
      });
    } else {
      console.log(`will run '${suiteName}' suite`);
      // No additional action needed for tests that should run
    }
  });

  // enable/disable tests based on checkbox value
  runner.suite.suites.map((s) => {
    const suiteName = s.title;
    if (!tests[suiteName]?.value) {
      // console.log(`skipping '${suiteName}' suite`);

      s.tests.map((t) => {
        try {
          t.skip();
        } catch (e) {} // do nothing w error
      });
    } else {
      // console.log(`will run '${suiteName}' suite`);
      s.tests.map((t) => {
        // @ts-expect-error - not sure why this is erroring
        t.reset();
      });
    }
  });

  let indents = -1;
  let currentSuite: string = '';

  const indent = () => Array(indents).join(' ');
  runner
    .once(EVENT_RUN_BEGIN, () => {
      stats.start = new Date();
    })
    .on(EVENT_SUITE_BEGIN, (suite: MochaTypes.Suite) => {
      indents++;
      if (suite.title !== '' && indents === 1) {
        currentSuite = suite.title;
      } else if (indents > 1) {
        addTestResult({
          description: suite.title,
          suiteName: currentSuite,
          type: 'grouping',
          indentation: indents - 1,
        });
      }
    })
    .on(EVENT_SUITE_END, () => {
      indents--;
    })
    .on(EVENT_TEST_PASS, (test: MochaTypes.Runnable) => {
      stats.passes++;
      addTestResult({
        description: test.title,
        suiteName: currentSuite,
        type: 'correct',
        indentation: indents,
      });
      console.log(`${indent()}pass: ${test.fullTitle()}`);
    })
    .on(EVENT_TEST_FAIL, (test: MochaTypes.Runnable, err: Error) => {
      stats.failures++;
      addTestResult({
        indentation: indents,
        description: test.fullTitle(),
        suiteName: currentSuite,
        type: 'incorrect',
        errorMsg: err.message,
      });
      console.log(
        `${indent()}fail: ${test.fullTitle()} - error: ${err.message}`
      );
    })
    .on(EVENT_TEST_PENDING, function () {
      stats.pending++;
    })
    .on(EVENT_TEST_END, function () {
      stats.tests++;
    })
    .once(EVENT_RUN_END, () => {
      stats.end = new Date();
      stats.duration = stats.end.valueOf() - stats.start.valueOf();
      console.log(JSON.stringify(runner.stats, null, 2));
    });

  runner.run();
  return runner; // Return the runner instance
};
