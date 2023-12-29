#include <iostream>
#include <fstream>

const unsigned int MAX_FILE_SIZE = 10485760;  // 10,485,760 bytes = 10MB


void loadFileContent(const char* filename, char* dest, size_t destSize) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate); // Open file at the end to get its size

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        dest[0] = '\0'; // Empty string on failure
        return;
    }

    std::streamsize fileSize = file.tellg(); // Get file size
    file.seekg(0, std::ios::beg); // Move back to the beginning of the file

    if (fileSize > static_cast<std::streamsize>(destSize - 1)) {
        std::cerr << "File size exceeds maximum allowed size." << std::endl;
        dest[0] = '\0'; // Empty string
        file.close();
        return;
    }

    file.read(dest, fileSize); // Read the content into the provided buffer
    dest[fileSize] = '\0'; // Null-terminate the string

    file.close(); // Close the file
}

int main() {
    const char* filename = "example.txt";
    const size_t maxFileSize = MAX_FILE_SIZE;
    char fileContent[maxFileSize];

    loadFileContent(filename, fileContent, maxFileSize);

    // Process the file content or print it
    std::cout << "File Content:\n" << fileContent << std::endl;

    return 0;
}
