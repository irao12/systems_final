#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include "functions.h"
#include "menu.h"

//adds a new account type that contains user/password/email into struct
struct acct * new_entry (char * type, char* usr, char *pw, char* mail) {
  struct acct * newentry;
  time_t curtime;
  time(&curtime);
  char buffer[50];
  
  newentry = malloc(sizeof(struct acct));
  strncpy (newentry->acctType, type, sizeof(newentry->acctType)-1);
  strncpy (newentry->username, usr, sizeof(newentry->username)-1);
  //strcat(newentry->username, ",");
  strncpy (newentry->password, pw, sizeof(newentry->password)-1);
  //strcat(newentry->password, ",");
  strncpy (newentry->email, mail, sizeof(newentry->email)-1);
  //strcat(newentry->email, ",");
  strncpy (newentry->lastUpdated, ctime_r(&curtime,buffer), sizeof(newentry->lastUpdated)-1);

  return newentry;
}

//prints entry
void printEntry (char * path, char * user){
  char x [100];
  strncpy(x, "data/", sizeof(x)-1);
  strncat(x, user, sizeof(x)-1);
  strncat(x, "/", sizeof(x)-1);
  strncat(x, path, sizeof(x)-1);
  remover(x);
  int i;
  int fd = open(x, O_RDONLY);
  if (fd < 0) {
    printf("\nEntry \"%s\" does not exist\n", path);
    return;
  }
  char line[100];
  char * y = line;
  char * token, *p;
  FILE* fp;
  fp = fopen(x, "r");
  fgets(y, 100, fp);
  p = y;
  token = strsep(&p,",");
  printf("\nAccount type: ");
  printf("%s\n", token);
  token = strsep(&p,",");
  printf("Username: ");
  printf("%s\n", token);
  token = strsep(&p,",");
  printf("Password: ");
  printf("%s\n", token);
  token = strsep(&p,",");
  printf("Email: ");
  printf("%s\n", token);
  token = strsep(&p,",");
  printf("Last updated: ");
  printf("%s\n", token);  
}

//converts struct and puts data into the file
void convert(struct acct *anEntry, char * user){
  char loc[100];
  strncpy(loc, "data/", sizeof(loc)-1);
  strcat(loc, user);
  strcat(loc, "/");
  strcat(loc, (anEntry->acctType));
  remover(loc);

  int fd = open(loc, O_CREAT | O_RDWR, 0666);

  if (fd < 0) {
    printf("error: %s\n",strerror(errno));
  }

  char info[100];
  strncpy(info, anEntry->acctType, sizeof(info)-1);
  strncat(info, ",", sizeof(info)-1);
  strncat(info,anEntry->username, sizeof(info)-1);
  strncat(info, ",", sizeof(info)-1);
  strncat(info,anEntry->password, sizeof(info)-1);
  strncat(info, ",", sizeof(info)-1);
  strncat(info,anEntry->email, sizeof(info)-1);
  strncat(info, ",", sizeof(info)-1);
  strncat(info,anEntry->lastUpdated, sizeof(info)-1);
  
  write(fd, info, sizeof(info));
  close(fd);
}

void see (char * user) {
  char text [100];
  printf("\nWhat entry would you like to view the contents of?\n");
  fgets(text, sizeof(text), stdin);
  remover(text);
  printEntry(text, user);
}

void add (char * userAcct) {
  char text [100], type [100], usr [100], pw [100], mail [100], prompt [10], loc [100];
  int con = 1;
  struct acct *acc_entry;
  while (con) {
    while (1) {
      printf("\nWhat account would you like to add?\n");
      fgets(type, sizeof(type), stdin);
      remover(type);
      strncpy(loc, "data/", sizeof(loc)-1);
      strncat(loc, userAcct, sizeof(loc)-1);
      strncat(loc, "/", sizeof(loc)-1);
      strncat(loc, type, sizeof(loc)-1);
      int fd = open(loc, O_RDONLY);
      if (fd < 0) {
        break;
      }
      else printf("\nAn Account with this type already exists\n");
    }
          
    printf("\nWhat is the username of said account?\n");
    fgets(usr,sizeof(usr),stdin);
    remover(usr);     
      
    printf("\nWhat is the password of said account?\n");
    fgets(pw,sizeof(pw),stdin);
    remover(pw);
            
    printf("\nWhat is your email of said account?\n");
    fgets(mail,sizeof(mail),stdin);
    remover(mail);
        
    acc_entry = new_entry(type,usr,pw,mail);
    printf("\nAccount type: %s\nUsername: %s\nPassword: %s\nEmail: %s\n", acc_entry->acctType, acc_entry->username, acc_entry->password, acc_entry->email);
    printf("\nIs this correct? \"yes\" or \"no\"\n");
          
    while(1){
      fgets(prompt,sizeof(prompt),stdin);
      remover(prompt);
      if(!strcmp(prompt,"yes")){
        con = 0;
        break;
      }
      else if (!strcmp(prompt,"no")) {
        printf ("Please correctly enter the information\n");
        break;
      }
      else printf("Is this correct? \"yes\" or \"no\"\n");
    }
  }
convert(acc_entry,userAcct);    

printf("\nEntry added\n");
}

//account_type-what type of account it is
//updates one part of an entry (email,password,user,etc) - entry_type
//takes argument for string with data

//updates one part of an account's entry

void update (char * user, char * new_data, char * account_type, char* entry_type) {
  
  struct acct * temp_entry = malloc(sizeof(struct acct));
  char * username;
  char * password;
  char * email;

  char loc [100];
  strncpy(loc, "data/", sizeof(loc)-1);
  strncat(loc, user, sizeof(loc)-1);
  strncat(loc, "/", sizeof(loc)-1);
  strncat(loc, account_type, sizeof(loc)-1);
  remover(loc);
  
  char line [100];
  char * y = line;
  char * token, * p;
  FILE * fp;
  fp = fopen(loc, "r");
  fgets (y, 100, fp);
  p = y;
  fclose(fp);
  //Account type, username, password, email, last updated

  if (!strcmp(entry_type, "username")) {
    strsep (&p, ",");
    username = new_data;
    strsep (&p,",");
    password = strsep (&p, ",");
    email = strsep(&p,",");
  }
  if (!strcmp(entry_type, "password")) {
    strsep (&p, ",");
    username = strsep (&p, ",");
    password = new_data;
    strsep (&p, ","); 
    email = strsep(&p, ",");
  }
  if (!strcmp(entry_type, "email")) {
    strsep (&p, ",");
    username = strsep (&p, ",");
    password = strsep (&p, ",");
    email = new_data;
    strsep (&p, ",");
  }

  temp_entry = new_entry (account_type, username, password, email);
  printf("\nAccount type: %s\nUsername: %s\nPassword: %s\nEmail: %s\n", temp_entry->acctType, temp_entry->username, temp_entry->password, temp_entry->email);
  printf("\nIs this correct? \"yes\" or \"no\"\n");
  
  char prompt [10];
  while (1) {
    fgets (prompt, sizeof(prompt), stdin);
    remover(prompt);
    if(!strcmp(prompt, "yes")) {
      convert (temp_entry, user);
      printf ("Update successful\n");
      break;
    }
    else if (!strcmp(prompt, "no")) {
      printf ("Update cancelled\n");
      break;
    }
    else printf("Is this correct? \"yes\" or \"no\"\n");
  }
}


void update_start(char * user){
  char update_acc [100];
  char update_entry [100];
  char update_data [100];

    while (1) {
      printf ("\nWhat account would you like to update?\n");
      fgets (update_acc, sizeof(update_acc), stdin);
            remover (update_acc);

      char loc [100];
      strncpy (loc, "data/", sizeof(loc)-1);
      strncat (loc, user, sizeof(loc)-1);
      strncat (loc, "/", sizeof(loc)-1);
      strncat (loc, update_acc,sizeof(loc)-1);
      int fd = open (loc, O_RDONLY);
      if (fd < 0) {
        printf("\nNo such account, please re-enter");
        memset (loc, 0, sizeof(loc));
      }
      else {
        break;
      }
    }

    printEntry (update_acc, user);

    while (1) {
      printf ("What entry would you like to update: \"username\" \"password\" \"email\"\n"); 
      fgets (update_entry, sizeof(update_entry), stdin);
      remover (update_entry);

      if (!strcmp(update_entry, "username") || !strcmp(update_entry, "password") || !strcmp(update_entry, "email")) {
         break;
      }
      else {
        printf("\nPlease enter a valid entry\n");
        memset (update_acc, 0, sizeof(update_entry));
      }
    }

    printf ("\nWhat would you like to update it to?\n");
    fgets (update_data, sizeof(update_data), stdin);
    remover (update_data);

    update (user, update_data, update_acc, update_entry);
}

void list(char *user){
  char loc[100];
  DIR *d;
  struct dirent *entry;
  
  printf("\n");
  printf("Listing current entries...\n");
  remover(user);
  strncpy(loc, "data/", sizeof(loc)-1);
  strncat(loc, user, sizeof(loc)-1);
  int fd=open(loc,O_RDONLY);
  if (fd == -1)
    {
      printf("%s\n", strerror(errno));
    }
  else
    {
      d = opendir(loc);
      entry = readdir(d);
    }
  while (entry != NULL){
    if (entry->d_type==DT_REG){
      printf("%s\n",entry->d_name);
    }
    entry=readdir(d);
  }
  closedir(d);

}

void remove_entry (char * user){
  char loc[100], entry[100];
  int con = 1;
  while (con){
    strncpy(loc, "data/", sizeof(loc)-1);
    strncat(loc, user, sizeof(loc)-1);
    strncat(loc, "/", sizeof(loc)-1);
    printf("\nWhich entry would you like to remove\n");
    fgets(entry, sizeof(entry), stdin);
    remover(entry);
    strncat(loc, entry, sizeof(loc)-1);
    int fd = open(loc, O_RDONLY);
    if (fd < 0){
      printf("\nThe entry \"%s\" does not exist\n", entry);
    }
    else {
      char prompt[50];
      while (1){
        printf("\nAre you sure you want to remove this entry, \"yes\" or \"no\"?\n");
        fgets(prompt, sizeof(prompt),stdin);
        remover(prompt);
        if (!strcmp(prompt, "yes")){
          close(fd);
          remove(loc);
          printf("\nEntry was removed\n");
          con = 0;
          break;
        }
        else if (!strcmp(prompt, "no")){
          close(fd);
          con = 0;
          break;
        }
      }
    }
  }
}