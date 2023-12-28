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

// rotation at 2, 13, 22.
unsigned int expansionPermutation0(unsigned int x) {
    return getRightRotation(x, 2) ^ getRightRotation(x, 13) ^ getRightRotation(x, 22);
}

// rotation at 6, 11, 25.
unsigned int expansionPermutation1(unsigned int x) {
    return getRightRotation(x, 6) ^ getRightRotation(x, 11) ^ getRightRotation(x, 25);
}

// rotation and shifting at 7, 18, 3.
unsigned int sigma0(unsigned int x) {
    return getRightRotation(x, 7) ^ getRightRotation(x, 18) ^ (x >> 3);
}

// rotation and shifting at 17, 19, 10.
unsigned int sigma1(unsigned int x) {
    return getRightRotation(x, 17) ^ getRightRotation(x, 19) ^ (x >> 10);
}

int main(){

    return 0;
}
