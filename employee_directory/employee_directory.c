#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "employee_directory.h"

void employeeDirectoryStart()
{
    int choice;

    do
    {
        printf("\n--- Employee Directory Menu ---\n");
        printf("1. Modify Employee Directory (Password Required)\n");
        printf("2. View Employee Directory\n");
        printf("3. Return to Main Menu\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); // consume newline character

        switch (choice)
        {
        case 1:
            if (checkPassword())
            {
                modifyDirectory();
            }
            else
            {
                printf("Incorrect password. Returning to menu.\n");
            }
            break;

        case 2:
            viewDirectory();
            break;

        case 3:
            return;

        defaule:
            printf("Invalid option. Please try again.\n");
        }
    } while (choice != 3);
}

// View the employee directory
void viewDirectory()
{
    FILE *directoryFile = fopen(DIRECTORY_FILE, "r");
    char line[EMPLOYEE_RECORD_SIZE];
    struct Employee emp;

    if (directoryFile == NULL)
    {
        printf("Error: Unable to open employee directory.\n");
        return;
    }

    printf("\n--- Employee Directory ---\n");
    printf("%-20s %-20s %-20s %-10s %-15s", "Last Name", "First Name", "Position", "Emp ID", "Telephone");
    printf("------------------------------------------------------------------------------------");

    // Loop through each line of the file
    while (fgets(line, sizeof(line), directoryFile))
    {
        // Parse the comma-separated values into the Employee struct
        sscanf(line, "%[^,],%[^,],%[^,],%d,%[^\n]", emp.lastName, emp.firstName, emp.position, emp.employeeID, emp.telephone);

        // Print the data from the Employee struct in formatted columns
        printf("%-20s %-20s %-20s %-10s %-15s\n", emp.lastName, emp.firstName, emp.position, emp.employeeID, emp.telephone);
    }
    fclose(directoryFile);
}

// Modify the employee directory
void modifyDirectory()
{
    int choice;
    do
    {
        printf("\n--- Modify Employee Directory ---\n");
        printf("1. Add Employee\n");
        printf("2. Edit Employee\n");
        printf("3. Delete Employee\n");
        printf("4. Change Password\n");
        printf("5. Return to Directory Menu\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice)
        {
        case 1:
            addEmployee();
            break;

        case 2:
            editEmployee();
            break;

        case 3:
            deleteEmployee();
            break;

        case 4:
            changePassword();
            break;

        case 5:
            return;

        default:
            printf("Invalid option. Please try again.\n");
        }
    } while (choice != 5);
}

// Check the user entered password
int checkPassword()
{
    char storedPassword[50];
    char enteredPassword[50];
    FILE *passwordFile = fopen(PASSWORD_FILE, "r");

    if (passwordFile == NULL)
    {
        printf("Password file not found.");
    }
    else
    {
        fscanf(passwordFile, "%s", storedPassword);
        fclose(passwordFile);
    }

    printf("Enter new password: ");
    getPassword(enteredPassword, sizeof(enteredPassword));

    if (strcmp(storedPassword, enteredPassword) == 0)
    {
        return 1;
    }
    return 0;
}

// Change the password
void changePassword()
{
    const FILE *passwordFile = fopen(PASSWORD_FILE, "w");

    if (passwordFile == NULL)
    {
        printf("Error: Unable to open password file.\n");
        return;
    }

    while (1)
    {
        char newPassword[50];
        char confirmPassword[50];

        printf("Enter new password: ");
        getPassword(newPassword, sizeof(newPassword));

        printf("Reenter password to confirm: ");
        getPassword(confirmPassword, sizeof(confirmPassword));

        if (strcmp(newPassword, confirmPassword) == 1)
        {
            break;
        }
        else
        {
            printf("Passwords don't match, Please try again.\n");
        }
    }

    printf("Password changed successfully.\n");
}

// Add a new employee
void addEmployee()
{
    struct Employee emp;
    int verifyID = 0;
    struct Employee employees[MAX_EMPLOYEES];
    int *employeeCount;
    loadEmployees(employees, *employeeCount);
    FILE *directoryFile = fopen(DIRECTORY_FILE, "a");

    if (directoryFile == NULL)
    {
        printf("Error: Unable to open employee directory.\n");
        return;
    }

    printf("Enter employee last name: ");
    fgets(emp.lastName, 50, stdin);
    emp.lastName[strcspn(emp.lastName, "\n")] = '\0';

    printf("Enter employee first name: ");
    fgets(emp.firstName, 50, stdin);
    emp.firstName[strcspn(emp.firstName, "\n")] = '\0';

    printf("Enter employee position: ");
    fgets(emp.position, 50, stdin);
    emp.position[strcspn(emp.position, "\n")] ='\0';

    do
    {
        int tempID;
        printf("Enter employee ID: ");
        scanf("%d", &tempID);
        getchar(); // Consume newline
        if (findEmployeeByID(employees, employeeCount, tempID) == -1)
        {
            printf("Error: Employee ID already in use. Please try again.\n");
        }
        else
        {
            verifyID = 1;
        }
    } while (verifyID != 1);

    printf("Enter employee telephone number: ");
    fgets(emp.telephone, 15, stdin);
    emp.telephone[strcspn(emp.telephone, "\n")] = '\0';

    // Write the employee record to the file
    fprintf(directoryFile, "%s,%s,%s,%06d,%s\n", emp.lastName, emp.firstName, emp.position, emp.employeeID, emp.telephone);
    fclose(directoryFile);

    printf("Employee added successfully.\n");
}

// Delete an existing employee
void deleteEmployee()
{
    struct Employee employees[MAX_EMPLOYEES];
    int employeeCount = 0;
    int employeeID;

    if (!loadEmployees(employees, &employeeCount))
    {
        printf("Error loading employees from directory file.\n");
        return;
    }

    // Prompt for the Employee ID
    printf("Enter Employee ID to delete: ");
    scanf("%d", &employeeID);
    getchar();

    // Find employee by ID
    const int index = findEmployeeByID(employees, employeeCount, employeeID);
    if (index == -1)
    {
        printf("Employee with ID %d not found.\n", employeeID);
        return;
    }
    for (int i = index; i < employeeCount -1; i++)
    {
        employees[i] = employees[i + 1];
    }
    employeeCount--;

    // Save updated employees back to the file
    saveEmployees(employees, employeeCount);

    printf("Employee with ID %06d deleted successfully.\n", employeeID);
}

// Edit an existing employee
void editEmployee()
{
    struct Employee employees[MAX_EMPLOYEES];
    int employeeCount = 0;
    int employeeID;
    int index;

    // Load employees from file
    if (!loadEmployees(employees, &employeeCount))
    {
        printf("Error loading employees from directory file.\n");
        return;
    }

    // Prompt for the employee ID
    printf("Enter employee ID to edit: ");
    scanf("%d", &employeeID);
    getchar();

    // Find employee by ID
    index = findEmployeeByID(employees, employeeCount, employeeID);
    if (index == -1)
    {
        printf("Employee with ID %d not found.\n", employeeID);
        return;
    }

    // Prompt for new details
    int choice;
    do
    {
        printf("Editing employee with ID %06d:\n", employeeID);
        printf("1. Change last name (current: %s)\n", employees[index].lastName);
        printf("2. Change first name (current %s)\n", employees[index].firstName);
        printf("3. Change position (current: %s)\n", employees[index].position);
        printf("4. Change telephone number (current: %s)\n", employees[index].telephone);
        printf("5. Return to employee directory menu\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter new last name: ");
            fgets(employees[index].lastName, 50, stdin);
            break;

        case 2:
            printf("Enter new first name: ");
            fgets(employees[index].firstName, 50, stdin);
            break;

        case 3:
            printf("Enter new position: ");
            fgets(employees[index].position, 50, stdin);
            break;

        case 4:
            printf("Enter new telephone number: ");
            fgets(employees[index].telephone, 15, stdin);
            break;

        case 5:
            break;

        default:
            printf("Invalid option. Please try again.\n");
        }
    } while (choice != 6);

    printf("Employee record updated successfully.\n");
}

void getPassword(char *password, size_t size)
{
    struct termios oldt, newt;
    int i = 0;
    char c;

    // Get current terminal settings and turn off echo
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Read Password
    while (i < size - 1 && (c = getchar()) != '\n')
    {
        if (c == '\b' || c == 127) // Handle backspace
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            password[i++] = c;
        }
    }
    password[i] = '\0';

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
}

// Load employees from the file into an array
int loadEmployees(struct Employee employees[], int *employeeCount)
{
    FILE *directoryFile = fopen(DIRECTORY_FILE, "r");
    if (directoryFile == NULL)
    {
        return 0;
    }

    *employeeCount = 0;
    while (fscanf(directoryFile, "%[^,],%[^,],%[^,],%d,%[^\n]\n", employees[*employeeCount].lastName,
            employees[*employeeCount].firstName, employees[*employeeCount].position,
            &employees[*employeeCount].employeeID, employees[*employeeCount].telephone) == 5)
    {
        (*employeeCount)++;
    }

    fclose(directoryFile);
    return 1;
}

// Save the employees from the array back into the file
void saveEmployees(struct Employee employees[], int employeeCount)
{
    FILE *directoryFile = fopen(DIRECTORY_FILE, "w");
    if (directoryFile == NULL)
    {
        printf("Error: Unable to open employee directory for writing.\n");
        return;
    }

    for (int i = 0; i < employeeCount; i++)
    {
        fprintf(directoryFile, "%s,%s,%s,%06d,%s\n", employees[i].lastName,
                employees[i].firstName, employees[i].position, employees[i].employeeID,
                employees[i].telephone);
    }
    fclose(directoryFile);
}

// Find index of employee by ID in the array
int findEmployeeByID(struct Employee employees[], int employeeCount, int employeeID)
{
    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i].employeeID == employeeID)
        {
            return i;
        }
    }
    return -1;
}