#include<iostream>
#include<cstring>
using namespace std;


//  perform an addition of two unsigned integers (a and c) with a carry (b) and handle overflow by incrementing the carry.
void dblIntAdd(unsigned int& a, unsigned int& b, unsigned int c) {
    if (a > 0xffffffffu - c) {
        ++b;
    }
    a += c;
}


int main(){

    return 0;
}
