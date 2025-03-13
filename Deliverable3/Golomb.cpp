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

int main() {
    string inputFilename = "input.txt";
    string outputFilename = "encoded.bin";
    string decodedFilename = "decoded.txt";
    int m = 5;

    ifstream inputFile(inputFilename);
    if (!inputFile) {
        cerr << "Error opening input.txt!" << endl;
        return 1;
    }

    BitStream bitStream(outputFilename, true);
    GolombCoding golomb(true);

    vector<int> originalNumbers;
    int num;

    while (inputFile >> num) {
        originalNumbers.push_back(num);
        golomb.encode(bitStream, num, m);
    }

    inputFile.close();
    bitStream.~BitStream();

    BitStream bitStreamRead(outputFilename, false);
    ofstream decodedFile(decodedFilename);
    if (!decodedFile) {
        cerr << "Error opening decoded.txt!" << endl;
        return 1;
    }

    cout << "Original -> Decoded\n";
    for (int original : originalNumbers) {
        int decodedValue = golomb.decode(bitStreamRead, m);
        decodedFile << decodedValue << endl;
        cout << original << " -> " << decodedValue << endl;

        if (original != decodedValue) {
            cerr << "Error: mismatch detected!" << endl;
        }
    }

    decodedFile.close();
    return 0;
}
