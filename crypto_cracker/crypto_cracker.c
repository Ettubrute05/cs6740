#include "crypto_cracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void cryptoCrackerStart()
{
    while (1)
    {
        int choice;

        printf("\n=== Crypto Cracker Menu ===\n");
        printf("1. Detect and decrypt file\n");
        printf("2. Return to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice)
        {
        case 1:
            cryptoCracker();
            break;

        case 2:
            return;

        default:
            printf("Invalid option. Please try again.\n");
            break;
        }
    }
}

void cryptoCracker()
{
    char content[10000];
    char decryptedText[10000];
    int fileRead = 0;

    while (fileRead == 0)
    {
        char filename[100];
        printf("Enter the name of the file: ");
        scanf("%s", filename);
        getchar();

        fileRead = readFile(filename, content, sizeof(content));
    }

    // Try detecting and decrypting Caesar cipher
    const int caesarKey = detectCaesarCipher(content);
    if (caesarKey != 0)
    {
        printf("Detected Caesar cipher with key: %d\n", caesarKey);
        decryptCaesarCipher(content, caesarKey, decryptedText);
        printf("Sample of decrypted text (Caesar): \n%.100s...\n", decryptedText);
        return;
    }

    // Try detecting Vigenere cipher
    const int vigenereKeyLength = detectVigenereCipher(content);
    char vigenereKey[13];
    if (vigenereKeyLength != 0)
    {
        findVigenereKey(content, vigenereKeyLength, vigenereKey);
        printf("Detected Vigenere cipher with key: %s\n", vigenereKey);
        decryptVigenereCipher(content, vigenereKey, decryptedText);
        printf("Sample of decrypted text (Vigenere): \n%.100s...\n", decryptedText);
        return;
    }

    // Try detecting columnar cipher
    const int columnarKeyLength = detectColumnarCipher(content);
    if (columnarKeyLength != 0)
    {
        printf("Detected Columnar cipher with key length %d\n", columnarKeyLength);
        decryptColumnarCipher(content, columnarKeyLength, decryptedText);
        printf("Sample of decrypted text (Columnar): \n%.100s...\n", decryptedText);
        return;
    }

    // If none of those ciphers are detected
    printf("Detected other cipher\n");
}

int readFile(char* filename, char *content, int maxlength)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: could not open file %s\n", filename);
        return 0;
    }
    fgets(content, maxlength, file);
    fclose(file);
    return 1;
}

int detectCaesarCipher(const char *text)
{
    int letterCounts[26] = {0};
    const int length = strlen(text);

    // Count letter frequencies
    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]))
        {
            letterCounts[tolower(text[i]) - 'a']++;
        }
    }

    // Find the most common letter (assuming it's e)
    int maxIndex = 0;
    for (int i = 0; i < 26; i++)
    {
        if (letterCounts[i] > letterCounts[maxIndex])
        {
            maxIndex = i;
        }
    }

    // Calculate the shift key
    const int key = (maxIndex - ('e' - 'a') + 26) % 26;

    // Validate if it's likely a Caesar cipher by checking the distribution
    if (letterCounts[maxIndex] > 0.1 * length && maxIndex != 4)
    {
        return key;
    }
    return 0;
}

void decryptCaesarCipher(const char *text, int key, char *output)
{
    const int length = strlen(text);
    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]))
        {
            const char base = isupper(text[i]) ? 'A' : 'a';
            output[i] = (text[i] - base - key + 26) % 26 + base;
        }
        else
        {
            output[i] = text[i];
        }
    }
    output[length] = "\0";
}

int detectVigenereCipher(const char *text)
{
    const int textLength = strlen(text);
    const int maxKeyLength = 12;
    int probableKeyLength = 0;
    int maxRepeats = 0;

    for (int keyLength = 1; keyLength <= maxKeyLength; keyLength++)
    {
        int repeatCount = 0;
        for (int i = 0; i < textLength - keyLength; i++)
        {
            if (strncmp(&text[i], &text[i + keyLength], 3) == 0)
            {
                repeatCount++;
            }
        }

        if (repeatCount > maxRepeats)
        {
            maxRepeats = repeatCount;
            probableKeyLength = keyLength;
        }
    }

    // Validate whether the cipher text is likely a Vigenere cipher
    if (probableKeyLength > 0)
    {
        char testKey[probableKeyLength + 1];
        char decryptedText[10000];
        findVigenereKey(text, probableKeyLength, testKey);
        decryptVigenereCipher(text, testKey, decryptedText);

        const int length = strlen(decryptedText);
        int vowelCount = 0;
        for (int i = 0; i < length; i++)
        {
            if (strchr("aeiuoAEIOU", decryptedText[i]))
            {
                vowelCount++;
            }
        }

        if (vowelCount > 0.15 * length)
        {
            return probableKeyLength;
        }
    }

    return 0;
}

void findVigenereKey(const char *text, int keyLength, char *key)
{
    int textLength = strlen(text);
    for (int k = 0; k < keyLength; k++)
    {
        int letterCounts[26] = {0};
        int segmentLength = 0;

        for (int i = k; i < textLength; i += keyLength)
        {
            if (isalpha(text[i]))
            {
                letterCounts[tolower(text[i]) - 'a']++;
                segmentLength++;
            }
        }

        int maxIndex = 0;
        for (int i = 1; i < 26; i++)
        {
            if (letterCounts[i] > letterCounts[maxIndex])
            {
                maxIndex = i;
            }
        }

        key[k] = (maxIndex - ('e' - 'a') + 26) % 26 + 'a';
    }
    key[keyLength] = '\0';
}

void decryptVigenereCipher(const char *text, const char *key, char *output)
{
    const int textLength = strlen(text);
    const int keyLength = strlen(key);

    for (int i = 0; i < textLength; i++)
    {
        if (isalpha(text[i]))
        {
            const char base = isupper(text[i]) ? 'A' : 'a';
            const char keychar = tolower(key[i % keyLength]) - 'a';
            output[i] = (text[i] - base - keychar + 26) % 26 + base;
        }
        else
        {
            output[i] = text[i];
        }
    }
    output[textLength] = '\0';
}

int detectColumnarCipher(const char *text)
{
    const int textLength = strlen(text);
    const int maxKeyLength = 20;
    int probableKeyLength = 0;

    for (int keyLength = 2; keyLength <= maxKeyLength; keyLength++)
    {
        int matches = 0;

        for (int i = 0; i < keyLength; i++)
        {
            for (int j = i; j < textLength; j += keyLength)
            {
                if (isalpha(text[j]))
                {
                    matches++;
                }
            }
        }

        if (matches > textLength * 0.7)
        {
            probableKeyLength = keyLength;
            break;
        }
    }

    return (probableKeyLength > 0) ? probableKeyLength : 0;
}

void decryptColumnarCipher(const char *text, int keyLength, char *output)
{
    const int textLength = strlen(text);
    const int numRows = (textLength + keyLength - 1) / keyLength;
    char tempGrid[numRows][keyLength];

    memset(tempGrid, 0, sizeof(tempGrid));

    int index = 0;
    for (int col = 0; col < keyLength && index < textLength; col++)
    {
        for (int row = 0; row < numRows && index < textLength; row++)
        {
            tempGrid[row][col] = text[index++];
        }
    }

    index = 0;
    for (int row = 0; row < numRows; row++)
    {
        for (int col = 0; col < keyLength; col++)
        {
            if (tempGrid[row][col] != 0)
            {
                output[index++] = tempGrid[row][col];
            }
        }
    }
    output[index] = '\0';
}