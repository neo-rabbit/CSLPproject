#include <iostream>
#include "BitStream.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }
    BitStream bstream(argv[2]);
    return 0;
}