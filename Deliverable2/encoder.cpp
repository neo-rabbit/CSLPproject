#include <iostream>
#include "BitStream.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }
    ifstream input_file(argv[1]);
    BitStream bstream(argv[2]);
    char bitChar;
    bool bit;
    while (input_file.get(bitChar)){
        if (bitChar == '1'){
            bit = 1;
        } else if (bitChar == '0'){
            bit = 0;
        } else {
            cout << endl;
            cerr << "Invalid input file" << endl;
            return 1;
        }
        bstream.writeBit(bit);
    }
    return 0;
}