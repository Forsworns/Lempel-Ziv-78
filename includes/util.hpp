#include <bitset>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#ifndef YPH_UTIL_H
#define YPH_UTIL_H

namespace yph
{
    using std::pair;
    using std::string;
    using std::unordered_map;
    using std::unordered_set;
    using std::vector;
    using Byte = std::bitset<8>;

    constexpr int CHAR_OVERFLOW = 256;
    const string DELIMETER = ".";

    size_t char2unsign(const char &c)
    {
        int num = static_cast<int>(c);
        return num < 0 ? num + CHAR_OVERFLOW : num;
    }

    size_t bStr2Num(const string &str)
    {
        size_t res = 0;
        size_t len = str.length();
        for (int i = 0; i < len; ++i)
        {
            res += (str[i] - '0') * (1 << (len - i - 1));
        }
        return res;
    }
} // namespace yph

#endif