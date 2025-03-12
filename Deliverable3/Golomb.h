#ifndef GOLOMBCODE_H
#define GOLOMBCODE_H

#include <iostream>
#include <fstream>
#include <vector>

class GolombCoding {
private:
    bool interleaving;

public:
    GolombCoding(bool il);
    ~GolombCoding();
    std::vector<bool> encode(int n);
    int decode(std::vector<bool> bits);
};

#endif