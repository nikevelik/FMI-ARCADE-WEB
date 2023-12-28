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

