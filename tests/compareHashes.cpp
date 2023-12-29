#include <iostream>


const unsigned int HASH_LEN = 64;

bool compareHashes(const char* hash1, const char* hash2) {
    for (int i = 0; i < HASH_LEN; ++i) {
        if (*(hash1 + i) != *(hash2 + i)) {
            return 0;
        }
    }
    return 1;
}

int main() {

    char str1[HASH_LEN+1] = "111111This is a 64-symbol string that will be saved to a file.12";  // Replace with your own data
    char str2[HASH_LEN+1] = "11111 1This is a 64-symbol string that will be savd to a file.12";  // Replace with your own data

    int result = compareHashes(str1, str2);

    if (result == 1) {
        std::cout << " are the same.\n";
    } else {
        std::cout << "are not the same.\n";
    }

    return 0;
}

