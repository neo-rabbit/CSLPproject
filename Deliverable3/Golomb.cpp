#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "Golomb.h"
#include "../Deliverable2/Bitstream.h"

using namespace std;

GolombCoding::GolombCoding(bool il){
    interleaving = il;
}

vector<bool> GolombCoding::encode(int n, int m){
    vector<bool> res = {};
    int q = floor(abs(n)/m);
    int r = abs(n)%m;
    if (interleaving){
    } else {
        // Represent signal by sign and magnitude
        if (n>=0){
            res.push_back(0);
        } else {
            res.push_back(1);
        }
        // Represnt q by its unary code
        for(int i=0; i<q; i++){
            res.push_back(1);
        }
        res.push_back(0);
        // Represent r by its binary code
        if (m%2 == 0){
            for (int i = m/2 - 1; i >= 0; i--) {
                res.push_back((r >> i) & 1);
            }
        } else {
            int b = ceil(log2(m));
            if (r < exp2(b)-m){
                for (int i = b - 2; i >= 0; i--) {
                    res.push_back((r >> i) & 1);
                }
            } else {
                for (int i = b - 1; i >= 0; i--) {
                    res.push_back(((r+int(exp2(b))-m) >> i) & 1);
                }
            }
        }
    }
    return res;
}

int GolombCoding::decode(vector<bool> bits){
    return 0;
}

int main(int argc, char** argv){
    GolombCoding golomb(false);
    for(int i = -4; i < 16; i++){
        vector<bool> v = golomb.encode(i, 5);
        for(int j=0; j<v.size(); j++)
            cout << v[j];
        cout << endl;
    }
    return 0;
}