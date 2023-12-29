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

// getting length of a string
int strLen(const char* str) {
    if(!str){
        return 0;
    }

    int len = 0;
    while (str[len] != '\0') {
        len++;
    }

    return len;
}

// Set a block of memory to a specific value
void memSet(unsigned char* ptr, const char value, const int num) {
    if(!ptr){
        return;
    }

    for (int i = 0; i < num; i++) {
        ptr[i] = value;
    }
}

// Formatting to HEX
void intToHex(char* dest, const int value) {
    if(!dest){
        return;
    }

    dest[0] = HEX_CHARS[(value >> 4) & 0xF];
    dest[1] = HEX_CHARS[value & 0xF];
}

void intHashToHexHash(int hash[32], char * dest){
    for (int i = 0; i < 32; i++) {
        intToHex(dest + i * 2, hash[i]);
    }
    dest[64] = '\0';
}

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
        return 0;
    }

    for (int i = 0; i < HASH_LEN; ++i) {
        if (*(hash1 + i) != *(hash2 + i)) {
            return 0;
        }
    }

    return 1;
}

void SHA256Init(unsigned int& datalen, unsigned int bitlen[2], unsigned int state[8]) {
    datalen = 0;
    bitlen[0] = 0;
    bitlen[1] = 0;
    state[0] = INITIAL_HASHES[0];
    state[1] = INITIAL_HASHES[1];
    state[2] = INITIAL_HASHES[2];
    state[3] = INITIAL_HASHES[3];
    state[4] = INITIAL_HASHES[4];
    state[5] = INITIAL_HASHES[5];
    state[6] = INITIAL_HASHES[6];
    state[7] = INITIAL_HASHES[7];
}

void SHA256(char* data, char* dest) {
    unsigned char sha_data[64];
    unsigned int datalen;
    unsigned int bitlen[2];
    unsigned int state[8];
    unsigned char hash[32];

    SHA256Init(datalen, bitlen, state);
    SHA256Update(sha_data, (unsigned char*)data, datalen, bitlen, state, strLen(data));
    SHA256Final(sha_data, datalen, bitlen, state, hash);
    intHashToHexHash(hash, dest);

}

int main (){

    return 0;
}
