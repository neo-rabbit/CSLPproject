#include <iostream>
#include <vector>
#include <cmath>
#include "Golomb.h"
#include "../Deliverable2/Bitstream.h"

using namespace std;

GolombCoding::GolombCoding(bool il) {
    interleaving = il;
}

void GolombCoding::encode(BitStream &bitStream, int n, int m) {
    int N;
    if (interleaving) {
        // Zigzag interleaving
        N = (n >= 0) ? (n * 2) : (-n * 2 - 1);
    } else {
        // Sign and magnitude
        N = abs(n);
        bitStream.writeBit(n < 0); // Write sign bit (0 for positive, 1 for negative)
    }

    int q = N / m;
    int r = N % m;

    // Unary encoding of q (q ones followed by a zero)
    for (int i = 0; i < q; i++) {
        bitStream.writeBit(1);
    }

    bitStream.writeBit(0);

    // Optimal binary encoding of r
    int b = ceil(log2(m));
    if (r < (1 << b) - m) {
        bitStream.writeBits(r, b - 1); // Use b-1 bits
    } else {
        bitStream.writeBits(r + ((1 << b) - m), b); // Use b bits
    }
}

int GolombCoding::decode(BitStream &bitStream, int m) {
    int q = 0;
    while (bitStream.readBit() == 1) {
        q++;
    }

    int b = ceil(log2(m));
    int threshold = (1 << b) - m;
    int r = bitStream.readBits(b - 1);
    if (r >= threshold) {
        r = (r << 1) | bitStream.readBit();
        r -= threshold;
    }

    int N = q * m + r;
    if (interleaving) {
        return (N % 2 == 0) ? (N / 2) : -(N / 2) - 1;
    } else {
        return bitStream.readBit() ? -N : N; // Read sign bit
    }
}