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
        void load(string inputName, string targetSuffix);
        void decode(int codeLength); // code length 1 for binary, 8 for ascii

    public:
        LZ78D(int kl);
        ~LZ78D();
        void decode(string inputName, string targetSuffix, bool binaryOrAscii);
        void dump();
    };

    LZ78D::LZ78D(int ds = 8)
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

    void LZ78D::load(string inputName, string targetSuffix)
    {
        outputName = inputName;
        outputName.erase(outputName.rfind(DELIMETER), outputName.length());
        outputName += ("_lz78." + targetSuffix);
        std::cout << "input file: " << inputName << ", output file: " << outputName << std::endl;
        std::ifstream fin(inputName, std::ios::in | std::ios::binary);

        if (fin.is_open())
        {
            size_t native = 0;
            //std::cout << "load input file" << std::endl;
            fin >> indexLength;
            fin >> native;
            fin.get();

            char c;
            while (fin.get(c))
            {
                auto b = static_cast<unsigned long long>(char2unsign(c));
                binaryCodes += Byte(b).to_string();
            }
            //std::cout << "read in:" << binaryCodes.length() / 8 << std::endl;
            // The completed bits are the 0s placed in the pos ((len/8-1)*8,len-native)
            //std::cout << "native codes in the last byte: " << native << std::endl;
            if (native != 0)
            {
                binaryCodes = binaryCodes.erase(binaryCodes.length() - 8, 8 - native);
            }
            //std::cout << "the completion is removed, see the reminder: " << binaryCodes.length() % (indexLength + 1) << std::endl;
        }
        else
        {
            std::cout << "cannot open the input file" << std::endl;
        }
        fin.close();
        //std::cout << "close the file" << std::endl;
    }

    void LZ78D::dump()
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

    void LZ78D::decode(int codeLength)
    {
        // the length may not be divided by (indexLength+1)
        for (size_t i = 0; i < binaryCodes.length(); i += (indexLength + codeLength))
        {
            // indexLength+1 because the coding is (index,0/1) in binary case
            string binaryIdx = binaryCodes.substr(i, indexLength);
            size_t idx = bStr2Num(binaryIdx);
            string codeword = dictionary[idx] + binaryCodes.substr(i + indexLength, codeLength); // pay attention to the index
            if (symbols.find(codeword) == symbols.end())
            {
                dictionary.push_back(codeword); // the part exceeding the maximum dictionary size in encoder will never be referred
                symbols.insert(codeword);
            }
            binaryOrig += codeword;
        }

        // std::cout << binaryOrig.substr(binaryOrig.length() - 64) << std::endl;
    }

    void LZ78D::decode(string inputName, string targetSuffix, bool binaryOrAscii)
    {
        reset();

        load(inputName, targetSuffix);
        if (binaryOrAscii)
        {
            decode(1); // 0 or 1 only takes 1 bit
        }
        else
        {
            decode(8); // ascii takes 8 bits
        }
        dump();
    }

}; // namespace yph

#endif