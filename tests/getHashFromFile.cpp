#include <iostream>
#include <fstream>

const unsigned int HASH_LEN = 64;

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

int main() {
    const char* fileName = "../../1KB.txt";
    char dest[HASH_LEN + 1];

    getHashFromFile(fileName, dest);

    std::cout << "String read from file: " << dest << std::endl;

    return 0;
}
