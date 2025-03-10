#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <iostream>
#include <fstream>
#include <vector>

class BitStream {
private:
    std::ifstream readfile;  // Input file stream for reading
    std::ofstream writefile; // Output file stream for writing
    uint8_t buffer;          // Buffer to store temporary bits
    int bitPos;              // Position of the next bit to be written (0-7)
    int readBitPos;          // Position of the next bit to be read (0-7)
    bool reading;            // Indicates if the BitStream is in read mode

public:
    // Constructor: Opens the file in either read or write mode
    BitStream(const std::string& filename, bool mode);

    // Destructor: Closes the file and flushes the buffer if needed
    ~BitStream();

    // Writes a single bit to the file
    void writeBit(bool bit);

    // Reads a single bit from the file
    int readBit();

    // Writes an integer using N bits (0 < N < 64)
    void writeBits(uint64_t bits, uint8_t N);

    // Reads an integer using N bits (0 < N < 64)
    uint64_t readBits(uint8_t N);

    // Writes a string as a series of bits
    void writeString(const std::string& str);

    // Reads a string of given length from the file as a series of bits
    std::string readString(size_t length);
};

#endif // BITSTREAM_H
