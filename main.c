#include "menu.h"
#include "functions.h"
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

static void sigmain (int signo) {
  if (signo == SIGINT) {
    printf ("\n\nExiting password manager...\n");
    exit(0);
  }
}

static void sigparent (int signo) {
	if (signo == SIGINT) {
     if (entry) printf ("\n\nLogging out...\n"); 
	}
}

static void sigchild(int signo){
  if (signo == SIGINT) {
    exit(0);
  }
}

int main (void) {
 
  umask(0000);
  setup();
  printf ("Welcome to the Password Manager, use ctrl+c to exit the password manager\n");
  
  while (1) {
    signal(SIGINT, sigmain);
    char line [100];
    int con = 1;
    char user[100];
      while (con) {
      printf("\nEnter \"login\" to log in or \"create\" to create a new account for the password manager\n");
  
      fgets(line, sizeof(line), stdin);
      remover(line);

      if (!strcmp(line,"create")){
        create_acc();
      } 
      else if (!strcmp(line, "login")) {
        login(&entry, user);
        if (!strcmp (user, "canceled")) {
          con = 1;
        }
        else {
          con = 0;
        }
      }
      else {
        printf("Try again\n");
      }
    }

    int status;
    int child;
    char prompt [10];

    child = fork();
    if (!child) {
      signal (SIGINT, sigchild);
      if (entry)
      printf ("\nLogged into account: %s\nUse ctrl+c to go back to the main menu\n", user);
      else break;

      while (entry) {
        printf ("\nWould you like to \"list\", \"see\", \"add\", \"update\" or \"remove\"\n");
        fgets (prompt, sizeof(prompt), stdin);
        remover(prompt);

        if (!strcmp(prompt, "add")) {
          add(user);
        }
        else if (!strcmp(prompt, "see")) {
          see(user);
        }
        else if (!strcmp(prompt, "update")) {
          update_start (user);
        }
        else if (!strcmp(prompt, "list")) {
          list(user);
        }
        else if(!strcmp(prompt, "remove")) {
          remove_entry(user);
        }
      } 
    }
    else {
      signal (SIGINT, sigparent);
      int childpid = wait(&status);
    }
  }

  return 0;
}
