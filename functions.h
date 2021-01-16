#ifndef FUNCTIONS_H
#define FUNCTIONS_H


struct acct {
  char username[100];
  char password[100];
  char email[100];
  char acctType[100];
  char lastUpdated[100];
};

struct acct *new_entry(char * type, char* usr, char *pw, char* mail);

void printEntry(char *path, char* user);
void convert(struct acct *anEntry, char * user);
void update(char* newdata, char* entryType, char* accountType, char* user);
void add(char* userAcct);
void see(char* user);

#endif