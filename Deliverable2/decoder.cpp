#include <iostream>
#include <fstream>
#include "BitStream.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }
    BitStream bstream(argv[1]);
    ofstream output_file;
    output_file.open(argv[2],ios_base::app);
    int bit = 0;
    while (bit != -1){
        bit = bstream.readBit();
        if (bit != -1) output_file.put(bit);
    }
    return 0;
}