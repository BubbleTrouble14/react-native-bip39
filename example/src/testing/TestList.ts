import type { TestItemType } from '../navigators/children/Entry/TestItemType';
import { registerInvalidEntropy } from './Tests/InvalidEntropy';
import { registerUtf8Passwords } from './Tests/Utf8Passwords';
import { registerValidateMnemonic } from './Tests/ValidateMnemonic';
import { registerWordListTests } from './Tests/Vectors';

export const TEST_LIST: Array<TestItemType> = [
  {
    description: 'vector',
    value: true,
    registrator: registerWordListTests,
  },
  {
    description: 'invalid entropy',
    value: false,
    registrator: registerInvalidEntropy,
  },
  {
    description: 'utf8 passwords',
    value: false,
    registrator: registerUtf8Passwords,
  },
  {
    description: 'validate mnemonic',
    value: false,
    registrator: registerValidateMnemonic,
  },
];
