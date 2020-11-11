# Yet Another Lempel-Ziv 78 

![Language](https://img.shields.io/badge/language-C++-green.svg)
![License](https://img.shields.io/badge/license-MIT-green)

Yet another [Lempel-Ziv-78](https://en.wikipedia.org/wiki/LZ77_and_LZ78) implementation in mordern C++. Also adopt the ideas from [Lempel–Ziv–Welch](https://en.wikipedia.org/wiki/Lempel%E2%80%93Ziv%E2%80%93Welch).

For test
```shell
make clean
cmake .
make
./obj/lz78_test ./data/0.txt 8096 any_flag_enable_binary
```

Example:

```C++
LZ78E encoder(stoi(dictionarySize));
encoder.encode(inputName, binaryOrAscii);

LZ78D decoder;
decoder.decode(encoder.getOutputName(), encoder.getInputSuffix(), binaryOrAscii);
```

