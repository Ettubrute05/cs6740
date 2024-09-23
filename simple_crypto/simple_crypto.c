#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "simple_crypto.h"

void cryptoStart()
{
    int choice;
    char key;
    int encrypt;

    do
    {
        printf("\n\n=== Simple Crypto Menu ===\n");
        printf("1. Encrypt\n");
        printf("2. Decrypt\n");
        printf("3. Return to Main Menu\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1 || choice == 2)
        {
            char inputFileName[MAX_LEN];
            char outputFileName[MAX_LEN];
            char substitution[26];

            printf("Enter key: ");
            scanf("%c", &key);

            if (!isalpha(key))
            {
                printf("Error: key must be a single alphabetic letter.\n");
                continue;
            }

            generateSubstitutionKey(key, substitution);

            while (1)
            {
                printf("Enter the name of the input file: ");
                scanf("%s", inputFileName);

                const FILE *file = fopen(inputFileName, "r");
                if (file == NULL)
                {
                    printf("Error: the file %s does not exist. Please enter a valid file name.\n", inputFileName);
                }
                else
                {
                    break;
                }
            }

            printf("Enter the name of the output file: ");
            scanf("%s", outputFileName);

            // encrypt or decrypt the file
            if (choice == 1)
            {
                encrypt = 1;
            }
            else
            {
                encrypt = 0;
            }
            processMessage(substitution, inputFileName, outputFileName, encrypt);
        }
        else if (choice != 3)
        {
            printf("Invalid choice. Please try again.\n\n");
            break;
        }
    } while (choice != 3);
}

// Generate the substitution string based on user entered key.
void generateSubstitutionKey(char key, char *substitution)
{
    key = toupper(key);
    const int start = key - 'A';

    for (int i = 0; i < 26; i++)
    {
        substitution[i] = 'A' + (start + i) % 26;
    }
}

// Encrypt or decrypt the message.
void processMessage(const char *substitution, const char *inputFileName, const char *outputFileName, int encrypt)
{
    FILE *inputFile = fopen(inputFileName, "r");
    FILE *outputFile = fopen(outputFileName, "w");

    char inverseSubstitution[26];
    const char alphabet[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < 26; i++)
    {
        inverseSubstitution[substitution[i] - 'A'] = alphabet[i];
    }

    char ch;
    while ((ch = fgetc(inputFile)) != EOF)
    {
        if (ch >= 'A' && ch <= 'Z')
        {
            if (encrypt)
            {
                fputc(substitution[ch - 'A'], outputFile);
            }
            else
            {
                fputc(inverseSubstitution[ch - 'A'], outputFile);
            }
        }
        else if (ch >= 'a' && ch <= 'z')
        {
            if (encrypt)
            {
                fputc(substitution[ch - 'a'] + 32, outputFile);
            }
            else
            {
                fputc(inverseSubstitution[ch - 'a'] + 32, outputFile);
            }
        }
        else
        {
            fputc(ch, outputFile);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
    printf("Operation completed successfully. Output written to %s\n", outputFileName);
}