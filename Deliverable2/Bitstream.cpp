#include <iostream>
#include <fstream>
#include "BitStream.h"

using namespace std;

BitStream::BitStream(char* file){
    filename = file;
    position = 0;
    padding = 8;
}
void BitStream::writeBit(bool bit){

}
bool BitStream::readBit(){
    ifstream readfile(filename);
    readfile.seekg(position);
    char curChar = readfile.get();
    readfile.close();
    if (padding == 1){
        padding = 8;
        position++;
    } else {
        padding--;
    }
    curChar = curChar >> padding - 1;
    curChar = curChar & 1;
    if (curChar == 0) return 0;
    else return 1;
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
