#include "AES_128.h"

extern unsigned char keys[12][16];
extern unsigned char cipherText[16];

// Will update the code making it more efficient and optimized

void AES_encrypt_128(unsigned char* initialKey, unsigned char* plainText)
{
    unsigned char temp[16];

    unsigned char i;

    unsigned char rounds;

    unsigned char a0;
    unsigned char a1;
    unsigned char a2;
    unsigned char a3;

    generateKeys(initialKey);

    for (i = 0; i < 16; i++)
    {
        temp[i] = plainText[i] ^ keys[0][i]; // Round 0 key XOR-ing
    }

    // AES-128 has 11 rounds
    for (rounds = 0; rounds < 11; rounds++)
    {
        // AES First stage sbox the states
        for (i = 0; i < 16; i++)
        {
            temp[i] = sbox[temp[i]]; // find the value from the sbox
        }

        //Stage 2 shift the rows
        shiftrows(temp, -1);

        if (rounds != 10)
        {
            //Stage 3 const matrix multiplication do not do it on the last round
            for (i = 0; i < 4; i++)
            {
                a0 = temp[i * 4];
                a1 = temp[i * 4 + 1];
                a2 = temp[i * 4 + 2];
                a3 = temp[i * 4 + 3];

                temp[i * 4] = finiteMultiplication(matrix[0], a0) ^ finiteMultiplication(matrix[1], a1) ^ finiteMultiplication(matrix[2], a2) ^ finiteMultiplication(matrix[3], a3);
                temp[i * 4 + 1] = finiteMultiplication(matrix[4], a0) ^ finiteMultiplication(matrix[5], a1) ^ finiteMultiplication(matrix[6], a2) ^ finiteMultiplication(matrix[7], a3);
                temp[i * 4 + 2] = finiteMultiplication(matrix[8], a0) ^ finiteMultiplication(matrix[9], a1) ^ finiteMultiplication(matrix[10], a2) ^ finiteMultiplication(matrix[11], a3);
                temp[i * 4 + 3] = finiteMultiplication(matrix[12], a0) ^ finiteMultiplication(matrix[13], a1) ^ finiteMultiplication(matrix[14], a2) ^ finiteMultiplication(matrix[15], a3);
            }
        }
        else
        {
            // do nothing, for compiler efficiency reasons
        }

        // AES Last stage XOR the states with the round key
        for (i = 0; i < 16; i++)
        {
            temp[i] = temp[i] ^ keys[rounds + 1][i];
        }

    }

    copyString(cipherText, temp);
}

void generateKeys(unsigned char* initialKey) // break the initialKey into 4 byte chunks to feed to the function
{

    unsigned char temp[16];

    unsigned char i;
    unsigned char j;

    for (i = 0; i < 16; i++)
    {
        temp[i] = initialKey[i];
        keys[0][i] = temp[i];
    }

    for (i = 1; i < 2; i++)
    {

        for (j = 0; j < 16; j++)
            keys[i][j] = temp[j];       // To Do: Fix the bugs with the key generation 

        shiftrows(temp, 0);

        keys[i][0] = keys[i - 1][0] ^ (sbox[temp[0]] ^ rcon[i - 1]);
        keys[i][1] = keys[i - 1][1] ^ (sbox[temp[1]] ^ rcon[i - 1]);
        keys[i][2] = keys[i - 1][2] ^ (sbox[temp[2]] ^ rcon[i - 1]); // W[i] in which i % 4 == 0
        keys[i][3] = keys[i - 1][3] ^ (sbox[temp[3]] ^ rcon[i - 1]); // 4 bytes is the entire word so everything here gets this treatment

        for (j = 1; j < 4; j++)
        {
            keys[i][4 * j] = keys[i - 1][4 * j] ^ keys[i][4 * (j - 1)];
            keys[i][4 * j + 1] = keys[i - 1][4 * j + 1] ^ keys[i][4 * (j - 1)]; // the previous key, the current element XOR
            keys[i][4 * j + 2] = keys[i - 1][4 * j + 2] ^ keys[i][4 * (j - 1)]; // the current key, the element offseted by 4 spaces
            keys[i][4 * j + 3] = keys[i - 1][4 * j + 3] ^ keys[i][4 * (j - 1)];
        }

        for (j = 0; j < 16; j++)
            temp[j] = keys[i][j];
    }
    showMessage(keys[1]);
}

// by assigning 2 bytes of memory instead of making pointers to the variables we
// spare ourselves a lot of headaches and yes, the treasury will have to bear this expanse.
unsigned char finiteMultiplication(unsigned char a, unsigned char b)
{
    unsigned char result = 0;

    unsigned char i;

    unsigned char hi_bit;

    for (i = 0; i < 8; i++)
    {
        // If the lowest bit of b is set, add current a to result
        if (b & 1)
            result ^= a;

        // Check if highest bit of a is set (i.e., overflow)
        hi_bit = a & 0x80;

        // Shift a left by 1 (i.e., multiply by x)
        a <<= 1;

        // If overflowed, reduce using AES's irreducible polynomial
        if (hi_bit)
            a ^= 0x1B; // 0x1B is the irreducible polynomial

        // Shift b right by 1 to process next bit
        b >>= 1;
    }
    return result;
}

// Needs optimization
static inline void shiftrows(unsigned char* temp, int i) // to the left, i is for the offset of the matrix to the rows
{
    unsigned char aux;

    if (i == -1)
    {
        // Row 1 (second row): shift left by 1
        aux = temp[1];
        temp[1] = temp[5];
        temp[5] = temp[9];
        temp[9] = temp[13];
        temp[13] = aux;

        // Row 2 (third row): shift left by 2
        aux = temp[2];
        temp[2] = temp[10];
        temp[10] = aux;
        aux = temp[6];
        temp[6] = temp[14];
        temp[14] = aux;

        // Row 3 (fourth row): shift left by 3 (or right by 1)
        aux = temp[3];
        temp[3] = temp[15];
        temp[15] = temp[11];
        temp[11] = temp[7];
        temp[7] = aux;
    }
    else
    {
        aux = temp[i];
        temp[i] = temp[i + 4];
        temp[i + 4] = temp[i + 8];
        temp[i + 8] = temp[i + 12];
        temp[i + 12] = aux;
    }
}

void AES_128_decrypt(unsigned char* cipherText)
{
    // To Do: make the decryption algorithm
}