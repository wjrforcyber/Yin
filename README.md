# Yin
![Yin Lib CTests](https://github.com/wjrforcyber/Yin/actions/workflows/main.yml/badge.svg)
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit)](https://github.com/wjrforcyber/Yin)
## Introduction
C Yin library, named after "寅虎", is one of the Synthesis and Verification Libraries of Zodiac Program.

Yin contains operation on truth table, it is separated from the graph based optimization library. Truth table, especially bit-level based technique will be extended here in this library. As for STP(Semi-tensor product) based example, please refer to [Zilib](https://github.com/wjrforcyber/Zi).

> [!CAUTION]
> Sanitize check is turned on, if mem error/leak exists, PR won't be accepted.

## Build
```
mkdir build
cd build
cmake ..
make
make test
```