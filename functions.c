#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include "functions.h"
#include "menu.h"

//adds a new account type that contains user/password/email into struct
struct acct *new_entry(char *type, char *usr, char *pw, char *mail)
{
  struct acct *newentry;
  time_t curtime;
  time(&curtime);
  char buffer[50];

  newentry = malloc(sizeof(struct acct));
  strncpy(newentry->acctType, type, sizeof(newentry->acctType) - 1);
  strncpy(newentry->username, usr, sizeof(newentry->username) - 1);
  //strcat(newentry->username, ",");
  strncpy(newentry->password, pw, sizeof(newentry->password) - 1);
  //strcat(newentry->password, ",");
  strncpy(newentry->email, mail, sizeof(newentry->email) - 1);
  //strcat(newentry->email, ",");
  strncpy(newentry->lastUpdated, ctime(&curtime), sizeof(newentry->lastUpdated) - 1);

  return newentry;
}

//prints entry
void printEntry(char *path, char *user)
{
  char x[100];
  strncpy(x, "data/", sizeof(x) - 1);
  strncat(x, user, sizeof(x) - 1);
  strncat(x, "/", sizeof(x) - 1);
  strncat(x, path, sizeof(x) - 1);
  remover(x);
  int i;
  int fd = open(x, O_RDONLY);
  if (fd < 0)
  {
    printf("error: %s\n", strerror(errno));
  }
  char line[100];
  char *y = line;
  char *token, *p;
  FILE *fp;
  fp = fopen(x, "r");
  fgets(y, 100, fp);
  p = y;
  token = strsep(&p, ",");
  printf("\nAccount type: ");
  printf("%s\n", token);
  token = strsep(&p, ",");
  printf("Username: ");
  printf("%s\n", token);
  token = strsep(&p, ",");
  printf("Password: ");
  printf("%s\n", token);
  token = strsep(&p, ",");
  printf("Email: ");
  printf("%s\n", token);
}

//converts struct and puts data into the file
void convert(struct acct *anEntry, char *user)
{
  char loc[100];
  strncpy(loc, "data/", sizeof(loc) - 1);
  strcat(loc, user);
  strcat(loc, "/");
  strcat(loc, (anEntry->acctType));
  remover(loc);

  int fd = open(loc, O_CREAT | O_RDWR, 0666);

  if (fd < 0)
  {
    printf("error: %s\n", strerror(errno));
  }

  char info[100];
  strncpy(info, anEntry->acctType, sizeof(info) - 1);
  strncat(info, ",", sizeof(info) - 1);
  strncat(info, anEntry->username, sizeof(info) - 1);
  strncat(info, ",", sizeof(info) - 1);
  strncat(info, anEntry->password, sizeof(info) - 1);
  strncat(info, ",", sizeof(info) - 1);
  strncat(info, anEntry->email, sizeof(info) - 1);

  write(fd, info, sizeof(info));
  close(fd);
}

//accountType-what type of account it is
//updates one part of an entry (email,password,user,etc) - entryType
//takes argument for string with data

/*
void update(char* newdata, char* entryType, char* accountType, char* user) {
  struct acct *newentry=malloc(sizeof(struct acct));
  char* username;
  char* password;
  char* email;

  char x[100];
  strncat(x, "data/", sizeof(x)-1);
  strncat(x, user, sizeof(x)-1);
  strncat(x, "/", sizeof(x)-1);
  strcat(x, accountType);
  int fd= open(x,O_RDWR);
  char *curr;
  read(fd,curr,sizeof(curr));
  if (strcmp(entryType, "username")){
   username = newdata;
   strsep(&curr,',');
   password= strsep(&curr,',');
   email=strsep(&curr,',');
  }
  if (strcmp(entryType, "password")){
    username=strsep(&curr,',');
    password=newdata;
    strsep(&curr,','); 
    email=strsep(&curr,','); 
  }
  if (strcmp(entryType, "email")){
    username=strsep(&curr,',');
    password=strsep(&curr,',');
    email=newdata;
  }

  newentry=new_entry(accountType, username, password, email);
  convert(newentry);
}

*/

void add(char *userAcct)
{
  char text[100], type[100], usr[100], pw[100], mail[100], prompt[10], loc[100];
  int con = 1;
  struct acct *acc_entry;
  while (con)
  {
    while (1)
    {
      printf("\nWhat account would you like to add?\n");
      fgets(type, sizeof(type), stdin);
      remover(type);
      strncpy(loc, "data/", sizeof(loc) - 1);
      strncat(loc, userAcct, sizeof(loc) - 1);
      strncat(loc, "/", sizeof(loc) - 1);
      strncat(loc, type, sizeof(loc) - 1);
      int fd = open(loc, O_RDONLY);
      if (fd < 0)
      {
        break;
      }
      else
        printf("\nAn Account with this type already exists\n");
    }

    printf("\nWhat is the username of said account?\n");
    fgets(usr, sizeof(usr), stdin);
    remover(usr);

    printf("\nWhat is the password of said account?\n");
    fgets(pw, sizeof(pw), stdin);
    remover(pw);

    printf("\nWhat is your email of said account?\n");
    fgets(mail, sizeof(mail), stdin);
    remover(mail);

    acc_entry = new_entry(type, usr, pw, mail);
    printf("\nAccount type: %s\nUsername: %s\nPassword: %s\nEmail: %s\n", acc_entry->acctType, acc_entry->username, acc_entry->password, acc_entry->email);
    printf("\nIs this correct? \"yes\" or \"no\"\n");

    while (1)
    {
      fgets(prompt, sizeof(prompt), stdin);
      remover(prompt);
      if (!strcmp(prompt, "yes"))
      {
        con = 0;
        break;
      }
      else if (!strcmp(prompt, "no"))
      {
        printf("Please correctly enter the information\n");
        break;
      }
      else
        printf("Is this correct? \"yes\" or \"no\"\n");
    }
  }
  convert(acc_entry, userAcct);

  printf("\nEntry added\n");
}

void see(char *user)
{
  char text[100];
  printf("\nWhat entry would you like to view the contents of?\n");
  fgets(text, sizeof(text), stdin);
  remover(text);
  printEntry(text, user);
}
