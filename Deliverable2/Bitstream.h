#ifndef BITSTREAM_CSLP
#define BITSTREAM_CSLP

#include <iostream>

class BitStream {
    private:
        char* filename;
        int position;
        int padding;
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