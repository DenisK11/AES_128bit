#include "AES_128.h"
#include "ECDA.h"

int main()
{
    unsigned char plainText[256] = "Johnson merge la magazin si se culca dupa!";
    unsigned char initialKey[17] = "Initial Keysssss";
    unsigned char temp[17];
    int iteration = 0;

    unsigned char tempDecryptedMessage[256] = "\0";
    unsigned char tempEncryptedMessage[256] = "\0";

    bool repeat = false;

    unsigned char temp1[16] = "";

    unsigned char* decryptedMessge;
    unsigned char* cipherText;

    repeat = checkPadding(initialKey, 0);

    while (repeat)
    {
            copynString(temp, plainText, 16, 16 * iteration);

            repeat = checkPadding(temp, 1);

        // generate_elipse_points(1, 113);
            cipherText = AES_encrypt_128(initialKey, temp);

            copyString(temp1, cipherText);

            appendString(tempEncryptedMessage, temp1);

            decryptedMessge = AES_128_decrypt(cipherText);

            appendString(tempDecryptedMessage, decryptedMessge);

            iteration++;
    }

    cout << "\nInitial message is: ";
    showMessage(plainText);
    cout << "\nCipherText is: ";
    showMessage(tempEncryptedMessage);
    cout << endl;
    cout << "Decrypted message is: ";
    showMessage(tempDecryptedMessage);

    return 0;   
}