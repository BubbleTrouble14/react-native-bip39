import type { TestItemType } from '../navigators/children/Entry/TestItemType';
import { registerDefaultWordlist } from './Tests/DefaultWordlist';
import { registerInvalidEntropy } from './Tests/InvalidEntropy';
import { registerUtf8Passwords } from './Tests/Utf8Passwords';
import { registerValidateMnemonic } from './Tests/ValidateMnemonic';
import { registerVectorsLong } from './Tests/VectorsLong';
import { registerVectorsShort } from './Tests/VectorsShort';

export const TEST_LIST: Array<TestItemType> = [
  {
    description: 'vector (short)',
    value: true,
    registrator: registerVectorsShort,
  },
  {
    description: 'vector (long)',
    value: false,
    registrator: registerVectorsLong,
  },
  {
    description: 'default wordlist',
    value: false,
    registrator: registerDefaultWordlist,
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
