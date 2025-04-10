#include "AES_128.h"
#include "ECDA.h"

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

   // repeat = checkPadding(initialKey, 0);
    repeat = true;

    while (repeat)
    {
        // generate_elipse_points(1, 113);
            AES_encrypt_128(initialKey, plainText);

            cout << "\nInitial message is: ";
            showMessage(plainText);
            cout << "\nCipherText is: ";
            showMessage(cipherText);
            cout << endl;

            //repeat = checkPadding(plainText, 1);
            repeat = false;
    }

    return 0;   
}