#include <stdio.h>
//#include <ctype.h>
//#include <string.h>
#include "simple_crypto/simple_crypto.h"
#include "employee_directory/employee_directory.h"

int main(void) {
    int choice;

    while (1)
    {
        printf("\n=== CS 6740 Main Menu ===\n");
        printf("1. Simple Crypto\n");
        printf("2. Employee Directory\n");
        printf("3. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            cryptoStart();
            break;

        case 2:
            employeeDirectoryStart();
            break;

        case 3:
            printf("Exiting...\n");
            return 0;

        default:
            printf("Invalid choice. Please try again.\n\n");
            break;
        }
    } // End while loop
    return 0;
} // End main
