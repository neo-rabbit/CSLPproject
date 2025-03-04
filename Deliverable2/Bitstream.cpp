#include <iostream>
#include "BitStream.h"

BitStream::BitStream(char* file){
    filename = file;
    position = 0;
    padding = 0;
}
void BitStream::writeBit(bool bit){

}
bool BitStream::readBit(){
    return 0;
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
