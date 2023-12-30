#include<iostream>
#include <fstream>
using namespace std;

const unsigned int ROUND_CONSTANTS[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

const unsigned int INITIAL_HASHES[8] = {
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19

};

const char * HEX_CHARS = "0123456789abcdef";

unsigned int MAX_FILESIZE_BYTES = 1024;

const unsigned int HASH_LEN = 64;

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

// check if hashes match or not
bool compareHashes(const char* hash1, const char* hash2) {
    if(!hash1 | !hash2){
        return false;
    }
    for (int i = 0; i < HASH_LEN; ++i) {
        if (*(hash1 + i) != *(hash2 + i)) {
            return false;
        }
    }
    return 1;
}


// convert the 8 subparts of (4-byte) words into a whole hash
bool subhashesToStr(unsigned int subhashes[8], char* dest){
    if(!dest || !subhashes){
        return false;
    }

    // constant iteration length. can be done without iteration/ single for-iterator
    for (unsigned int bytePos = 0; bytePos < 4; ++bytePos) {
        // for each byte position in a subhashes
        for(unsigned int partIdx = 0; partIdx < 8; ++partIdx){
            // for each subhashes
            //extract byte at position bytePos from subhash
            unsigned int byte = (subhashes[partIdx] >> (24 - bytePos * 8)) & 0x000000ff; // 0-255
            //calculate corresponding idx in dest
            unsigned int charIdx = (bytePos + (4*partIdx)) * 2; // 0-63
            dest[charIdx] = HEX_CHARS[(byte >> 4) & 0xF]; // 0-FF
            dest[charIdx+1] = HEX_CHARS[byte & 0xF]; // 0-FF
        }
    }

    dest[64] = '\0';
    return true;
}
// main SHA function
bool SHA256(const char* input_str, char* dest) {
    if(!dest){
        return false;
    }
    if(!input_str){
        return false;
    }
    // container for each 64-symbol block of the input
    unsigned char dataBuffer[64];
    // keep track of last iterated symbol in block
    unsigned int idxInBuffer = 0;
    // keep track of total bits iterated
    unsigned int bitlen[2] = {0, 0};
    // sub-hashes (8 words of 32 bits)
    unsigned int subhashes[8];
    for(unsigned int partIdx = 0; partIdx < 8; partIdx ++){
        subhashes[partIdx] = INITIAL_HASHES[partIdx];
    }
    if(!SHA256Update(dataBuffer, (const unsigned char*)input_str, idxInBuffer, bitlen, subhashes)){
        return false;
    }
    if(!SHA256Final(dataBuffer, idxInBuffer, bitlen, subhashes)){
       return false;
    }
    if(!subhashesToStr(subhashes, dest)){
        return false;
    }
    return true;
}

int main (){

    return 0;
}
