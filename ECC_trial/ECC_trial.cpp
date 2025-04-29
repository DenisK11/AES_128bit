#include "AES_128.h"
#include "ECDH.h"

int main()
{
    Point G = { 192, 105, false };
    Point alice_pub;
    Point bob_pub;
    Point alice_shared;
    Point bob_shared;

    int alice_secret = 7;
    int bob_secret = 11;

    alice_pub = scalar_mult(alice_secret, G); // generate the public "keys" for alice and bob
    bob_pub = scalar_mult(bob_secret, G);

    alice_shared = scalar_mult(alice_secret, bob_pub); // generate the shared keys for alice and bob
    bob_shared = scalar_mult(bob_secret, alice_pub);


    unsigned char plainText[256] = "Johnson merge la magazin si se culca dupa John John johsonaaa!";
    unsigned char initialKey[17] = "Initial Keysssss"; // The initial key that was chosen will be sent over the network
    unsigned char temp[17];                            // even if it is intercepted it will be worthless unless the attacker can
    int iteration = 0;                                 // get the shared key.

    unsigned char tempDecryptedMessage[256] = "\0";
    unsigned char tempEncryptedMessage[256] = "\0";

    bool repeat = false;

    unsigned char temp1[16] = "";

    unsigned char* decryptedMessge;
    unsigned char* cipherText;

    bad_hash(initialKey, alice_shared.x);

    repeat = checkPadding(initialKey, 0);

    while (repeat)
    {
            copynString(temp, plainText, 16, 16 * iteration);

            repeat = checkPadding(temp, 1);

            cipherText = AES_encrypt_128(initialKey, temp);

            copyString(temp1, cipherText);

            appendString(tempEncryptedMessage, temp1);

            decryptedMessge = AES_128_decrypt(cipherText);

            appendString(tempDecryptedMessage, decryptedMessge);

            iteration++;
    }

    removePadding(tempDecryptedMessage);

    cout << "\nInitial message is: ";
    showMessage(plainText);
    cout << "\nCipherText is: ";
    showMessage(tempEncryptedMessage);
    cout << endl;
    cout << "Decrypted message is: ";
    showMessage(tempDecryptedMessage);

    return 0;   
}