# Project: Password Manager

Group Members: Ivan Rao, Gene Ye, Alex Zhang (Period 10)

## Description
The Password Manager (not very secure) is utilized through the terminal and allows the user to store account information such as emails, usernames, and passwords used for accounts. Every user would have to create their own account for the password manager before they can store information and would have to log in to have access to the information. 

## Technical Design:
- Processes
- Uses forks to create a child process that executes the main part of the program while the parent process waits and loops so the password manager continues until exited. This allows the two processes two have different responses to signals. 

- Allocating Memory:
  Allocates memory for the account structs that will hold the account information. By allocating memory for the structs, we can pass on the pointer to functions and the functions will be able to the change the structs.

- Working with files:
  Uses files to store the account information for the password manager and the other accounts you store. The files will store informaton and be read to print the account information. 

- Signals:
  Uses signals to control how exiting works so that theuser can back out to the main menu and the program can continue to provide functionality.

## Required Libraries:
- <stdlib.h> 
- <stdio.h>
- <strings.h>
- <signal.h>
- <sys/stat.h>
- <sys/wait.h>
- <fcntl.h>
- <unistd.h>
- <errno.h>
- <time.h>
- <dirent.h>

## Instructions
Note: When the program prompts you to type in something, the user should type whatever is in the quotations without the quotations.
- Ex: If the program prompts you to enter "login", the user would type and enter login

After the program runs, the user enters the main menu. In the main menu, the user has the option to log into their password manager account—by typing "login"—or creating a new password manager account—by typing "create".

Once the user has logged in, they will enter the prompt menu.
In the prompt menu, the user is able to:

1. List the accounts already stored in the password manager
  - by entering "list" while in the prompt menu

2. See an account entry that is stored in the passwor manager
  - by entering "see" while in the prompt menu
  - the user is prompted to enter the account type afterwards
    - the account type must match character for character with what is shown when the accounts are listed. This is the same as the type the user inputted when the account was added

3. Add an account entry to the password manager
 - by entering "add" while in the prompt menu
  - The password manager will prompt the user to input the account type, username, password, and email associated with the account.
    - If the account you are adding doesn't have an email, username, or password associated with it, just enter "0" and it exempt that piece of information when you see an account.
  - The password manager shows a preview of the account entry and ask to confirm the information
    - If it is not confirmed, the user will reinput the information
    - Once it is confirmed, account entry will be stored as a file

4. Update an account entry that was stored in the password manager
 - by entering "update" while in the prompt menu
   - The password manager will prompt you to put the account you want to update. This is the account type.
   - Once an account is entered to update, it's information will be displayed.
   - The user will then be prompted to enter which piece of information (username, password, or email) they want to change.
    - When a piece of information is entered, the user is prompted to input what they want to change it to, and ask the user to confirm if it's correct
    - The password manager then asks if the user is finished updating. If they enter "no", they will be able to change another piece of information. If they enter "yes", they will return to the prompt menu

  5. Remove an account entry that was stored in the password manager
   - By entering "remove" while in the prompt menu
   - the user is prompted to enter the account type they want to remove
   - Once it is confirmed that they want to remove that account entry, the file associated with the account will be removed.
    - Otherwise, nothing happens and the user is prompted to input the account they want to remove again

Note: While the user is listing, seeing, adding, updating,or removing, they can enter "cancel" at any step to return to the prompt menu

- The user can log out of the password manager account and return to the main menu by pressing ctrl+c or sending a SIGINT signal. Pressing ctrl+c while in the main menu will allow the user to exit the password manager


