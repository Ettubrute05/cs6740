This repo is for the projects assigned in cs 6740.

# How to Compile
I used CLion to compile this project, but you should be able to use gcc on file at the following path:
cs6740_submission/cmake-build-debug/cs6740

To run the program use the command ./cs6740 in the cmake-build-debug directory.

The program itself has prompts to navigate, and it should be self explanatory.

The password is "admin1234"

# Logic
The program operates as a setuid executable, allowing it to run with root permissions when editing the password.txt,
rather than the user executing it. When a user runs the program with their own permissions the setuid bit grants them
elevated privileges. The program includes functions for managing an employee directory, requiring user authentication
to modify records. It securely prompts for passwords without showing the user's input on the screen and verifies the
passwords match before updating the password file. The permissions to the password file are only changed right before
and right after actually changing the password. This ensures that sensitive operations are executed securely while
maintaining the principle of least privilege.