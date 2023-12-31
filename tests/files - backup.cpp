#include<iostream>
#include <fstream>
using namespace std;

unsigned int MAX_FILESIZE_BYTES = 1024;

const unsigned int HASH_LEN = 64;

// saves message of length HASH_LEN (hash_str) to a file (file)
bool saveHashToFile(const char* hash_str, const char* file) {
    if(!hash_str || !file){
        return false;
    }
    {
        ofstream outFile;

        outFile.open(file);
        if (!outFile.is_open()) {
            return false;
        }

        outFile.write(hash_str, HASH_LEN);
        if (!outFile.good()) {
            outFile.close();
            return false;
        }

        outFile.close();
    }

    return true;
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



bool readFile(const char* file, char* dest) {
    if(!dest || !file){
        return 0;
    }
    dest[0] = '\0';
    ifstream inFile(file, ios::ate | ios::binary);
    if (!inFile.is_open()) {
        return false;
    }
    streamsize fileSize = inFile.tellg();
    inFile.seekg(0, ios::beg);
    if (fileSize > MAX_FILESIZE_BYTES) {
        return false;
    }
    inFile.read(dest, fileSize);
    dest[fileSize] = '\0';
    inFile.close();
    return true;
}

int main(){

    const char* myString = "111111This is a 64-symbol string that will be saved to a file.1234567890123456789012345678901234567890123456789012345678901234";

    const char* file = "hash1.txt";

    char loadedHash[HASH_LEN];

    if(saveHashToFile(myString, file)){
        cout << "success";
    }else{
        cout << "error somewhere";
    }

    if(getHashFromFile(file, loadedHash)){
        cout << "String read from file: " << loadedHash << endl;

    }else{
        cout << "Error somewhere";
    }

/*
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
