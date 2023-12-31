#include<iostream>
#include <fstream>
using namespace std;

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

unsigned int MAX_FILESIZE_BYTES = 1024;

const unsigned int HASH_LEN = 64;

// saves message of length HASH_LEN (str) to a file (filename)
void saveHashToFile(const char* str, const char* filename) {
    if(!str | !filename){
        return;
    }
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
    if(!fileName | !dest){
        return;
    }
    dest[0] = '\0';
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << fileName << endl;
        return;
    }
    file.read(dest, HASH_LEN);
    dest[HASH_LEN] = '\0';
    file.close();
}



bool readFile(const char* filename, char* dest) {
    if(!dest || !filename){
        return 0;
    }
    dest[0] = '\0';
    ifstream file(filename, ios::ate | ios::binary);
    if (!file.is_open()) {
        return false;
    }
    streamsize fileSize = file.tellg();
    file.seekg(0, ios::beg);
    if (fileSize > MAX_FILESIZE_BYTES) {
        return false;
    }
    file.read(dest, fileSize);
    dest[fileSize] = '\0';
    file.close();
    return true;
}

int main(){
    /*
    const char* myString = "111111This is a 64-symbol string that will be saved to a file.1234567890123456789012345678901234567890123456789012345678901234";

    const char* filename = "output.txt";

    saveHashToFile(myString, filename);


    const char* fileName = "output.txt";
    char dest[HASH_LEN + 1];

    getHashFromFile(fileName, dest);

     cout << "String read from file: " << dest << endl;


    char str1[HASH_LEN+1] = "111111This is a 64-symbol string that will be saved to a file.12";  // Replace with your own data
    char str2[HASH_LEN+1] = "111111This is a 64-symbol string that will be saved to a file.12";  // Replace with your own data

    int result = compareHashes(str1, str2);

    if (result == 1) {
        cout << " are the same.\n";
    } else {
        cout << "are not the same.\n";
    }

    const char* file2name = "1KB.txt";
    char fileContent[MAX_FILESIZE_BYTES+1] = "";
    if (readFile(file2name, fileContent)) {
        cout << "File content:\n" << fileContent << "-end" <<endl;
    }
*/
    return 0;
}
