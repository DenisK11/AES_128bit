#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace std;

FILE* CSV_file; // To Do: implement the ECDA key exchange after u are done with AES and add modularity to the project

static unsigned char keys[12][16];
unsigned char cipherText[16];

static const unsigned char rcon[10] = {

    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

static const uint8_t sbox[256] = {
    //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static const uint8_t inverse_sbox[256] = {
    //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x52, 0x09,	0x6a, 0xd5, 0x30, 0x36,	0xa5, 0x38,	0xbf, 0x40,	0xa3, 0x9e,	0x81, 0xf3,	0xd7, 0xfb,
    0x7c, 0xe3,	0x39, 0x82,	0x9b, 0x2f,	0xff, 0x87,	0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b,	0x94, 0x32,	0xa6, 0xc2,	0x23, 0x3d,	0xee ,0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

static const unsigned char matrix[16] = {
    //0     1     2     3
    0x02, 0x03, 0x01, 0x01,
    0x01, 0x02, 0x03, 0x01,
    0x01, 0x01, 0x02, 0x03,
    0x03, 0x01, 0x01, 0x02,
};

void generate_elipse_points(int starting_point, int order)
{
    float pointElipse[1000][2];
    float multiplyElipse[1000][2];

    pointElipse[0][0] = starting_point;
    pointElipse[0][1] = fmod( sqrtf( (pow(pointElipse[0][0],3) + 7)), order );

    multiplyElipse[0][0] = pointElipse[0][0];
    multiplyElipse[0][1] = pointElipse[0][1];

    for (int i = 1; i < order; i++)
    {
        pointElipse[i][0] = starting_point + i;
        pointElipse[i][1] = fmod( sqrtf((pow(pointElipse[i][0], 3) + 7)),  order );

        multiplyElipse[i][0] = 1 + i;
        multiplyElipse[i][1] = fmod(multiplyElipse[0][1] * (i + 1), order);
    }

    CSV_file = fopen("curve.csv", "w +");

    for (int i = 0; i < order; i++)
    {
            cout << pointElipse[i][0] << " ";
            cout << pointElipse[i][1] << "      " << multiplyElipse[i][0] << " ";
            cout << multiplyElipse[i][1] << endl;

            fprintf(CSV_file, "%f, ", pointElipse[i][0]);
            fprintf(CSV_file, "%f\n", pointElipse[i][1]);

            printf("\n");
    }

    fclose(CSV_file);

}

static inline void showmatrix(unsigned char* temp)
{
    for (int i = 0; i < 16; i++)
    {
        i % 4 == 0 ? printf("\n") : false;
        printf("%X ", temp[i]);
        
    }
}

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

static inline void showMessage(unsigned char* msg)
{
    unsigned char i;
    for (i = 0; i < 16; i++)
        printf("%02X ", msg[i]);
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

    for (i = 1; i < 0; i++)
    {

        for(j = 0; j < 16; j++)
            keys[i][j] = temp[j];       // To Do: Fix the bugs with the key generation 

        shiftrows(temp, 0);

        keys[i][0] = keys[i - 1][0] ^ (sbox[((temp[0] >> 4) & 0xff) * 16 + (((temp[0] << 4) & 0xff) >> 4)] ^ rcon[i - 1]);
        keys[i][1] = keys[i - 1][1] ^ (sbox[((temp[1] >> 4) & 0xff) * 16 + (((temp[1] << 4) & 0xff) >> 4)] ^ rcon[i - 1]);
        keys[i][2] = keys[i - 1][2] ^ (sbox[((temp[2] >> 4) & 0xff) * 16 + (((temp[2] << 4) & 0xff) >> 4)] ^ rcon[i - 1]); // W[i] in which i % 4 == 0
        keys[i][3] = keys[i - 1][3] ^ (sbox[((temp[3] >> 4) & 0xff) * 16 + (((temp[3] << 4) & 0xff) >> 4)] ^ rcon[i - 1]); // 4 bytes is the entire word so everything here gets this treatment

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
}

bool checkPadding(unsigned char* text, unsigned char key)
{
    unsigned char i = 0;
    bool retval = false;

    if (text[16] != NULL && key != 0)
    {
        while (text[i + 16] != NULL)
        {
            text[i] = text[i + 16];
            text[i + 16] = NULL;
            i++;
        }
        retval = true;
    }
    else if (key == 0 && (text[16] == '\0' || text[16] == NULL))    // To do: fix the bugs with the padding
    {
        cout << "\nIncopatible key" << endl;
        return false;
    }

    if (text[15] == NULL || key == 0)
    {
        for (i = 0; i < 16; i++)
            if (text[i] == NULL)
                text[i] = '0';
        retval = true;
    }

    return retval;

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

static inline void copyString(unsigned char* destination, unsigned char* source)
{
    unsigned char i;

    for (i = 0; i < 16; i++)
    {
        destination[i] = source[i];
    }
}

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

void AES_128_decrypt(unsigned char* cipherText)
{
        // To Do: make the decryption algorithm
}

int main()
{
    unsigned char plainText[16] = { 
    0x00, 0x11, 0x22, 0x33,
    0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb,
    0xcc, 0xdd, 0xee, 0xff
    };
    unsigned char initialKey[16] = {
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b,
    0x0c, 0x0d, 0x0e, 0x0f
    };
    bool repeat = false;

    unsigned char temp[16] = {
    0xd6, 0xaa, 0x74, 0xfd,
    0xd2, 0xaf, 0x72, 0xfa,
    0xda, 0xa6, 0x78, 0xf1,
    0xd6, 0xab, 0x76, 0xfe
    };

    for (int i = 0; i < 16; i++)
    {
        keys[1][i] = temp[i];
    }

   // repeat = checkPadding(initialKey, 0);
    repeat = true;

    while (repeat)
    {
        // generate_elipse_points(1, 113);
            AES_encrypt_128(initialKey, plainText);

            cout << "Initial message is: ";
            showmatrix(plainText);
            cout << "\nCipherText is: ";
            showMessage(cipherText);
            cout << endl;

            //repeat = checkPadding(plainText, 1);
            repeat = false;
    }

    return 0;   
}