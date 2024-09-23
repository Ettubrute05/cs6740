#include <stdio.h>
//#include <ctype.h>
//#include <string.h>
#include "simple_crypto/simple_crypto.h"

int main(void) {
    int choice;

    while (1)
    {
        printf("=== CS 6705 Main Menu ===\n");
        printf("1. Simple Crypto\n");
        printf("2. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            cryptoStart();
            break;

        case 2:
            printf("Exiting...\n");
            return 0;

        default:
            printf("Invalid choice. Please try again.\n\n");
            break;
        }
    } // End while loop
    return 0;
} // End main
