#ifndef _EMPLOYEE_DIRECTORY_H_
#define _EMPLOYEE_DIRECTORY_H_

#define MAX_EMPLOYEES 100
#define EMPLOYEE_RECORD_SIZE 256
#define PASSWORD_FILE "password.txt"
#define DIRECTORY_FILE "directory.txt"

// Structure to hold employee information
struct Employee
{
    char lastName[50];
    char firstName[50];
    char position[50];
    int employeeID;
    char telephone[15];
};

void employeeDirectoryStart();
void viewDirectory();
void modifyDirectory();
int checkPassword();
void changePassword();
void addEmployee();
void deleteEmployee();
void editEmployee();
void getPassword(char *password, size_t size);
int loadEmployees(struct Employee employees[], int *employeeCount);
void saveEmployees(struct Employee employees[], int employeeCount);
int findEmployeeByID(struct Employee employees[], int employeeCount, int employeeID);
void createFileIfNotExists(const char *filename);

#endif