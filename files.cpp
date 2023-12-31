#include<iostream>
#include <fstream>
using namespace std;

unsigned int MAX_FILESIZE_BYTES = 1024;

const unsigned int HASH_LEN = 64;



bool readFile(const char* file, char* dest) {
    if(!dest || !file){
        return 0;
    }
    {
        ifstream inFile;

        inFile.open(file, ios::ate | ios::binary);
        if (!inFile.is_open()) {
            return false;
        }

        streamsize fileSize = inFile.tellg();
        if (fileSize > MAX_FILESIZE_BYTES) {
            return false;
        }

        inFile.seekg(0, ios::beg);
        inFile.read(dest, fileSize);
        if (!inFile.good()) {
            inFile.close();
            return false;
        }

        dest[fileSize] = '\0';
        inFile.close();
    }
    return true;
}

int main(){

    /*
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
