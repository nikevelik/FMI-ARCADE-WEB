# SHA256
This is console application, implementing hashing of TEXT with SHA256, part of Secure Hash Algorithm 2. 

The application has the following functionalities:
- reading a message from a file
- hashing a message using SHA-256
- saving a hashed message to a file
- hashed message reading - determine if a submitted hash matches the hash of a read message

## Code documentation:

### Helper Functions
1. addWithCarry - Incrementation of a number, handle overflow by incrementing the carry.
    - The addWithCarry function is necessary to keep track of the total bit length of the input data processed by the SHA-256 algorithm. The bit length is an integral part of the SHA-256 algorithm, and it influences the finalization steps and ensures that the entire message length is taken into account when computing the hash.

2. getRightRotation - Circular right rotation - shift right and wrap the shifted bits on the left.
3. getChooseBitByBit -  the x input chooses if the output is from y or from z. More precisely, for each bit position, that result bit is according to the bit from y (or respectively z ) at this position, depending on if the bit from x at this position is 1 (or respectively 0).
4. getBitwiseMajority - If a minimum 2 of 3 bits at position n in inputs x, y, z are 1, the majority of bits is considered 1. Thus, bit at position n in result will be 1. Otherwise 0.

### SHA 256 Logical Functions
1. Expansion Permutation 0: fundamental constant function in the SHA-256 algorithm. It involves circular right rotations of the input 'x' at specific bit positions (2, 13, and 22). EP0 contributes to the non-linearity and diffusion properties required for secure cryptographic hash generation.

2. Expansion Permutation 1: another crucial constant function in the SHA-256 algorithm. Similar to EP0, it performs circular right rotations on the input 'x' at specific bit positions (6, 11, and 25). EP1 plays a vital role in enhancing the cryptographic strength of the hash function.

3. Sigma 0: an essential constant function that integrates circular right rotations and bit-wise shifting operations. It operates on the input 'x' at specific bit positions (7, 18, and 3) to introduce further complexity and security to the SHA-256 algorithm.

4. Sigma 1: a critical constant function in the SHA-256 algorithm, similar to SIG0. It applies circular right rotations and bit-wise shifting operations to the input 'x' at specified bit positions (17, 19, and 10). SIG1 contributes to the overall robustness and cryptographic properties of the hash function.

### Constants
1. ROUND_CONSTANTS (K0-K63): SHA-256 uses a set of 64 constant 32-bit words (K0, K1, ..., K63) during the processing of each 512-bit block of the input message. These constants are derived from the fractional parts of the cube roots of the first 64 prime numbers.
2. INITIAL_HASHES (0-8): starting values for every (32-bit) hash subpart (they make up the whole 256 bit hash)
3. HASH_LEN - 64.
4. HEX_CHARS - string/array of chars that contains the corresponding hex digit to numbers 0-15
5. MAX_FILESIZE_BYTES - maximum allowed file size in bytes



### File I/O

1. saveHashToFile - saves message of length HASH_LEN (str) to a file (filename)

2. getHashFromFile - gets message from file and saves it to dest.

3. compareHashes - check if hashes match or not

4. readFileIntoChar - loads File Content into char[] variable. Limited by MAX_FILESIZE_BYTES

### SHA functions
1. SHA256 - main function. 
    0. The `SHA256` function is the main component of the SHA-256 hashing algorithm, which generates a secure 256-bit hash (32-byte hash) from the given input string. The resulting hash is stored in the character array provided by the `dest` parameter.

    1. Parameters
        - `input_str` (const char*): The input string to be hashed.
        - `dest` (char*): A pointer to the destination character array where the resulting hash will be stored.

    2. Return Value
        - `bool`: The function returns `true` if the hashing process is successful, and `false` if either the `dest` pointer or the `input_str` is null.
    
    3. Implementation Details
        1. Input Validation:
            - The function checks if the destination pointer (`dest`) and the input string pointer (`input_str`) are not null. If either is null, the function returns `false`.

        2. Initialization:
            - The function initializes a data buffer (`dataBuffer`) to store each 64-symbol block of the input.
            - It tracks the last iterated symbol in the block (`idxInBuffer`) and the total bits iterated (`bitlen`).
            - The sub-hashes (`subhashes`) are initialized with the initial hash values (`INITIAL_HASHES`).

        3. SHA256 Update:
            - The function calls the `SHA256Update` function to process the input string in 64-symbol blocks, updating the sub-hashes accordingly. If the update fails, the function returns `false`.

        4. SHA256 Finalization:
            - The function calls the `SHA256Final` function to include bias in the hash based on the input length, updating the sub-hashes. If the finalization fails, the function returns `false`.

        5. Conversion to String:    
            - The function calls the `subhashesToStr` function to convert the sub-hashes into a hash string. If the conversion fails, the function returns `false`.

        6. Success:
            - If all steps are completed successfully, the function returns `true`.



2. subhashesToStr - convert the 8 subparts of (4-byte) words into a whole hash
    0. The `subhashesToStr` function is designed to convert an array of 8 subhashes, each represented as a 4-byte word, into a single hash string. The resulting hash string is stored in a character array provided as the `dest` parameter. The hash string is represented in hexadecimal format.
    1. Parameters
        - `subhashes` (unsigned int[8]): An array containing 8 subhashes, each represented as a 4-byte word.
        - `dest` (char*): A pointer to the destination character array where the resulting hash string will be stored.
    2. Return Value
        - `bool`: The function returns `true` if the conversion is successful, and `false` if either the `dest` pointer or the `subhashes` array is null.

    3. Implementation Details
        - The function iterates through each byte position (0 to 3) in the 4-byte words and each subhash in the `subhashes` array. For each byte, it extracts the byte at the specified position, calculates the corresponding index in the destination array (`dest`), and converts the byte to a hexadecimal representation. The resulting hash string is formed by concatenating the hexadecimal representations of the bytes.

3.SHA256Final
    0. The `SHA256Final` function is responsible for finalizing the SHA-256 hash computation. It includes padding the input data, updating the bit length based on the input length, and incorporating a bias in the hash calculation. The final hash is updated in the `subhashes` array.
    1. Parameters
        - `dataBuffer` (unsigned char*): A pointer to the input data buffer.
        - `idxInBuffer` (unsigned int): The index indicating the current position in the data buffer.
        - `bitlen` (unsigned int[2]): A 64-bit array representing the bit length of the input data.
        - `subhashes` (unsigned int[8]): An array containing 8 subhashes.
    2. Return Value
        - `bool`: The function returns `true` if the finalization process is successful and `false` if any of the input pointers (`dataBuffer`, `bitlen`, or `subhashes`) is null.
    3. Implementation Details
        - The function first checks for null pointers in the input parameters. It then determines whether there is enough space in the data buffer for padding. If there is sufficient space, padding is added directly. If not, a transformation is performed first before padding.
        - The bit length is incremented based on the input length, and the updated bit length is added to the next transformation. The `SHA256Transform` function is called twice, once before padding and once after, to update the subhashes with the bias included.
