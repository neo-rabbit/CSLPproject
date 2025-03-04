#ifndef BITSTREAM_CSLP
#define BITSTREAM_CSLP

#include <iostream>
#include <fstream>

class BitStream {
    private:
        char* filename;
        int position;
        int padding;
        int filesize;
    public:
        BitStream(char* file);
        void writeBit(bool bit);
        bool readBit();
        void writeBits(uint N, uint bits);
        uint readBits(uint N);
        void writeString(char* str);
        char* readString();
};

#endif