#include "Utilities.h"

void showMessage(unsigned char* msg)
{
    unsigned char i;
    for (i = 0; i < 16; i++)
        printf("%02X ", msg[i]);
}

void copyString(unsigned char* destination, unsigned char* source)
{
    unsigned char i;

    for (i = 0; i < 16; i++)
    {
        destination[i] = source[i];
    }
}

void showmatrix(unsigned char* temp)
{
    for (int i = 0; i < 16; i++)
    {
        i % 4 == 0 ? printf("\n") : false;
        printf("%X ", temp[i]);

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
        printf("\nIncopatible key");
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