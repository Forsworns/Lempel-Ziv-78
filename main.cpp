#include <iostream>
#include <chrono>
#include "includes/lz78e.hpp"
#include "includes/lz78d.hpp"

using namespace std;
using namespace yph;

int main(int argc, char *argv[])
{
    string inputName = "./78c++/data/text.txt";
    string dictionarySize = "1240000";
    bool binaryOrAscii = true;
    switch (argc)
    {
    case 4: // non-break to adopt the remaining configs
        binaryOrAscii = false;
    case 3:
        dictionarySize = argv[2];
    case 2:
        inputName = argv[1];
        break;
    }

    auto t1 = chrono::steady_clock::now();
    LZ78E encoder(stoi(dictionarySize));
    encoder.encode(inputName, binaryOrAscii);
    auto t2 = chrono::steady_clock::now();
    auto encode_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << encode_span.count() << endl;

    t1 = chrono::steady_clock::now();
    LZ78D decoder;
    decoder.decode(encoder.getOutputName(), encoder.getInputSuffix(), binaryOrAscii);
    t2 = chrono::steady_clock::now();
    auto decode_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << decode_span.count() << endl;

    system("pause");
    return 0;
}
