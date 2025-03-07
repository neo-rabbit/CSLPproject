#ifndef BITSTREAM_CSLP
#define BITSTREAM_CSLP

#include <iostream>
#include <fstream>

class BitStream {
    private:
        std::ifstream readfile;
        std::ofstream writefile;
        int position;
        int padding;
        char nextByte;
    public:
        BitStream(char* file);
        void writeBit(bool bit);
        int readBit();
        void writeBits(uint N, uint bits);
        uint readBits(uint N);
        void writeString(char* str);
        char* readString();
};

#endif