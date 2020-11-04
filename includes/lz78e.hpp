#include "util.hpp"

#ifndef YPH_LZ78E_H
#define YPH_LZ78E_H

namespace yph
{

    class LZ78E
    {
    private:
        int maxDictionarySize;
        unordered_map<string, size_t> symbols;
        vector<string> dictionary;
        vector<pair<size_t, char>> result;
        string prev;
        string outputName;
        string codes;
        string orig;
        string binaryOrig;
        string suffix;
        void reset();
        void load(string inputName, std::function<void(char)> callback);
        void encode(string toCoded);

    public:
        LZ78E(int ds);
        ~LZ78E();
        void encode(string inputName, bool binaryOrAscii);
        void dump(std::function<void(std::stringstream &, char)> callback);
        string getOutputName();
        string getInputSuffix();
    };

    LZ78E::LZ78E(int ds = 8)
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
        dictionary.emplace_back(""); // remember add the empty string into the dictionary
        symbols.emplace("", 0);
    }

    void LZ78E::load(string inputName, std::function<void(char)> callback)
    {
        outputName = inputName;
        outputName.erase(outputName.rfind(DELIMETER), outputName.length());
        outputName += ".lz78";
        suffix = inputName.substr(inputName.rfind(DELIMETER) + 1);
        std::cout << "input file: " << inputName << ", output file: " << outputName << std::endl;
        std::ifstream fin(inputName, std::ios::in | std::ios::binary);

        if (fin.is_open())
        {
            //std::cout << "load input file" << std::endl;
            char c;
            while (fin.get(c))
            {
                callback(c);
            }
        }
        else
        {
            std::cout << "cannot open the input file" << std::endl;
        }
        fin.close();
        // std::cout << "close the file" << std::endl;
    }

    void LZ78E::dump(std::function<void(std::stringstream &, char)> callback)
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
        // std::cout << indexLength << std::endl;

        std::stringstream ss;
        for (auto r : result)
        {
            string first = std::bitset<sizeof(decltype(r.first)) * 8>(r.first).to_string();
            first = first.substr(first.length() - indexLength);
            ss << first;
            callback(ss, r.second);
            // std::cout << first << std::endl;
            // std::cout << r.second << std::endl;
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
            //std::cout << "write codes of length" << codes.length() << std::endl;
        }
        else
        {
            std::cout << "cannot open the output file" << std::endl;
        }
        fout.close();
    }

    void LZ78E::encode(string toCoded)
    {
        // std::cout << binaryOrig.substr(binaryOrig.length() - 64) << std::endl;
        for (auto c : toCoded)
        {
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
                // std::cout << key << ":" << symbols.find(prev)->second << "," << c << std::endl;
                prev.clear();
            }
        }
        if (prev.length() != 0)
        {
            prev.erase(prev.length() - 1);
            result.emplace_back(symbols.find(prev)->second, toCoded[toCoded.length() - 1]);
        }
    }

    void LZ78E::encode(string inputName, bool binaryOrAscii)
    {
        reset();
        if (binaryOrAscii)
        {
            load(inputName, [&](char c) {
                Byte b(static_cast<unsigned long long>(char2unsign(c)));
                binaryOrig += b.to_string();
            });
            encode(binaryOrig);
            dump([](std::stringstream &ss, char c) {
                ss << c;
            });
        }
        else
        {
            load(inputName, [&](char c) {
                orig += c;
            });
            encode(orig);
            dump([](std::stringstream &ss, char c) {
                ss << Byte(char2unsign(c)).to_string();
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