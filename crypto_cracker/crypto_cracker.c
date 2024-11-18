#include "crypto_cracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Menu for the crypto cracker section of the program
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

// Attempt to identify and decrypt enciphered text
void cryptoCracker()
{
    char content[MAX_TEXT_SIZE];
    char decryptedText[MAX_TEXT_SIZE];
    int fileRead = 0;
    char filename[100];

    while (fileRead == 0)
    {
        printf("Enter the name of the file: ");
        scanf("%s", filename);
        getchar();

        fileRead = readFile(filename, content, sizeof(content));
    }
    clock_t start, end;
    double elapsedTime;
    start = clock();

    // Try detecting and decrypting Caesar cipher
    const int caesarKey = detectCaesarCipher(content);
    if (caesarKey != 0)
    {
        printf("Detected Caesar cipher with key: %c\n", ('a' + caesarKey));
        decryptCaesarCipher(content, caesarKey, decryptedText);
        printf("Sample of decrypted text (Caesar): \n%.200s...\n", decryptedText);
        end = clock();
        elapsedTime = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Time taken to calculate result %.3f seconds\n", elapsedTime);
        return;
    }

    // Try detecting Vigenere cipher
    char vigenereKey[MAX_KEY_LENGTH + 1];
    const int vigenereKeyLength = detectVigenereCipher(content);
    if (vigenereKeyLength != 0)
    {
        findVigenereKey(content, vigenereKeyLength, vigenereKey);
        decryptVigenereCipher(content, vigenereKey, decryptedText);
    }
    if (vigenereKeyLength != 0 && containsCommonWords(decryptedText) > 3)
    {
        printf("Detected Vigenere cipher with key: %s\n", vigenereKey);
        printf("Sample of decrypted text (Vigenere): \n%.200s...\n", decryptedText);
        end = clock();
        elapsedTime = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Time taken to calculate result %.3f seconds\n", elapsedTime);
        return;
    }

    // Try detecting columnar cipher
    int columnarKeyLength = detectColumnarCipher(content);
    if (columnarKeyLength != 0)
    {
        columnarKeyLength = decryptColumnarCipher(content, decryptedText);
        printf("Detected Columnar cipher with key %d\n", columnarKeyLength);
        printf("Sample of decrypted text (Columnar): \n%.200s...\n", decryptedText);
        end = clock();
        elapsedTime = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Time taken to calculate result %.3f seconds\n", elapsedTime);
        return;
    }

    // If none of those ciphers are detected
    printf("Detected other cipher\n");
    end = clock();
    elapsedTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken to calculate result %.3f seconds\n", elapsedTime);
}

// Read the text from the file
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

// Check the text to detect if it was encrypted with a Caesar cipher
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

// Decrypt text with Caesar cipher
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

// Detect if the text was encrypted with Vigenere cipher
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
        char decryptedText[MAX_TEXT_SIZE];
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

        if (testKey[0] == 'a' && (testKey[1] == 'a' || testKey[1] == '\0'))
        {
            return 0;
        }

        if (vowelCount > 0.15 * length && probableKeyLength > 1)
        {
            return probableKeyLength;
        }
    }

    return 0;
}

// Find the Vigenere cipher key
void findVigenereKey(const char *text, int keyLength, char *key)
{
    const int textLength = strlen(text);
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

    // Check for repeating substrings in the key and reduce it to the shortest correct key
    for (int subLen = 1; subLen <= keyLength / 2; subLen++) {
        int isRepeating = 1;
        for (int i = 0; i < keyLength - subLen; i++) {
            if (key[i] != key[i + subLen]) {
                isRepeating = 0;
                break;
            }
        }
        if (isRepeating) {
            key[subLen] = '\0'; // Reduce the key to the shortest repeating substring
            break;
        }
    }
}

// Decrypt text with Vigenere cpher
void decryptVigenereCipher(const char *text, const char *key, char *output)
{
    const int textLength = strlen(text);
    const int keyLength = strlen(key);

    for (int i = 0; i < textLength; i++)
    {
        if (isalpha(text[i]))
        {
            const char base = isupper(text[i]) ? 'A' : 'a';
            const char keyChar = tolower(key[i % keyLength]) - 'a';
            output[i] = (text[i] - base - keyChar + 26) % 26 + base;
        }
        else
        {
            output[i] = text[i];
        }
    }
    output[textLength] = '\0';
}

// Detect if the text was encrypted with columnar cipher
int detectColumnarCipher(const char *text) {
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

    // Validate if it's likely a columnar cipher by checking the distribution
    if (letterCounts[maxIndex] > 0.1 * length && maxIndex == 4)
    {
        return 1;
    }
    return 0;
}

// Decrypt text with columnar cipher
int decryptColumnarCipher(const char *text, char *output)
{
    const int textLength = strlen(text);
    int probableKeyLength = 0;
    int maxWordsCount = 0;
    char tempOutput[MAX_TEXT_SIZE];

    for (int keyLength = 2; keyLength <= textLength / 2; keyLength++)
    {
        if (textLength % keyLength != 0)
        {
            continue;
        }

        const int cols = keyLength;
        const int rows = textLength / keyLength;
        char tempGrid[rows][cols];

        int index = 0;
        for (int i = 0; i < cols; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                tempGrid[j][i] = text[index++];
            }
        }

        index = 0;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                tempOutput[index++] = tempGrid[i][j];
            }
        }

        tempOutput[index] = '\0';

        const int wordsCount = containsCommonWords(tempOutput);

        if (wordsCount > maxWordsCount)
        {
            maxWordsCount = wordsCount;
            probableKeyLength = keyLength;
            strncpy(output, tempOutput, textLength);
        }
    }

    return probableKeyLength;
}

// Count the common words in the text
int containsCommonWords(const char *text) {
    const char *commonWords[15] = {
        "THE", "AND", "FOR", "YOU", "THAT", "WITH", "THIS", "THEY", "HAVE",
        "FROM", "WILL", "YOUR", "MORE", "BEEN", "WHICH"
    };

    int wordCount[15] = {0};
    int count = 0;
    const int textLength = strlen(text);
    char textCopy[textLength +1];
    strcpy(textCopy, text);

    // Convert the text to lowercase for case-insensitive comparison
    for (int i = 0; textCopy[i]; i++) {
        textCopy[i] = toupper(textCopy[i]);
    }

    // Check each substring of length 3 to 6
    for (int length = 3; length <= 6; length++) {
        for (int i = 0; i <= textLength - length; i++) {
            char substring[6 + 1] = {0};
            strncpy(substring, &textCopy[i], length);
            substring[length] = '\0';

            for (int j = 0; j < 15; j++) {
                if (strcmp(substring, commonWords[j]) == 0 && wordCount[j] == 0) {
                    wordCount[j] = 1;
                    count++;
                    break;
                }
            }
        }
    }

    return count;
}