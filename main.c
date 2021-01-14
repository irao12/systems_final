#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>

int entry = 0;

int main(void) {
umask(0000);
setup();
while (1){
  char line[100];
  printf("Welcome to the Password Manager\n");
  int con = 1;
  char *user;
  while (con){
    printf("Enter \"login\" to log in or \"create\" to create a new account\nfor the password manager\n");
    fgets(line, sizeof(line), stdin);
    remover(line);

    if (!strcmp(line,"create")){
      create_acc();
      con = 0;
    } 
    else if (!strcmp(line, "login")){
      user = login(&entry);
      con = 0;
    }
    else {
      printf("Try again\n");
    }

  }

  int status;
  int child;
  child = fork();

  if (!child){
    while (entry){
      char prompt[10];
      printf("\nWould you like to \"see\", \"add\", or \"update\"\n");
      fgets(prompt, sizeof(prompt), stdin);
    }
    return 0;
  }

  else {
    int childpid = wait(&status);
  }

}
  return 0;
}
