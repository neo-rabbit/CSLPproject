#include <iostream>
#include <fstream>
#include <vector>
#include "Bitstream.h"

void encodeFile(const std::string& inputFilename, const std::string& outputFilename) {
    std::ifstream inputFile(inputFilename);
    if (!inputFile) {
        std::cerr << "Error: Unable to open input file!" << std::endl;
        return;
    }

    BitStream bitStream(outputFilename, true); // Write mode
    std::string bitString;  // To accumulate the bits
    char bitChar;

    // Read the input file character by character and store the '0' and '1' into bitString
    while (inputFile.get(bitChar)) {
        if (bitChar == '0') {
            bitString += '0';
        } else if (bitChar == '1') {
            bitString += '1';
        }
    }

    // Handle the case where the number of bits is not a multiple of 8 by adding padding
    size_t bitCount = bitString.size();
    while (bitCount % 8 != 0) {
        bitString += '0';  // Add padding '0' to make the bit count a multiple of 8
        bitCount++;
    }

    // Write the bitString to the output binary file using writeString
    bitStream.writeString(bitString);

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
