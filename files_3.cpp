#include <iostream>
#include <fstream>

void processFile(const char* filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    const int bufferSize = 64;
    char buffer[bufferSize + 1];  // +1 for null terminator

    while (!file.eof()) {
        file.read(buffer, bufferSize);
        buffer[file.gcount()] = '\0';  // Null-terminate the buffer

        // Process the buffer (in this case, just print it)
        std::cout << "Buffer contents: " << buffer << std::endl;
    }

    file.close();
}


int main() {
    const char* filename = "output.txt";  // Replace with the actual file name

    processFile(filename);

    return 0;
}
