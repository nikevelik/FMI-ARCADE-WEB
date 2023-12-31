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

// hashing (with sha256 algorithm) transformation on the 8 subhashes, based on the data
// data is an array of 64 elements with values 0-256
bool SHA256Transform(const unsigned char* data, unsigned int subhashes[8]) {
    if(!data || !subhashes){
        return false;
    }

    // index of (32-bit) word in message schedule
    // index of character in data input
    // message schedule - expansion of data input
    unsigned int wordIdx, charIdx, messageSchedule[64];
    // values to add to subhashes
    unsigned int subhashIncrement[8];

    // set starting value to currentsubhashes
    for(unsigned int partIdx = 0; partIdx < 8; partIdx ++){
        subhashIncrement[partIdx] = subhashes[partIdx];
    }

    // put the data in the message schedule (64 elements x 8bits go to (the first) 16 elements x 32 bits)
    for (wordIdx = 0, charIdx = 0; wordIdx < 16; wordIdx++, charIdx += 4){
        messageSchedule[wordIdx] = (data[charIdx] << 24) | (data[charIdx + 1] << 16) | (data[charIdx + 2] << 8) | (data[charIdx + 3]);
    }
    // fill up the rest (48) elements with values, based on the first 16.
    for (; wordIdx < 64; wordIdx++){
        messageSchedule[wordIdx] = sigma1(messageSchedule[wordIdx - 2]) + messageSchedule[wordIdx - 7] + sigma0(messageSchedule[wordIdx - 15]) + messageSchedule[wordIdx - 16];
    }
    // calculate values to add to subhashes, based on the 1.previous values, 2. round constants, 3. message schedule
    for (wordIdx = 0; wordIdx < 64; ++wordIdx) {
        unsigned int tmp1 = subhashIncrement[7] + expansionPermutation1(subhashIncrement[4]) + getChooseBitByBit(subhashIncrement[4], subhashIncrement[5], subhashIncrement[6]) + ROUND_CONSTANTS[wordIdx] + messageSchedule[wordIdx];
        unsigned int tmp2 = expansionPermutation0(subhashIncrement[0]) + getBitwiseMajority(subhashIncrement[0], subhashIncrement[1], subhashIncrement[2]);
        subhashIncrement[7] = subhashIncrement[6];
        subhashIncrement[6] = subhashIncrement[5];
        subhashIncrement[5] = subhashIncrement[4];
        subhashIncrement[4] = subhashIncrement[3] + tmp1;
        subhashIncrement[3] = subhashIncrement[2];
        subhashIncrement[2] = subhashIncrement[1];
        subhashIncrement[1] = subhashIncrement[0];
        subhashIncrement[0] = tmp1 + tmp2;
    }

    //update subhashes
    for(unsigned int partIdx = 0; partIdx < 8; partIdx++){
        subhashes[partIdx] += subhashIncrement[partIdx];
    }
    return true;
}

// iterate input string, updating the subhashes & bitlen after every 512 bit block (after every 64 chars of the input)
bool SHA256Update(unsigned char* dataBuffer, const unsigned char* input_str, unsigned int& idxInBuffer, unsigned int bitlen[2], unsigned int subhashes[8]) {
    if(!input_str || !dataBuffer || !bitlen || !subhashes){
        return false;
    }

    // iterate the input string
    for (unsigned int i = 0; input_str[i] != '\0'; ++i) {
        // save current block data into the buffer
        dataBuffer[idxInBuffer] = input_str[i];
        idxInBuffer++;
        // after the 64-char block is iterated (after every 512 bits buffered)
        if (idxInBuffer == 64) {
            // update subhashes based on the block
            if(!SHA256Transform(dataBuffer, subhashes)){
                return false;
            }
            // update bitlen
            addWithCarry(bitlen[0], bitlen[1], 512);
            // start new block
            idxInBuffer = 0;
        }
    }
    return true;
}

// include bias in the hash, based on the input length, updating the sub-hashes
bool SHA256Final(unsigned char* dataBuffer, unsigned int idxInBuffer, unsigned int bitlen[2], unsigned int subhashes[8]) {
    if(!dataBuffer || !bitlen || !subhashes){
        return false;
    }
    unsigned int i = idxInBuffer;

    // bitlen is 64 bit array (8 bytes). it needs 8 bytes of space in the dataBuffer for it to be added in a transformation
    // make space for bitlen:
    if (idxInBuffer < 56) {
        // already has space. do padding until there are exactly 8 spots
        dataBuffer[i++] = 0x80;

        while (i < 56){
            dataBuffer[i++] = 0x00;
        }
    } else {
        // not enough space. perform transformation first
        dataBuffer[i++] = 0x80;

        while (i < 64){
            dataBuffer[i++] = 0x00;
        }

        if(!SHA256Transform(dataBuffer, subhashes)){
            return false;
        }

        // transformation done. do padding until there are exactly 8 spots
        for(i = 0; i < 56; i++){
            dataBuffer[i] = 0;
        }
    }

    // increment bitlen for the last (partial block)
    addWithCarry(bitlen[0], bitlen[1], idxInBuffer * 8);
    //add bitlen to the next transformation
    dataBuffer[63] = bitlen[0];
    dataBuffer[62] = bitlen[0] >> 8;
    dataBuffer[61] = bitlen[0] >> 16;
    dataBuffer[60] = bitlen[0] >> 24;
    dataBuffer[59] = bitlen[1];
    dataBuffer[58] = bitlen[1] >> 8;
    dataBuffer[57] = bitlen[1] >> 16;
    dataBuffer[56] = bitlen[1] >> 24;
    if(!SHA256Transform(dataBuffer, subhashes)){
        return false;
    }
    return true;
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
