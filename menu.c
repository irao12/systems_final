#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

void setup(){
  DIR* dir = opendir("data");
  if (!dir){
    mkdir("data", 0777);
  }
  dir = opendir("data/accounts");
  if (!dir){
    mkdir("data/accounts", 0777);
  }
}

void remover(char* buffer)
{
  char *returnval;
  returnval = strchr(buffer,'\n');
  if (returnval != NULL){
    *returnval='\0';
  }
}

void create_acc(){
  char usr[50];
  char pass[31];
  int i;
  int con = 1;
  while (con){
    printf("\nEnter a username for this account\n");
    fgets(usr, sizeof(usr), stdin);
    remover(usr);
    char location[100];
    strcpy(location, "data/accounts/");
    strcat(location, usr);
    int file = open(location, O_RDWR);
    if (file>0) {
      printf("An account already exists with this name\n");
    }
    else {
      file = open(location, O_RDWR | O_CREAT, 0666);
      printf("\nEnter a password for this account (max 30 characters)\n");
      fgets(pass,sizeof(pass), stdin);
      write(file, pass, sizeof(pass));
      char dat_loc[100];
      strcpy(dat_loc, "data/");
      strcat(dat_loc, usr);
      int test = mkdir(dat_loc, 0777);
      if (test == -1){
        printf("error: %s", strerror(errno));
      }
      
      printf("Account has been created\n\n");

      con = 0;
    }
  }
}

char * login(int * entry){
  char * usr_entered = malloc(sizeof(char)*51);
  char pass[31], pass_entered[31];
  char location[100];
  int con = 1;
  FILE* fp;
  while (con){
    printf("\nEnter your username\n");
    fgets(usr_entered, sizeof(usr_entered), stdin);
    remover(usr_entered);
    strcpy(location, "data/accounts/");
    strcat(location, usr_entered);
    fp = fopen(location, "r");
    if (!fp){
      printf("Account with that name does not exist\n");
    }
    else{
      printf("\nEnter your password\n");
      fgets(pass_entered, sizeof(pass_entered), stdin);
      remover(pass_entered);
      fgets(pass, sizeof(pass), fp);
      remover(pass);
      if (!strcmp(pass, pass_entered)){
        printf("\nLogin successful!\n");
        con = 0;
        *entry = 1;
      }
      else{
        printf("\nIncorrect password\n");
      }
    }
  }
  return usr_entered;
}
