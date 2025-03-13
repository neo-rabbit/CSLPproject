#ifndef GOLOMBCODE_H
#define GOLOMBCODE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "../Deliverable2/Bitstream.h"

class GolombCoding {
private:
    bool interleaving;

public:
    GolombCoding(bool il);
    void encode(BitStream &bitStream, int n, int m);
    int decode(BitStream &bitStream, int m);
};

#endif