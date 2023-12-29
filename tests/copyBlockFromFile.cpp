#include <iostream>
#include <fstream>

// Function to copy the block-th block of 64 characters from a file
void copyBlockFromFile(const char *filename, const unsigned long long block, unsigned char *dest) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file\n";
        exit(EXIT_FAILURE);
    }

    file.seekg(block * 64, std::ios::beg);

    file.read( (char*) dest, 64);

    if (file.gcount() < 64) {
        if (file.eof()) {
            std::cout << "End of file reached before reading 64 characters.\n";
        } else if (file.fail()) {
            std::cerr << "Error reading from file\n";
        }
    }

    file.close();
}

int main() {
    const char *filename = "../../1KB.txt";
    const unsigned int block = 2; // Replace with the desired block number

    // Get the length of the file
    long fileLength = getFileLength(filename);
    std::cout << "File length: " << fileLength << " bytes\n";

    // Allocate a buffer to store the block of 64 characters
    unsigned char dest[64];

    // Copy the specified block from the file to the buffer
    copyBlockFromFile(filename, block, dest);

    // Print the contents of the buffer
    std::cout << "Block " << block << " contents: " << std::string(reinterpret_cast<char*>(dest), 64) << "\n";

    return 0;
}
