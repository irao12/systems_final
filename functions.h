#ifndef FUNCTIONS_H
#define FUNCTIONS_H


struct acct {
  char username [100];
  char password [100];
  char email [100];
  char acctType [100];
  char lastUpdated [100];
};

struct acct *new_entry (char * type, char* usr, char *pw, char* mail);

void printEntry (char *path, char* user);
void convert (struct acct *anEntry, char * user);

void see (char* user);
void add (char* userAcct);
void update_start(char * user);
void list(char *user);
void remove_entry(char *user);

#endif