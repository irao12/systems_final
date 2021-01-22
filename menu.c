#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

char location[100];

static void sigcreate (int signo) {
	if (signo == SIGINT) {
    printf ("\n\nExiting password manager...\n");
    remove(location);
    exit(0);
	}
}

static void siglogin (int signo) {
	if (signo == SIGINT) {
    printf ("\n\nExiting password manager...\n");
    exit(0);
	}
}

void setup() {
  DIR* dir = opendir("data");
  if (!dir){
    mkdir("data", 0777);
  }
  dir = opendir("data/accounts");
  if (!dir){
    mkdir("data/accounts", 0777);
  }
}

void trim (char * str) {
  int index, i;
  index = -1;
  
  i = 0;
  while(str[i]) {
    if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
      index= i;
    }
    i++;
  }

  str[index + 1] = '\0';
}

void remover(char * buffer) {
  char * returnval;
  returnval = strchr (buffer,'\n');
  if (returnval != NULL){
    *returnval = '\0';
  }
  trim (buffer);
}

void create_acc() {
  signal (SIGINT, sigcreate);

  printf ("\nCreating account...\nType \"cancel\" at anytime to go back to main menu\n");
  char usr[50];
  char pass[31], re_pass[31];
  int i;
  int con = 1;
  while (con) {
    printf ("\nEnter a username for this account\n");
    fgets (usr, sizeof(usr), stdin);
    remover (usr);

    if (!strcmp(usr, "cancel")) {
      break;
    }

    strcpy (location, "data/accounts/");
    strcat (location, usr);
    int file = open (location, O_RDWR);
    if (file > 0) {
      printf("An account already exists with this name\n");
    }
    else {
      file = open (location, O_RDWR | O_CREAT, 0666);
      while (1) {
        printf ("\nEnter a password for this account (max 30 characters)\n");
        fgets (pass, sizeof(pass), stdin);
        remover(pass);

        if (!strcmp(pass, "cancel")) {
          break;
        }

        printf ("\nRe-enter to confirm your password\n");
        fgets (re_pass,sizeof(pass), stdin);
        remover (re_pass);
        
        if (!strcmp(re_pass, "cancel")) {
          break;
        }

        if (!strcmp(pass, re_pass)) {
          break;
        }
        else printf ("\nPasswords do not match");
      }

      if (!strcmp(pass, "cancel") || !strcmp(re_pass, "cancel")) {
        remove(location);
        break;
      }

      write (file, pass, sizeof(pass));
      char dat_loc [100];
      strcpy (dat_loc, "data/");
      strcat (dat_loc, usr);
      int test = mkdir (dat_loc, 0777);
      if (test == -1) {
        printf("error: %s", strerror(errno));
      }
      
      printf("\nAccount has been created!\n");

      con = 0;
    }
  }
}

void login(int * entry, char * user) {
  signal (SIGINT, siglogin);

  printf ("\nLogging in...\nType \"cancel\" at anytime to go back to main menu\n");
  char usr_entered[100];
  char pass[31], pass_entered[31];
  char location2[200];
  int con = 1;
  FILE* fp;
  while (con) {
    printf("\nEnter your username\n");
    fgets(usr_entered, sizeof(usr_entered), stdin);
    remover(usr_entered);

    if (!strcmp(usr_entered, "cancel")) {
      con = 0;
      break;
    }

    strcpy(location2, "data/accounts/");
    strcat(location2, usr_entered);
    fp = fopen(location2, "r");

    if (!fp) {
      printf("%s\n", strerror(errno));
      printf("Account with that name does not exist\n");
    }
    else {
      printf("\nEnter your password\n");
      fgets(pass_entered, sizeof(pass_entered), stdin);
      remover(pass_entered);
      
      if (!strcmp(pass_entered, "cancel")) {
        con = 0;
        break;
      }

      fgets(pass, sizeof(pass), fp);
      remover(pass);
      if (!strcmp(pass, pass_entered)) {
        printf("\nLogin successful!\n");
        con = 0;
        *entry = 1;
      }
      else {
        printf("\nIncorrect password\n");
      }
    }
  }

  if (!strcmp(usr_entered, "cancel") || !strcmp(pass_entered, "cancel")) {
    char * backed_out = "canceled";
    *entry = 0;
    return;
  }
  strcpy(user, usr_entered);
  return;
}
