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
  char buffer[100];
  
  newentry = malloc(sizeof(struct acct));

  strncpy (newentry->acctType, type, sizeof(newentry->acctType)-1);

  strncpy (newentry->username, usr, sizeof(newentry->username)-1);

  strncpy (newentry->password, pw, sizeof(newentry->password)-1);

  strncpy (newentry->email, mail, sizeof(newentry->email)-1);

  strncpy (newentry->lastUpdated, ctime_r(&curtime,buffer), sizeof(newentry->lastUpdated)-1);

  return newentry;
}

//prints entry
void printEntry (char * path, char * user){
  char loc [200];
  strncpy(loc, "data/", sizeof(loc)-1);
  strncat(loc, user, sizeof(loc)-1);
  strncat(loc, "/", sizeof(loc)-1);
  strncat(loc, path, sizeof(loc)-1);
  remover(loc);
  int i;
  int fd = open(loc, O_RDONLY);
  if (fd < 0) {
    printf("\nEntry \"%s\" does not exist\n", path);
    return;
  }

  char line[100];
  char * y = line;
  char * token, *p;

  FILE* fp;
  fp = fopen(loc, "r");
  fgets(y, 100, fp);
  p = y;
  token = strsep(&p,",");
  printf("\nAccount type: %s\n", token);
  
  token = strsep(&p,",");
  trim(token);
  if (strcmp(token, "0")){
    printf("Email: %s\n", token);
  }

  token = strsep(&p,",");
  trim(token);
  if (strcmp(token,"0")){
    printf("Username: %s\n", token);
  }

  token = strsep(&p,",");
  trim(token);
  if (strcmp(token,"0")){
    printf("Password: %s\n", token);
  }

  token = strsep(&p,",");
  printf("Last updated: %s", token);  
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
  strncat(info,anEntry->email, sizeof(info)-1);
  strncat(info, ",", sizeof(info)-1);
  strncat(info,anEntry->username, sizeof(info)-1);
  strncat(info, ",", sizeof(info)-1);
  strncat(info,anEntry->password, sizeof(info)-1);
  strncat(info, ",", sizeof(info)-1);
  strncat(info,anEntry->lastUpdated, sizeof(info)-1);
  
  write(fd, info, sizeof(info));
  close(fd);
}

void see (char * user) {
  char text [100];
  printf("\nWhat entry would you like to view the contents of?\nType \"cancel\" to go back\n");
  fgets(text, sizeof(text), stdin);
  remover(text);

  if (!strcmp(text, "cancel")) {
    return;
  }
  else {
    printEntry(text, user);
  }
}

void add (char * userAcct) {
  char text [100], type [100], usr [100], pw [100], mail [100], prompt [10], loc [100];
  int con = 1;
  struct acct *acc_entry;

  printf ("\nAdding account...\nType \"cancel\" at anytime to go back\n");

  while (con) {
    while (1) {
      printf("\nWhat account would you like to add?\n");
      fgets(type, sizeof(type), stdin);
      remover(type);

      if (!strcmp(type, "cancel")) {
        break;
      }

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

    if (!strcmp(type, "cancel")) {
      break;
    }

    printf("\nWhat is your email associated with said account? Enter \"0\" if N/A\n");
    fgets(mail, sizeof(mail), stdin);
    remover(mail);

    if (!strcmp(mail, "cancel")) {
      break;
    } 
         
          
    printf("\nWhat is the username associated with said account? Enter \"0\" if N/A\n");
    fgets(usr,sizeof(usr),stdin);
    remover(usr);

    if (!strcmp(usr, "cancel")) {
      break;
    }      
      
    printf("\nWhat is the password associated with said account? Enter \"0\" if N/A\n");
    fgets(pw, sizeof(pw), stdin);
    remover(pw);

    if (!strcmp(pw, "cancel")) {
      break;
    } 
            
    acc_entry = new_entry(type,usr,pw,mail);
    printf("\nAccount type: %s\nEmail: %s\nUsername: %s\nPassword: %s\n", acc_entry->acctType, acc_entry->email, acc_entry->username, acc_entry->password);
    printf("\nIs this correct? \"yes\" or \"no\"\n");
          
    while(1) {
      fgets(prompt,sizeof(prompt),stdin);
      remover(prompt);
      if (!strcmp(prompt,"yes")) {
        con = 0;
        break;
      }
      else if (!strcmp(prompt,"no")) {
        printf ("Please correctly enter the information\n");
        break;
      }
      else if (!strcmp(prompt, "cancel")) {
        con = 0;
        break;
      }
      else printf("Is this correct? \"yes\" or \"no\"\n");
    }
  }

  if (!strcmp(type, "cancel") || !strcmp(usr, "cancel") || !strcmp(pw, "cancel") || !strcmp(mail, "cancel") || !strcmp(prompt, "cancel")) {
    printf ("\nCancelling entry...\n");
    return;
  }
  else {
    convert(acc_entry,userAcct);    
    printf("\nEntry added\n");
  }
  free(acc_entry);
}

//account_type-what type of account it is
//updates one part of an entry (email,password,user,etc) - entry_type
//takes argument for string with data

//updates one part of an account's entry
void update_start (char * user) {
  char update_acc [100];
  char update_entry [100];
  char update_data [100];
  char prompt [10];
  printf ("\nUpdating entry...\nType \"cancel\" at anytime to go back\n");
  
  int con = 1;
  int con2 = 1;
  while (con) {
    while (1) {
      printf ("\nWhat account would you like to update?\n");
      fgets (update_acc, sizeof(update_acc), stdin);
      remover (update_acc);

      if (!strcmp(update_acc, "cancel")) {
        break;
      } 

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

    if (!strcmp (update_acc, "cancel")) {
      break;
    }

    printEntry (update_acc, user);
    while (con2){
    while (1) {
      printf ("\nWhat entry would you like to update: \"email\", \"username\", or \"password\" \n"); 
      fgets (update_entry, sizeof(update_entry), stdin);
      remover (update_entry);

      if (!strcmp(update_entry, "cancel")) {
        break;
      } 
      else if (!strcmp(update_entry, "username") || !strcmp(update_entry, "password") || !strcmp(update_entry, "email")) {
          break;
      }
      else {
        printf("\nPlease enter a valid entry\n");
        memset (update_acc, 0, sizeof(update_entry));
      }
    }

    if (!strcmp(update_entry, "cancel")) {
      break;
    } 

    printf ("\nWhat would you like to update it to?\n");
    fgets (update_data, sizeof(update_data), stdin);
    remover (update_data);

    if (!strcmp(update_data, "cancel")) {
      break;
    }
    
    struct acct * temp_entry = malloc(sizeof(struct acct));
    char * username;
    char * password;
    char * email;

    char loc [100];
    strncpy(loc, "data/", sizeof(loc)-1);
    strncat(loc, user, sizeof(loc)-1);
    strncat(loc, "/", sizeof(loc)-1);
    strncat(loc, update_acc, sizeof(loc)-1);
    remover(loc);
    
    char line [100];
    char * y = line;
    char * token, * p;
    FILE * fp;
    fp = fopen(loc, "r");
    fgets (y, 100, fp);
    p = y;
    fclose(fp);

    if (!strcmp(update_entry, "username")) {
      strsep (&p, ",");
      email = strsep(&p,",");
      username = update_data;
      strsep (&p,",");
      password = strsep (&p, ",");
    }
    else if (!strcmp(update_entry, "password")) {
      strsep (&p, ",");
      email = strsep(&p, ",");
      username = strsep (&p, ",");
      password = update_data;
      strsep (&p, ","); 
    }
    else if (!strcmp(update_entry, "email")) {
      strsep (&p, ",");
      email = update_data;
      strsep (&p, ",");
      username = strsep (&p, ",");
      password = strsep (&p, ",");
    }

    temp_entry = new_entry (update_acc, username, password, email);
    printf("\nAccount type: %s\nEmail: %s\nUsername: %s\nPassword: %s\n", 
    temp_entry->acctType,
    temp_entry->email,temp_entry->username, temp_entry->password);
    printf("\nIs this correct? \"yes\" or \"no\"\n");


    while (1) {
      fgets (prompt, sizeof(prompt), stdin);
      remover(prompt);

      if(!strcmp(prompt, "yes")) {
        convert (temp_entry, user);
        printf ("Update successful\n");
        con = 0;
        break;
      }
      else if (!strcmp(prompt, "no")) {
        printf ("\nPlease re-enter the information correctly\n");
        break;
      }
      else if (!strcmp(prompt, "cancel")) {
        printf ("\nCancelling update...\n");
        con = 0;
        break;
      }
      else printf("Is this correct? \"yes\" or \"no\"\n");
    }
  
  if (!strcmp (update_acc, "cancel") || !strcmp(update_entry, "cancel") || !strcmp(update_data, "cancel")) {
    printf ("\nCancelling update...\n");
    return;
  }
  while (1){
    printf("\nAre you finished updating this account? \"yes\" or \"no\"\n");
    fgets(prompt, sizeof(prompt), stdin);
    remover(prompt);
    trim(prompt);
    if (!strcmp(prompt, "yes")) {
      con2 = 0;
      con = 0;
      break;
    }
    else if (!strcmp(prompt, "no") || !strcmp(prompt, "cancel")){
      break;
    }
  }
  free(temp_entry);
  }
  }
}

void list(char *user){
  int found = 0;
  char loc[100];
  DIR *d;
  struct dirent *entry;
  
  printf("\n");
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
      if (!found) printf("Listing current entries...\n");
      found = 1;
      printf("%s\n",entry->d_name);
    }
    entry=readdir(d);
  }
  if (!found){
    printf("There are no entries\n");
  }
  closedir(d);

}

void remove_entry (char * user){
  char loc[100], entry[100];
  int con = 1;

  printf ("\nRemoving entry...\nType \"cancel\" at anytime to go back\n");
  while (con) {
    strncpy(loc, "data/", sizeof(loc)-1);
    strncat(loc, user, sizeof(loc)-1);
    strncat(loc, "/", sizeof(loc)-1);
    printf("\nWhich entry would you like to remove\n");
    fgets(entry, sizeof(entry), stdin);
    remover(entry);

    if (!strcmp (entry, "cancel")) {
      break;
    }

    strncat(loc, entry, sizeof(loc)-1);
    int fd = open(loc, O_RDONLY);
    if (fd < 0) {
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
          printf ("\nPlease re-enter the information correctly\n");
          close(fd);
          break;
        }
        else if (!strcmp(prompt, "cancel")) {
          printf ("\nCancelling removal...\n");
          close (fd);
          con = 0;
          break;
        }
        else printf ("\nAre you sure you want to remove this entry, \"yes\" or \"no\"?\n");
      }
    }
  }
  if (!strcmp (entry, "cancel")) {
    printf ("\nCancelling removal...\n");
    return;
  }
}
