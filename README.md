# Project: Password Manager

Group Members : Ivan Rao, Gene Ye, Alex Zhang

## Description: 
We will create a "password manager" that will allow the 
user to store account information such as emails used 
for the account, usernames, and passwords. Every user would 
have to create their own account for the password manager 
before they can store information and would have to log in 
to have access to the information.

## Technical Design:
The password manager will be used through the terminal. The
user can communicate with the password manager by inputting
certain commands to add, remove, or modify data. There might 
be extra commands once the main features are working.

### Topics from class:
-Processes (fork, exec etc.)
  We will use forks to create a child process that executes the 
  main part of the program while the parent process waits and loops
  so the password manager continues until exited. 

-Allocating Memory:
  We will allocate memory for the account structs that will hold
  the account information. By allocating memory for the structs,
  we can pass on the pointer to functions and the functions will
  be able to the change the structs.

-Working with files:
  We will use files to store the account information for the
  password manager and the other accounts you store. We will create
  a file whenever someone creates an account. The files will be opened 
  and read to print  the account information. 

-Signals:
  We will use signals to control how exiting works so that the 
  user can back out to the main menu and the program can continue
  to provide functionality.

-Finding information with files (minor part of the program):
  We will use information from the files to tell the user information such
  as when the account information was added or last updated.

### Data Structures and Algorithms used:
-Structs:
  When we read the file for an account, we will store all of the 
  information into a struct so that we don't have to go back to 
  the file. This will also let us change the account information
  before we update the information in the file. The struct will
  have the acc_type, email, username, password, and the time 
  added/updated.

### Breakdown of parts:
-Ivan will work setting up the fork so that the program 
continues to prompt the user until the user quits.

-Gene will work on creating functionality to add/change/remove 
account information and finding information from the files. 

-Alex will work on making it so that signals will allow 
the user to log out to the main menu as well as creating the 
struct and making sure it works as intended.

-Everyone will help when needed to create and debug the 
functions necessary for the password manager to work.

## Timeline:
By 1/15: Have the main menu working so the user is able to create 
and account and log in. The program should also loop when the child 
process is done.

By 1/18: Have the account struct set up and have the functions needed
to add, change, and remove accounts.

By 1/21: Implement the signals so the user is able to log out and go 
back to the main menu

By 1/23 or the due date: Make sure there are no bugs and the program
is working as it should.
