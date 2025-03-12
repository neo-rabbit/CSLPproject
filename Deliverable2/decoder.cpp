#include <iostream>
#include <fstream>
#include "Bitstream.h"

void decodeFile(const std::string& inputFilename, const std::string& outputFilename) {
    std::ofstream outputFile(outputFilename);
    if (!outputFile) {
        std::cerr << "Error: Unable to open output file!" << std::endl;
        return;
    }

    BitStream bitStream(inputFilename, false); // Read mode
    int bit;
    while ((bit = bitStream.readBit()) != -1) {
        outputFile.put(bit ? '1' : '0');
    }

    outputFile.close();
    std::cout << "Decoding complete: " << outputFilename << " generated." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_binary_file> <output_text_file>" << std::endl;
        return 1;
    }

    decodeFile(argv[1], argv[2]);
    return 0;
}
