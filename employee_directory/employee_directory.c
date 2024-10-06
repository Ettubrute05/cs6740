#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "employee_directory.h"
#include <errno.h>
#include <sys/stat.h>

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

        default:
            printf("Invalid option. Please try again.\n");
        }
    } while (choice != 3);
}

// View the employee directory
void viewDirectory()
{
    createFileIfNotExists(DIRECTORY_FILE);
    FILE *directoryFile = fopen(DIRECTORY_FILE, "r");
    char line[EMPLOYEE_RECORD_SIZE];

    if (directoryFile == NULL)
    {
        printf("Error: Unable to open employee directory.\n");
        return;
    }

    printf("\n--- Employee Directory ---\n");
    printf("%-20s %-20s %-20s %-10s %-15s\n", "Last Name", "First Name", "Position", "Emp ID", "Telephone");
    printf("------------------------------------------------------------------------------------\n");

    // Loop through each line of the file
    while (fgets(line, sizeof(line), directoryFile))
    {
        struct Employee emp;
        // Tokenize each part of the line using comma as delimiter
        const char *token = strtok(line, ",");

        // Extract the last name
        if (token != NULL)
        {
            strncpy(emp.lastName, token, sizeof(emp.lastName) - 1);
            emp.lastName[sizeof(emp.lastName) - 1] = '\0';
        }

        // Extract the first name
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            strncpy(emp.firstName, token, sizeof(emp.firstName) - 1);
            emp.firstName[sizeof(emp.firstName) - 1] = '\0';
        }

        // Extract the position
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            strncpy(emp.position, token, sizeof(emp.position) - 1);
            emp.position[sizeof(emp.position) - 1] = '\0';
        }

        // Extract the employee ID and convert it to an int
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            emp.employeeID = strtol(token, NULL, 10);
        }

        // Extract the telephone number
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            strncpy(emp.telephone, token, sizeof(emp.telephone) - 1);
            emp.telephone[sizeof(emp.telephone) - 1] = '\0';
        }

        // Print the data into formatted columns
        printf("%-20s %-20s %-20s %-10d %-15s\n", emp.lastName, emp.firstName, emp.position, emp.employeeID, emp.telephone);
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

    printf("Enter password: ");
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


    int passwordsMatch = 0;
    char newPassword[50];
    while (!passwordsMatch)
    {
        char confirmPassword[50];

        printf("Enter new password: ");
        getPassword(newPassword, sizeof(newPassword));

        printf("Reenter new password to confirm: ");
        getPassword(confirmPassword, sizeof(confirmPassword));

        if (strcmp(newPassword, confirmPassword) == 0)
        {
            passwordsMatch = 1;
        }
        else
        {
            printf("Passwords don't match, Please try again.\n");
        }
    }

    // Temporarily change the file permissions to 600 to change the password
    if (chmod(PASSWORD_FILE, 600) != 0)
    {
        printf("Error: Unable to set password file permissions to 600.\n");
        return;
    }

    // Open the file
    FILE *passwordFile = fopen(PASSWORD_FILE, "w");
    if (passwordFile == NULL)
    {
        printf("Error: Unable to open password file.\n");
        return;
    }

    // Write the new password to th file
    fprintf(passwordFile, "%s", newPassword);
    fclose(passwordFile);

    if (chmod(PASSWORD_FILE, 0400) != 0)
    {
        printf("Error: Unable to set password file permissions to 400.\n");
    }
    else
    {
        printf("Password changed successfully.\n");
    }

}

// Add a new employee
void addEmployee()
{
    struct Employee emp;
    int isIDUnique = 0;
    struct Employee employees[MAX_EMPLOYEES];
    int employeeCount = 0;
    if (!loadEmployees(employees, &employeeCount))
    {
        printf("Error loading employee directory.\n");
        return;
    }
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

    while (!isIDUnique)
    {
        char idInput[20];
        char *endptr;

        printf("Enter employee ID: ");
        //scanf("%d", emp.employeeID);
        //getchar(); // Consume newline
        fgets(idInput, sizeof(idInput), stdin);
        idInput[strcspn(idInput, "\n")] = '\0';

        errno = 0;
        const long tempID = strtol(idInput, &endptr, 10);

        // Checking for valid inputs
        if (errno != 0 || *endptr != '\0' || tempID < 100000 || tempID > 999999)
        {
            printf("Invalid Employee ID. Please enter a valid 6-digit number.\n");
            continue;
        }

        emp.employeeID = (int)tempID;

        // Check if the employee ID is already in use
        if (findEmployeeByID(employees, employeeCount, emp.employeeID) != -1)
        {
            printf("Employee ID already in use. Please try again.\n");
        }
        else
        {
            isIDUnique = 1;
        }
    }

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
    const int index = findEmployeeByID(employees, employeeCount, employeeID);
    if (index == -1)
    {
        printf("Employee with ID %d not found.\n", employeeID);
        return;
    }

    // Prompt for new details
    int choice;
    do
    {
        printf("\n--- Editing record for employee with ID %06d ---\n", employeeID);
        printf("1. Change last name, current: %s\n", employees[index].lastName);
        printf("2. Change first name, current %s\n", employees[index].firstName);
        printf("3. Change position, current: %s\n", employees[index].position);
        printf("4. Change telephone number, current: %s", employees[index].telephone);
        printf("5. Return to employee directory menu\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            printf("Enter new last name: ");
            fgets(employees[index].lastName, 50, stdin);
            employees[index].lastName[strcspn(employees[index].lastName, "\n")] = '\0';
            break;

        case 2:
            printf("Enter new first name: ");
            fgets(employees[index].firstName, 50, stdin);
            employees[index].firstName[strcspn(employees[index].firstName, "\n")] = '\0';
            break;

        case 3:
            printf("Enter new position: ");
            fgets(employees[index].position, 50, stdin);
            employees[index].position[strcspn(employees[index].position, "\n")] = '\0';
            break;

        case 4:
            printf("Enter new telephone number: ");
            fgets(employees[index].telephone, 15, stdin);
            employees[index].telephone[strcspn(employees[index].telephone, "\n")] = '\0';
            break;

        case 5:
            break;

        default:
            printf("Invalid option. Please try again.\n");
        }
    } while (choice != 5);

    // Save updated employees back to the file
    saveEmployees(employees, employeeCount);

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

    char line[EMPLOYEE_RECORD_SIZE];
    *employeeCount = 0;

    // Loop through each line of the file
    while (fgets(line, sizeof(line), directoryFile) != NULL)
    {
        if (*employeeCount >= MAX_EMPLOYEES)
        {
            printf("Maximum employee count reached. Some employees may not be loaded.\n");
            break;
        }

        // Tokenize the line
        const char *token = strtok(line, ",");

        // Extract the last name
        if (token != NULL)
        {
            strncpy(employees[*employeeCount].lastName, token, sizeof(employees[*employeeCount].lastName) - 1);
            employees[*employeeCount].lastName[sizeof(employees[*employeeCount].lastName) - 1] = '\0';
        }

        // Extract the first name
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            strncpy(employees[*employeeCount].firstName, token, sizeof(employees[*employeeCount].firstName) - 1);
            employees[*employeeCount].firstName[sizeof(employees[*employeeCount].firstName) - 1] = '\0';
        }

        // Extract the position
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            strncpy(employees[*employeeCount].position, token, sizeof(employees[*employeeCount].position) - 1);
            employees[*employeeCount].position[sizeof(employees[*employeeCount].position) - 1] = '\0';
        }

        // Extract the employee ID and convert it to an int
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            employees[*employeeCount].employeeID = strtol(token, NULL, 10);
        }

        // Extract the telephone number
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            strncpy(employees[*employeeCount].telephone, token, sizeof(employees[*employeeCount].telephone) - 1);
            employees[*employeeCount].telephone[sizeof(employees[*employeeCount].telephone) - 1] = '\0';
        }

        // Increment employee count
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
        fprintf(directoryFile, "%s,%s,%s,%06d,%s", employees[i].lastName,
                employees[i].firstName, employees[i].position, employees[i].employeeID,
                employees[i].telephone);
    }
    fclose(directoryFile);
}

// Find index of employee by ID in the array
int findEmployeeByID(struct Employee employees[], const int employeeCount, const int employeeID)
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

// Create the file if it doesn't exist
void createFileIfNotExists(const char *filename)
{
    FILE *fp = fopen(filename, "a");
    if (fp != NULL)
    {
        fclose(fp);
    }
}