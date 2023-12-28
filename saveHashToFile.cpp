
#include <iostream>
#include <fstream>

using namespace std;

const unsigned int HASH_LEN = 64;


void saveStringToFile(const char* str, const char* filename) {
    // Open the file in binary mode for writing
    ofstream outFile(filename, ios::binary);

    // Check if the file is open
    if (!outFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    // Write the string to the file
    outFile.write(str, HASH_LEN);

    // Close the file
    outFile.close();

    cout << "String saved to file: " << filename << endl;
}

int main() {
    const char* myString = "111111This is a 64-symbol string that will be saved to a file.1234567890123456789012345678901234567890123456789012345678901234";

    // Specify the filename
    const char* filename = "output.txt";

    // Call the function to save the string to the file
    saveStringToFile(myString, filename);

    return 0;
}
