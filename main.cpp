#include<iostream>
#include<cstring>
using namespace std;


//  Incrementation of a number, handle overflow by incrementing the carry.
void addWithCarry(unsigned int& main, unsigned int& carry, unsigned int addend) {
    if (main > 0xffffffff - addend) {
        ++carry;
    }
    main += addend;
}

// circular right rotation - shift right and wrap the shifted bits on the left.
unsigned int getRightRotation(unsigned int value, unsigned int shift) {
    return ((value >> shift) | (value << (32 - shift)));
}

// 'x' chooses between 'y' or 'z'
// for each '1' ('0') bit in x, get the corresponding bit from y (from z)
unsigned int getChooseBitByBit(unsigned int x, unsigned int y, unsigned int z) {
    return ((x & y) ^ (~x & z));
}

// each result bit is according to the majority of the 3 input bits for x, y and z.
unsigned int getBitwiseMajority(unsigned int x, unsigned int y, unsigned int z) {
    return ((x & y) ^ (x & z) ^ (y & z));
}


int main(){

    return 0;
}
