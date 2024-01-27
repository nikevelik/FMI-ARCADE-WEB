/**
*
* Solution to course project #06
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author <Nikola Georgiev>
* @idnumber <4MI0600288>
* @compiler GCC
*
* Implementation of SHA256 algorithm in C++ for hashing strings and files
*
*/

#include <iostream>
#include <fstream>
using namespace std;

// Constants for better readability
const unsigned int BLOCK_SIZE = 64;
const unsigned int WORD_SIZE = 8;
const unsigned int BITS_IN_BYTE = 8; // = BITS_IN_CHAR
const unsigned int BITS_IN_INT = 32;
const unsigned int MESSAGE_SCHEDULE_SIZE = 64;
const unsigned int HASH_SIZE = 64;
const unsigned int BITS_IN_BLOCK = 512;
const unsigned int SUBHASHES_NUMBER = 8;
const unsigned int BITLEN_ARR_SIZE = 2;
const unsigned int FINAL_PADDING_THRESHOLD = 56;
const unsigned int MAX_UNSIGNED_INT = 0xffffffff;
const unsigned int MAX_UNSIGNED_CHAR = 0x000000ff;
const unsigned int PADDING_VALUE = 0x80;
const unsigned int MESSAGE_COMPRESSION_RATE = 16;
const unsigned int BYTES_IN_INT = 4;
const unsigned int MAX_BYTEPOS_IN_INT = 24;
const char *HEX_CHARS = "0123456789abcdef";


const unsigned int ROUND_CONSTANTS[MESSAGE_SCHEDULE_SIZE] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

const unsigned int INITIAL_HASHES[SUBHASHES_NUMBER] = {
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19

};



//  Incrementation of a number, handle overflow by incrementing the carry.
void addWithCarry(unsigned int &main, unsigned int &carry, unsigned int addend)
{
    if (main > MAX_UNSIGNED_INT - addend)
    {
        ++carry;
    }
    main += addend;
}

// circular right rotation - shift right and wrap the shifted bits on the left.
unsigned int getRightRotation(unsigned int value, unsigned int shift)
{
    return ((value >> shift) | (value << (BITS_IN_INT - shift)));
}

// 'x' chooses between 'y' or 'z'
// for each '1' ('0') bit in x, get the corresponding bit from y (from z)
unsigned int getChooseBitByBit(unsigned int x, unsigned int y, unsigned int z)
{
    return ((x & y) ^ (~x & z));
}

// each result bit is according to the majority of the 3 input bits for x, y and z.
unsigned int getBitwiseMajority(unsigned int x, unsigned int y, unsigned int z)
{
    return ((x & y) ^ (x & z) ^ (y & z));
}

// rotation at 2, 13, 22.
unsigned int bigSigma0(unsigned int x)
{
    return getRightRotation(x, 2) ^ getRightRotation(x, 13) ^ getRightRotation(x, 22);
}

// rotation at 6, 11, 25.
unsigned int bigSigma1(unsigned int x)
{
    return getRightRotation(x, 6) ^ getRightRotation(x, 11) ^ getRightRotation(x, 25);
}

// rotation and shifting at 7, 18, 3.
unsigned int smallSigma0(unsigned int x)
{
    return getRightRotation(x, 7) ^ getRightRotation(x, 18) ^ (x >> 3);
}

// rotation and shifting at 17, 19, 10.
unsigned int smallSigma1(unsigned int x)
{
    return getRightRotation(x, 17) ^ getRightRotation(x, 19) ^ (x >> 10);
}

// Helper function to calculate tmp1 value in SHA256Transform
unsigned int calculateTmp1(unsigned int subhashIncrement[8], unsigned int wordIdx, const unsigned int messageSchedule[64]) {
    return subhashIncrement[7] + bigSigma1(subhashIncrement[4])
        + getChooseBitByBit(subhashIncrement[4], subhashIncrement[5], subhashIncrement[6])
        + ROUND_CONSTANTS[wordIdx]
        + messageSchedule[wordIdx];
}

// Helper function to calculate tmp2 value in SHA256Transform
unsigned int calculateTmp2(unsigned int subhashIncrement[8]) {
    return bigSigma0(subhashIncrement[0])
        + getBitwiseMajority(subhashIncrement[0], subhashIncrement[1], subhashIncrement[2]);
}

// check if hashes match or not
bool compareHashes(const char *hash1, const char *hash2)
{
    if (!hash1 | !hash2)
    {
        return false;
    }
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        if (*(hash1 + i) != *(hash2 + i))
        {
            return false;
        }
    }
    return true;
}

// hashing (with sha256 algorithm) transformation on the 8 subhashes, based on the data
// data is an array (block) of 64 elements with values 0-256
bool SHA256Transform(const unsigned char *data,
                    unsigned int subhashes[SUBHASHES_NUMBER])
{
    if (!data || !subhashes)
    {
        return false;
    }

    // index of (32-bit) word in message schedule
    // index of character in data input
    // message schedule - expansion of data input
    // values to add to subhashes
    unsigned int wordIdx, charIdx, messageSchedule[MESSAGE_SCHEDULE_SIZE], subhashIncrement[SUBHASHES_NUMBER];

    // set starting value to currentsubhashes
    for (unsigned int partIdx = 0; partIdx < SUBHASHES_NUMBER; partIdx++)
    {
        subhashIncrement[partIdx] = subhashes[partIdx];
    }

    // put the data in the message schedule (considering types' size)
    for (wordIdx = 0, charIdx = 0; wordIdx < MESSAGE_COMPRESSION_RATE; wordIdx++, charIdx += 4)
    {
        messageSchedule[wordIdx] = (data[charIdx] << (3 * BITS_IN_BYTE))
                                    | (data[charIdx + 1] << (2 * BITS_IN_BYTE))
                                    | (data[charIdx + 2] << BITS_IN_BYTE)
                                    | (data[charIdx + 3]);
    }
    // fill up the rest (48) elements with values, based on the first 16.
    for (; wordIdx < MESSAGE_SCHEDULE_SIZE; wordIdx++)
    {
        messageSchedule[wordIdx] = smallSigma1(messageSchedule[wordIdx - 2])
                                    + messageSchedule[wordIdx - 7]
                                    + smallSigma0(messageSchedule[wordIdx - 15])
                                    + messageSchedule[wordIdx - 16];
    }
    // calculate values to add to subhashes, based on the 1.previous values, 2. round constants, 3. message schedule
    for (wordIdx = 0; wordIdx < MESSAGE_SCHEDULE_SIZE; ++wordIdx)
    {
        unsigned int tmp1 = calculateTmp1(subhashIncrement, wordIdx, messageSchedule);
        unsigned int tmp2 = calculateTmp2(subhashIncrement);
        subhashIncrement[7] = subhashIncrement[6];
        subhashIncrement[6] = subhashIncrement[5];
        subhashIncrement[5] = subhashIncrement[4];
        subhashIncrement[4] = subhashIncrement[3] + tmp1;
        subhashIncrement[3] = subhashIncrement[2];
        subhashIncrement[2] = subhashIncrement[1];
        subhashIncrement[1] = subhashIncrement[0];
        subhashIncrement[0] = tmp1 + tmp2;
    }

    // update subhashes
    for (unsigned int partIdx = 0; partIdx < SUBHASHES_NUMBER; partIdx++)
    {
        subhashes[partIdx] += subhashIncrement[partIdx];
    }
    return true;
}

// update subhashes and bitlen
bool SHA256Step(unsigned char *dataBuffer,
                unsigned int bitlen[BITLEN_ARR_SIZE],
                unsigned int subhashes[SUBHASHES_NUMBER])
{
    // update subhashes based on the block
    if (!SHA256Transform(dataBuffer, subhashes))
    {
        return false;
    }
    // update bitlen
    addWithCarry(bitlen[0], bitlen[1], BITS_IN_BLOCK);
    return true;
}

// iterate input string, updating the subhashes & bitlen after every 512 bit block (after every 64 chars of the input)
bool SHA256Update(unsigned char *dataBuffer,
                    const unsigned char *input_str,
                    unsigned int &idxInBuffer,
                    unsigned int bitlen[BITLEN_ARR_SIZE],
                    unsigned int subhashes[SUBHASHES_NUMBER])
{
    if (!input_str || !dataBuffer || !bitlen || !subhashes)
    {
        return false;
    }

    // iterate the input string
    for (unsigned int i = 0; input_str[i] != '\0'; ++i)
    {
        // save current block data into the buffer
        dataBuffer[idxInBuffer] = input_str[i];
        idxInBuffer++;
        // after the 64-char block is iterated (after every 512 bits buffered)
        if (idxInBuffer == BLOCK_SIZE)
        {
            if (!SHA256Step(dataBuffer, bitlen, subhashes))
            {
                return false;
            }
            // start new block
            idxInBuffer = 0;
        }
    }
    return true;
}

// iterate input string, updating the subhashes & bitlen after every 512 bit block (after every 64 chars of the file)
bool SHA256FileUpdate(unsigned char *dataBuffer,
                        const char *file,
                        unsigned int &idxInBuffer,
                        unsigned int bitlen[BITLEN_ARR_SIZE],
                        unsigned int subhashes[SUBHASHES_NUMBER])
{
    if (!file || !dataBuffer || !bitlen || !subhashes)
    {
        return false;
    }

    {
        ifstream inFile;

        inFile.open(file);
        if (!inFile.is_open())
        {
            return false;
        }

        // while we can read full 64-char block from the file
        while (inFile.read((char *)dataBuffer, BLOCK_SIZE))
        {
            // update subhashes and bitlen
            if (!SHA256Step(dataBuffer, bitlen, subhashes))
            {
                return false;
            }
        }

        // remember how many chars are in the incomplete last block
        idxInBuffer = inFile.gcount();

        inFile.close();
    }
    return true;
}

// include bias in the hash, based on the input length, updating the sub-hashes
bool SHA256Final(unsigned char *dataBuffer,
                unsigned int idxInBuffer,
                unsigned int bitlen[BITLEN_ARR_SIZE],
                unsigned int subhashes[SUBHASHES_NUMBER])
{
    if (!dataBuffer || !bitlen || !subhashes)
    {
        return false;
    }
    unsigned int i = idxInBuffer;

    // bitlen is 64 bit array (8 bytes). it needs 8 bytes of space in the dataBuffer for it to be added in a transformation
    // make space for bitlen:
    if (idxInBuffer < FINAL_PADDING_THRESHOLD)
    {
        // already has space. do padding until there are exactly 8 spots
        dataBuffer[i++] = PADDING_VALUE;

        while (i < FINAL_PADDING_THRESHOLD)
        {
            dataBuffer[i++] = 0x00;
        }
    }
    else
    {
        // not enough space. perform transformation first
        dataBuffer[i++] = PADDING_VALUE;

        while (i < BLOCK_SIZE)
        {
            dataBuffer[i++] = 0x00;
        }

        if (!SHA256Transform(dataBuffer, subhashes))
        {
            return false;
        }

        // transformation done. do padding until there are exactly 8 spots
        for (i = 0; i < FINAL_PADDING_THRESHOLD; i++)
        {
            dataBuffer[i] = 0;
        }
    }

    // increment bitlen for the last (partial block)
    addWithCarry(bitlen[0], bitlen[1], idxInBuffer * 8);
    // add bitlen to the next transformation
    dataBuffer[63] = bitlen[0];
    dataBuffer[62] = bitlen[0] >> BITS_IN_BYTE;
    dataBuffer[61] = bitlen[0] >> (BITS_IN_BYTE*2);
    dataBuffer[60] = bitlen[0] >> (BITS_IN_BYTE*3);
    dataBuffer[59] = bitlen[1];
    dataBuffer[58] = bitlen[1] >> BITS_IN_BYTE;
    dataBuffer[57] = bitlen[1] >> (BITS_IN_BYTE*2);
    dataBuffer[56] = bitlen[1] >> (BITS_IN_BYTE*3);
    if (!SHA256Transform(dataBuffer, subhashes))
    {
        return false;
    }
    return true;
}

// convert the 8 subparts of (4-byte) words into a whole hash
bool subhashesToStr(unsigned int subhashes[SUBHASHES_NUMBER], char *dest)
{
    if (!dest || !subhashes)
    {
        return false;
    }

    // constant iteration length. can be done without iteration/ single for-iterator
    for (unsigned int bytePos = 0; bytePos < BYTES_IN_INT; ++bytePos)
    {
        // for each byte position in a subhashes
        for (unsigned int partIdx = 0; partIdx < SUBHASHES_NUMBER; ++partIdx)
        {
            // for each subhashes, convert a given byte to HEX representation
            // extract byte at position bytePos from subhash
            unsigned char byte = (subhashes[partIdx] >> (MAX_BYTEPOS_IN_INT - bytePos * BITS_IN_BYTE)) & MAX_UNSIGNED_CHAR; // 0-255
            // calculate corresponding idx in dest
            unsigned int offset = BYTES_IN_INT * partIdx;
            unsigned int charIdx = (bytePos + offset) << 1;       // 0-63
            dest[charIdx] = HEX_CHARS[(byte >> 4) & 0xF];         // 0-FF
            dest[charIdx + 1] = HEX_CHARS[byte & 0xF];            // 0-FF
        }
    }

    dest[HASH_SIZE] = '\0';
    return true;
}

// main SHA function
bool SHA256(const char *input_str, char *dest)
{
    if (!dest)
    {
        return false;
    }
    if (!input_str)
    {
        return false;
    }
    // container for each 64-symbol block of the input
    unsigned char dataBuffer[BLOCK_SIZE];
    // keep track of last iterated symbol in block
    unsigned int idxInBuffer = 0;
    // keep track of total bits iterated
    unsigned int bitlen[BITLEN_ARR_SIZE] = {0, 0};
    // sub-hashes (8 words of 32 bits)
    unsigned int subhashes[SUBHASHES_NUMBER];
    for (unsigned int partIdx = 0; partIdx < SUBHASHES_NUMBER; partIdx++)
    {
        subhashes[partIdx] = INITIAL_HASHES[partIdx];
    }
    if (!SHA256Update(dataBuffer, (const unsigned char *)input_str, idxInBuffer, bitlen, subhashes))
    {
        return false;
    }
    if (!SHA256Final(dataBuffer, idxInBuffer, bitlen, subhashes))
    {
        return false;
    }
    if (!subhashesToStr(subhashes, dest))
    {
        return false;
    }
    return true;
}

// saves message of length HASH_SIZE (hash_str) to a file (file)
bool saveHashToFile(const char *hash_str, const char *file)
{
    if (!hash_str || !file)
    {
        return false;
    }
    {
        ofstream outFile;

        outFile.open(file);
        if (!outFile.is_open())
        {
            return false;
        }

        outFile.write(hash_str, HASH_SIZE);
        if (!outFile.good())
        {
            outFile.close();
            return false;
        }

        outFile.close();
    }

    return true;
}

// gets hash message (first HASH_SIZE symbols) from file and saves it to dest.
bool getHashFromFile(const char *file, char *dest)
{
    if (!file || !dest)
    {
        return false;
    }
    {
        ifstream inFile;
        inFile.open(file);
        if (!inFile.is_open())
        {
            return false;
        }

        inFile.read(dest, HASH_SIZE);
        if (!inFile.good())
        {
            inFile.close();
            return false;
        }

        if (inFile.gcount() < HASH_SIZE)
        {
            inFile.close();
            return false;
        }

        dest[HASH_SIZE] = '\0';
        inFile.close();
    }
    return true;
}

// main function for file
bool SHA256File(const char *file, char *dest)
{

    if (!dest)
    {
        return false;
    }

    if (!file)
    {
        dest = nullptr;
        return false;
    }

    // container for each 64-symbol block of the input
    unsigned char dataBuffer[BLOCK_SIZE];
    // keep track of last iterated symbol in block
    unsigned int idxInBuffer = 0;
    // keep track of total bits iterated
    unsigned int bitlen[BITLEN_ARR_SIZE] = {0, 0};
    // sub-hashes (8 words of 32 bits)
    unsigned int subhashes[SUBHASHES_NUMBER];

    // initialise subparts
    for (unsigned int partIdx = 0; partIdx < SUBHASHES_NUMBER; partIdx++)
    {
        subhashes[partIdx] = INITIAL_HASHES[partIdx];
    }
    if (!SHA256FileUpdate(dataBuffer, file, idxInBuffer, bitlen, subhashes))
    {
        return false;
    }
    if (!SHA256Final(dataBuffer, idxInBuffer, bitlen, subhashes))
    {
        return false;
    }
    if (!subhashesToStr(subhashes, dest))
    {
        return false;
    }
    return true;
}

int main()
{
    char hashedString[HASH_SIZE + 1],
         loadedHash[HASH_SIZE + 1],
         hashedFile[HASH_SIZE + 1],
         hashedStringLikeFile[HASH_SIZE + 1];

    const char *inputString = "1";
    const char *inputFile = "example.txt";
    const char *outputFile = "hash1.txt";
    const char *inputStringLikeFile = "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF";

    // Hash input string
    if (SHA256(inputString, hashedString))
    {
        cout << "SHA256('" << inputString << "') -> " << hashedString << "\n";
    }
    else
    {
        cerr << "Error hashing the input string.\n";
        return 1;
    }

    // Save hash to file
    if (saveHashToFile(hashedString, outputFile))
    {
        cout << "Hash '" << hashedString << "' saved to file " << outputFile << "\n";
    }
    else
    {
        cerr << "Error saving hash to file.\n";
        return 1;
    }

    // Load hash from file
    if (getHashFromFile(outputFile, loadedHash))
    {
        cout << "Hash '" << loadedHash << "' read from file " << outputFile << "\n";
    }
    else
    {
        cerr << "Error reading hash from file.\n";
        return 1;
    }

    // Hash input file
    if (SHA256File(inputFile, hashedFile))
    {
        cout << "SHA256('" << inputFile << "') -> " << hashedFile << "\n";
    }
    else
    {
        cerr << "Error hashing the input file.\n";
    }

    // Hash input string like a file
    if (SHA256(inputStringLikeFile, hashedStringLikeFile))
    {
        cout << "SHA256('" << inputStringLikeFile << "') -> " << hashedStringLikeFile << "\n";
    }
    else
    {
        cerr << "Error hashing the input string.\n";
        return 1;
    }

    // Compare hashes
    if (compareHashes(loadedHash, hashedString))
    {
        cout << "Hashes:\n" << loadedHash << " and\n" << hashedString << "\nmatch!\n";
    }
    else
    {
        cout << "Hashes:\n" << loadedHash << " and\n" << hashedString << "\ndo not match.\n";
    }

    if (compareHashes(hashedFile, hashedStringLikeFile))
    {
        cout << "Hashes:\n" << hashedFile << " and\n" << hashedStringLikeFile << "\nmatch!\n";
    }
    else
    {
        cout << "Hashes:\n" << hashedFile << " and\n" << hashedStringLikeFile << "\ndo not match.\n";
    }

    if (compareHashes(hashedFile, hashedString))
    {
        cout << "Hashes:\n" << hashedFile << " and\n" << hashedString << "\nmatch!\n";
    }
    else
    {
        cout << "Hashes:\n" << hashedFile << " and\n" << hashedString << "\ndo not match.\n";
    }

    return 0;
}

