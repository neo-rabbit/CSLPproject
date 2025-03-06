#include <iostream>
#include <fstream>
#include "BitStream.h"

using namespace std;

BitStream::BitStream(char* file){
    filename = file;
    position = 0;
    padding = 8;
    nextByte = 0;
}
void BitStream::writeBit(bool bit){
    ofstream writefile;
    writefile.open(filename, ios_base::app);
    char byte;
    if (bit) {
        nextByte = nextByte | (1 << padding - 1);
    }
    if (padding == 1){
        padding = 8;
        writefile.put(nextByte);
        nextByte = 0;
    } else {
        padding--;
    }
    writefile.close();
}
int BitStream::readBit(){
    ifstream readfile(filename);
    readfile.seekg(position);
    char curChar;
    if (readfile.get(curChar)){
        readfile.close();
        curChar = curChar >> padding - 1;
        curChar = curChar & 1;
        if (padding == 1){
            padding = 8;
            position++;
        } else {
            padding--;
        }
        if (curChar == 0) return 0;
        else return 1;
    } else {
        readfile.close();
        return -1;
    }
}
void BitStream::writeBits(uint N, uint bits){
    
}
uint BitStream::readBits(uint N){
    return 0;
}
void BitStream::writeString(char* str){

}
char* BitStream::readString(){
    return "0";
}
