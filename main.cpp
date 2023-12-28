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

int main(){

    return 0;
}
