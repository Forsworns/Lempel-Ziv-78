#include "util.hpp"

#ifndef YPH_LZ78E_H
#define YPH_LZ78E_H

namespace yph
{
    class LZ78E
    {
    private:
        size_t maxDictionarySize;
        unordered_map<string, size_t> symbols;
        vector<string> dictionary;
        vector<pair<size_t, string>> result;
        string prev;
        string outputName;
        string codes;
        string orig;
        string binaryOrig;
        string suffix;
        size_t bitLength;
        void reset();
        void load(const string &inputName, std::function<void(char &&)> callback);
        void encode(const string &toCoded);

    public:
        LZ78E(size_t &&ds);
        ~LZ78E();
        void encode(const string &inputName, size_t bitLength);
        void save(std::function<void(std::stringstream &, string)> callback);
        string getOutputName();
        string getInputSuffix();
    };

    LZ78E::LZ78E(size_t &&ds = 256)
    {
        maxDictionarySize = ds;
        reset();
    }

    LZ78E::~LZ78E()
    {
    }

    void LZ78E::reset()
    {
        dictionary.clear();
        symbols.clear();
        result.clear();
        orig = "";
        prev = "";
        binaryOrig = "";
        codes = "";
        suffix = "";
        bitLength = 0;
        dictionary.emplace_back(""); // remember add the empty string into the dictionary
        symbols.emplace("", 0);
    }

    void LZ78E::load(const string &inputName, std::function<void(char &&)> callback)
    {
        outputName = inputName;
        outputName.erase(outputName.rfind(DELIMETER), outputName.length());
        outputName += ".lz78";
        suffix = inputName.substr(inputName.rfind(DELIMETER) + 1);
        // std::cout << "input file: " << inputName << ", output file: " << outputName << std::endl;
        std::ifstream fin(inputName, std::ios::in | std::ios::binary);

        if (fin.is_open())
        {

            char c;
            while (fin.get(c))
            {
                callback(std::move(c));
            }
        }
        else
        {
            std::cout << "cannot open the input file" << std::endl;
        }
        fin.close();
    }

    void LZ78E::save(std::function<void(std::stringstream &, string)> callback)
    {
        // dictionary size, codeword length (key length), index length
        std::ofstream fout(outputName, std::ios::out | std::ios::binary);
        size_t indexLength = 0;
        size_t dl = dictionary.size() - 1;
        while (dl > 0)
        {
            indexLength++;
            dl /= 2;
        }
        fout << indexLength << " ";

        std::stringstream ss;
        for (auto r : result)
        {
            string first = std::bitset<sizeof(decltype(r.first)) * 8>(r.first).to_string(); // the bitset size must be known during compiling
            first = first.substr(first.length() - indexLength);
            ss << first;
            callback(ss, r.second);
        }

        string binaryCodes = "";
        ss >> binaryCodes;
        fout << binaryCodes.length() % 8 << std::endl;       // the original bits without completion, otherwise, think about
        for (size_t i = 0; i < binaryCodes.length(); i += 8) // what if the length is not divided by 8 exactly
        {
            auto c = Byte(binaryCodes.substr(i, 8)).to_ulong();
            codes += static_cast<char>(c);
        }

        if (fout.is_open())
        {
            fout.write(codes.c_str(), codes.length());
            std::cout << binaryCodes.length() / 8 << std::endl;
        }
        else
        {
            std::cout << "cannot open the output file" << std::endl;
        }
        fout.close();
    }

    void LZ78E::encode(const string &toCoded)
    {
        size_t i = 0;
        for (; i < toCoded.length() - toCoded.length() % bitLength; i += bitLength)
        {
            string c = toCoded.substr(i, bitLength);
            string key = prev + c;
            if (symbols.find(key) != symbols.end()) // pay attention, the last key may not be processed in the loop!
            {
                prev += c;
            }
            else
            {
                if (dictionary.size() < maxDictionarySize) // if large than maximum dictionary size, neglect the new key
                {
                    dictionary.push_back(key);
                    symbols.emplace(key, dictionary.size() - 1);
                }
                result.emplace_back(symbols.find(prev)->second, c);

                prev.clear();
            }
        }
        if (i != toCoded.length())
        {
            string c = toCoded.substr(i);
            result.emplace_back(symbols.find(prev)->second, c);
            prev.clear();
        }
        else
        {
            if (prev.length() != 0)
            {
                result.emplace_back(symbols.find(prev.substr(0, prev.length() - bitLength))->second, prev.substr(prev.length() - bitLength));
                prev.clear();
            }
        }
    }

    void LZ78E::encode(const string &inputName, size_t bs)
    {
        reset();
        bitLength = bs;
        if (bitLength % 8 != 0)
        {
            load(inputName, [&](char &&c) {
                Byte b(static_cast<unsigned long long>(char2unsign(c)));
                binaryOrig += b.to_string();
            });
            encode(binaryOrig);
            save([](std::stringstream &ss, string c) {
                ss << c;
            });
        }
        else
        {
            bitLength = bitLength / 8;
            load(inputName, [&](char &&c) {
                orig += c;
            });
            encode(orig);
            save([](std::stringstream &ss, string c) {
                for (auto b : c)
                {
                    ss << Byte(char2unsign(b)).to_string();
                }
            });
        }
    }

    string LZ78E::getOutputName()
    {
        return outputName;
    }

    string LZ78E::getInputSuffix()
    {
        return suffix;
    }
}; // namespace yph

#endif