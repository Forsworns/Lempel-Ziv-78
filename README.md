# Yet Another Lempel-Ziv 78 

![Language](https://img.shields.io/badge/language-C++-green.svg)
![License](https://img.shields.io/badge/license-MIT-green)

Yet another [Lempel-Ziv-78](https://en.wikipedia.org/wiki/LZ77_and_LZ78) implementation in modern C++. Also adopt the ideas from [Lempel–Ziv–Welch](https://en.wikipedia.org/wiki/Lempel%E2%80%93Ziv%E2%80%93Welch).

Support any dictionary size and any coding symbol. 

For example, 8-bit (ascii) symbols as coding units with a dictionary of size 8096

```shell
make clean
cmake .
make
./obj/lz78_test ./data/txt.txt 8096 8
```

Example:

```C++
LZ78E encoder(dictionarySize);
encoder.encode(inputName, symbolLength);

LZ78D decoder;
decoder.decode(outputName, outputSuffix, symbolLength);
```

