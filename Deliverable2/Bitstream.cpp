#include <iostream>
#include <fstream>
#include "BitStream.h"

using namespace std;

BitStream::BitStream(char* file){
    readfile.open(file);
    writefile.open(file, ios_base::app);
    position = 0;
    padding = 7;
    nextByte = 0;
}
void BitStream::writeBit(bool bit){
    char byte;
    if (bit) {
        nextByte = nextByte | (1 << padding);
    }
    if (padding == 0){
        padding = 7;
        writefile.put(nextByte);
        nextByte = 0;
    } else {
        padding--;
    }
}
int BitStream::readBit(){
    readfile.seekg(position);
    char curChar;
    if (readfile.get(curChar)){
        curChar = curChar >> padding;
        curChar = curChar & 1;
        if (padding == 0){
            padding = 7;
            position++;
        } else {
            padding--;
        }
        if (curChar == 0) return 0;
        else return 1;
    } else {
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
