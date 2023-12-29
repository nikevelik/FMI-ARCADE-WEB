#include <iostream>
#include <fstream>

unsigned int MAX_FILESIZE_BYTES = 1024;

bool readFile(const char* filename, char* dest) {
    if(!dest || !filename){
        return 0;
    }
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    if (fileSize > MAX_FILESIZE_BYTES) {
        return false;
    }
    file.read(dest, fileSize);
    dest[fileSize] = '\0';
    file.close();
    return true;
}

int main() {
    const char* filename = "1KB.txt";
    char fileContent[1024+1] = "";
    if (readFile(filename, fileContent)) {
        std::cout << "File content:\n" << fileContent << std::endl;
    }
    return 0;
}
