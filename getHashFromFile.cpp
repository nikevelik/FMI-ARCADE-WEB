#include <iostream>
#include <fstream>

const unsigned int HASH_LEN = 64;

void readAndSaveString(const char* fileName, char* dest) {
    // Open the file
    std::ifstream file(fileName);

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << fileName << std::endl;
        return;
    }

    // Read the first 64 characters from the file
    file.read(dest, 64);

    // Close the file
    file.close();
}

int main() {
    const char* fileName = "output.txt";
    char dest[HASH_LEN + 1]; // Assuming one extra character for null-termination

    // Call the function to read and save the string
    readAndSaveString(fileName, dest);

    // Null-terminate the destination string
    dest[64] = '\0';

    // Display the result
    std::cout << "String read from file: " << dest << std::endl;

    return 0;
}
