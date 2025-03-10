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
    size_t length = 8; // Let's read strings in 8-bit chunks (1 byte)

    while (true) {
        // Read 8 bits (1 byte) at a time
        std::string decodedStr = bitStream.readString(length);
        if (decodedStr.empty()) {
            break; // Stop when we reach the end of the file
        }

        // Convert the decoded bits into characters and write to the output file
        outputFile << decodedStr;
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
