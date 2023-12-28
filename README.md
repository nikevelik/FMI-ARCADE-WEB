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
1. Round Constants (K0-K63): SHA-256 uses a set of 64 constant 32-bit words (K0, K1, ..., K63) during the processing of each 512-bit block of the input message. These constants are derived from the fractional parts of the cube roots of the first 64 prime numbers.
2. HASH_LEN - 64.


### File I/O

1. saveHashToFile - saves message of length HASH_LEN (str) to a file (filename)

2. getHashFromFile - gets message from file and saves it to dest.

3. compareHashes - check if hashes match or not