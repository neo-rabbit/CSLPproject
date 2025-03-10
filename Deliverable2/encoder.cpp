#include <iostream>
#include <fstream>
#include "Bitstream.h"

void encodeFile(const std::string& inputFilename, const std::string& outputFilename) {
    std::ifstream inputFile(inputFilename);
    if (!inputFile) {
        std::cerr << "Error: Unable to open input file!" << std::endl;
        return;
    }

    BitStream bitStream(outputFilename, true); // Write mode
    char bitChar;
    while (inputFile.get(bitChar)) {
        if (bitChar == '0') {
            bitStream.writeBit(0);
        } else if (bitChar == '1') {
            bitStream.writeBit(1);
        }
    }

    inputFile.close();
    std::cout << "Encoding complete: " << outputFilename << " generated." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_text_file> <output_binary_file>" << std::endl;
        return 1;
    }

    encodeFile(argv[1], argv[2]);
    return 0;
}
