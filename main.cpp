#include<iostream>
#include <fstream>
using namespace std;

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
void memSet(unsigned char* ptr, char value, int num) {
    if(!ptr){
        return;
    }
    for (int i = 0; i < num; i++) {
        ptr[i] = value;
    }
}

// Format a string
void sprintF(char* dest, int value) {
    if(!dest){
        return;
    }
    char hexChars[] = "0123456789abcdef";
    dest[0] = hexChars[(value >> 4) & 0xF];
    dest[1] = hexChars[value & 0xF];
}

// Concatenate strings
void strCat(char* dest, const char* src) {
    if(!dest || !src){
        return;
    }
    int destLen = strLen(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[destLen + i] = src[i];
        i++;
    }
    dest[destLen + i] = '\0';
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

const unsigned int roundConstants[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

const unsigned int HASH_LEN = 64;

// saves message of length HASH_LEN (str) to a file (filename)
void saveHashToFile(const char* str, const char* filename) {
    ofstream outFile(filename, ios::binary);

    if (!outFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    outFile.write(str, HASH_LEN);
    outFile.close();
    cout << "String saved to file: " << filename << endl;
}


// gets message from file and saves it to dest.
void getHashFromFile(const char* fileName, char* dest) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << fileName << std::endl;
        return;
    }
    file.read(dest, HASH_LEN);
    dest[HASH_LEN] = '\0';
    file.close();
}

// check if hashes match or not
bool compareHashes(const char* hash1, const char* hash2) {
    for (int i = 0; i < HASH_LEN; ++i) {
        if (*(hash1 + i) != *(hash2 + i)) {
            return 0;
        }
    }
    return 1;
}

int main(){

    ///const char* myString = "111111This is a 64-symbol string that will be saved to a file.1234567890123456789012345678901234567890123456789012345678901234";

    ///const char* filename = "output.txt";

    ///saveHashToFile(myString, filename);


    ///const char* fileName = "output.txt";
    ///char dest[HASH_LEN + 1];

    ///getHashFromFile(fileName, dest);

    /// std::cout << "String read from file: " << dest << std::endl;


    ///char str1[HASH_LEN+1] = "111111This is a 64-symbol string that will be saved to a file.12";  // Replace with your own data
    ///char str2[HASH_LEN+1] = "11111 1This is a 64-symbol string that will be savd to a file.12";  // Replace with your own data

    ///int result = compareHashes(str1, str2);

    ///if (result == 1) {
       /// std::cout << " are the same.\n";
    ///} else {
     ///   std::cout << "are not the same.\n";
    ///}


    return 0;
}
