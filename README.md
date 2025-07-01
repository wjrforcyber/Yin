# Yin
![Yin Lib CTests](https://github.com/wjrforcyber/Yin/actions/workflows/main.yml/badge.svg)
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit)](https://github.com/wjrforcyber/Yin)
## Introduction
C Yin library, named after "寅虎", is one of the Synthesis and Verification Libraries of Zodiac Program.

Yin contains operation on truth table, it is separated from the graph based optimization library. Truth table, especially bit-level based technique will be extended here in this library. As for STP(Semi-tensor product) based example, please refer to [Zilib](https://github.com/wjrforcyber/Zi).

> [!CAUTION]
> Sanitize check is turned on, if mem error/leak exists, PR won't be accepted.

## Build
Since some functionalities depend on [Zilib](https://github.com/wjrforcyber/Zi), a recursive clone is needed.
```
git clone --recursive git@github.com:wjrforcyber/Yin.git
mkdir build
cd build
cmake ..
make
make test
```

## Select Interface
- Check if a boolean function is positive/negative unate/binate etc. Use 
<br>`enum FUNC_TYPE isUnate(truthTable* tt, ziArray* res);`
For example, XOR is binate, by loading a two input truthtable $T$ which contains `0110`, and an array $A$ for storing each property of a variable $v_i$ on index $i$ of $A$. The final result is the property $P$ of the whole Boolean function.

```C
    ziArray record_xor;
    char tts_xor[] = "0110";
    truthTable *ttXOR = readTT(tts_xor, 1);
    enum FUNC_TYPE type_unate_xor = isUnate(ttXOR, &record_xor);
    assert(type_unate_xor == BINATE);
    clearEachEntryArray(&record_xor);
```