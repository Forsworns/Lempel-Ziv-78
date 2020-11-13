#include "util.hpp"

#ifndef YPH_LZ78D_H
#define YPH_LZ78D_H

namespace yph
{
    class LZ78D
    {
    private:
        vector<string> dictionary;
        unordered_set<string> symbols;
        string outputName;
        string codes;
        string orig;
        string binaryCodes;
        string binaryOrig;
        size_t indexLength;
        void reset();
        void load(const string &inputName, const string &targetSuffix);
        void decode(size_t codeLength); // code length 1 for binary, 8 for ascii

    public:
        LZ78D();
        ~LZ78D();
        void decode(const string &inputName, const string &targetSuffix, size_t bitLength);
        void save();
    };

    LZ78D::LZ78D()
    {
        reset();
    }

    LZ78D::~LZ78D()
    {
    }

    void LZ78D::reset()
    {
        dictionary.clear();
        symbols.clear();
        orig = "";
        binaryOrig = "";
        binaryCodes = "";
        codes = "";
        indexLength = 0;
        dictionary.emplace_back("");
    }

    void LZ78D::load(const string &inputName, const string &targetSuffix)
    {
        outputName = inputName;
        outputName.erase(outputName.rfind(DELIMETER), outputName.length());
        outputName += ("_lz78." + targetSuffix);
        // std::cout << "input file: " << inputName << ", output file: " << outputName << std::endl;
        std::ifstream fin(inputName, std::ios::in | std::ios::binary);

        if (fin.is_open())
        {
            size_t native = 0;

            fin >> indexLength;
            fin >> native;
            fin.get();

            char c;
            while (fin.get(c))
            {
                auto b = static_cast<unsigned long long>(char2unsign(c));
                binaryCodes += Byte(b).to_string();
            }
            // The completed bits are the 0s placed in the pos ((len/8-1)*8,len-native)
            if (native != 0)
            {
                binaryCodes = binaryCodes.erase(binaryCodes.length() - 8, 8 - native);
            }
        }
        else
        {
            std::cout << "cannot open the input file" << std::endl;
        }
        fin.close();
    }

    void LZ78D::save()
    {
        // dictionary size, codeword length (key length), index length
        std::ofstream fout(outputName, std::ios::out | std::ios::binary);
        for (size_t i = 0; i < binaryOrig.length(); i += 8)
        {
            auto c = Byte(binaryOrig.substr(i, 8)).to_ulong();
            orig += static_cast<char>(c);
        }
        if (fout.is_open())
        {
            for (auto c : orig)
            {
                fout.put(c);
            }
            // fout<<orig;
        }
        else
        {
            std::cout << "cannot open the output file" << std::endl;
        }
        fout.close();
    }

    void LZ78D::decode(size_t codeLength)
    {
        for (size_t i = 0; i < binaryCodes.length(); i += (indexLength + codeLength))
        {
            string binaryIdx = binaryCodes.substr(i, indexLength);
            size_t idx = bStr2Num(binaryIdx);
            string codeword = dictionary[idx] + binaryCodes.substr(i + indexLength, codeLength); // pay attention to the index
            // If the string is shorter than codeLength, the substr() would read as many characters as possible.
            // That is, the substr has dealt with the incomplete last symbol for us already.
            if (symbols.find(codeword) == symbols.end())
            {
                dictionary.push_back(codeword); // the part exceeding the maximum dictionary size in encoder will never be referred
                symbols.insert(codeword);
            }
            binaryOrig += codeword;
        }
    }

    void LZ78D::decode(const string &inputName, const string &targetSuffix, size_t bitLength)
    {
        reset();
        load(inputName, targetSuffix);
        decode(bitLength);
        save();
    }

}; // namespace yph

#endif