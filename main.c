#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

int main(void) {
while (1){
  char line[100];
  printf("Welcome to the Password Manager\n");
  char test = 48;
  int con = 1;
  int entry = 0;
  while (con){
    printf("Enter \"login\" to log in or \"create\" to create a new account\nfor the password manager\n");
    fgets(line, sizeof(line), stdin);
    remover(line);
    if (!strcmp(line,"create")){
      create_acc();
      con = 0;
    } 
    /*else if (!strcmp(line, "login")){
      login();
      con = 0;
    }
    */
    else {
      printf("Try again\n");
    }
  }
  while (entry){

  }
}
  return 0;
}