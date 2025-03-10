#include <iostream>
#include <fstream>
#include <vector>
#include "Bitstream.h"

BitStream::BitStream(const std::string& filename, bool mode) {
    if (mode) {  // true = write mode
        writefile.open(filename, std::ios::binary);
        buffer = 0;
        bitPos = 7;
    } else {  // false = read mode
        readfile.open(filename, std::ios::binary);
        buffer = 0;
        readBitPos = -1; // Indicates that the buffer needs to be loaded
    }
    reading = !mode;
}

BitStream::~BitStream() {
    // Flush the remaining bits to the file before closing
    if (!reading && bitPos < 7) {
        writefile.put(buffer);
    }
    if (writefile.is_open()) writefile.close();
    if (readfile.is_open()) readfile.close();
}

void BitStream::writeBit(bool bit) {
    if (bit) {
        buffer |= (1 << bitPos);
    }
    if (bitPos == 0) {
        writefile.put(buffer);
        buffer = 0;
        bitPos = 7;
    } else {
        bitPos--;
    }
}

int BitStream::readBit() {
    // Load a new byte when all bits in the buffer have been consumed
    if (readBitPos == -1 || readBitPos == 8) {
        if (!readfile.get(reinterpret_cast<char&>(buffer))) {
            return -1; // EOF reached
        }
        readBitPos = 0;
    }
    int bit = (buffer >> (7 - readBitPos)) & 1;
    readBitPos++;
    return bit;
}

void BitStream::writeBits(uint64_t bits, uint8_t N) {
    for (int i = N - 1; i >= 0; i--) {
        writeBit((bits >> i) & 1);
    }
}

uint64_t BitStream::readBits(uint8_t N) {
    uint64_t result = 0;
    for (int i = 0; i < N; i++) {
        int bit = readBit();
        if (bit == -1) return 0;  // EOF or end of data
        result = (result << 1) | bit;
    }
    return result;
}

void BitStream::writeString(const std::string& str) {
    for (char c : str) {
        // Ensure that the character is either '0' or '1', and call writeBit
        if (c == '0') {
            writeBit(0);  // Write bit 0
        } else if (c == '1') {
            writeBit(1);  // Write bit 1
        }
    }
}

std::string BitStream::readString(size_t length) {
    std::string result;
    for (size_t i = 0; i < length; i++) {
        int bit = readBit(); // Read a single bit
        if (bit == -1) {
            return result; // End of file, return the string collected so far
        }
        // Append '0' or '1' to the result string
        result += (bit ? '1' : '0');
    }
    return result; // Return the string of bits
}
