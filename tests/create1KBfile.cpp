#include <iostream>
#include <fstream>

int main() {
    const char characterToWrite = 'A';

    const std::size_t fileSize = (1 << 10);  // 2^30 = 2^10 * 2^10 * 2^5// 32megabytes

    std::ofstream outputFile("1KB.txt", std::ios::binary);

    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return 1; // Return an error code
    }

    for (std::size_t i = 0; i < fileSize; ++i) {
        outputFile.put(characterToWrite);
    }

    outputFile.close();

    std::cout << "File 'output.txt' generated successfully with " << fileSize << " 'A's." << std::endl;

    return 0; // Return success
}
