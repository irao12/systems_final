#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>


void remover(char* buffer)
{
  char *returnval;
  returnval = strchr(buffer,'\n');
  if (returnval != NULL){
    *returnval='\0';
  }
}

int pass_length(char* buffer){
  int i = 0;
  int count = 0;
  while (1){
    if (buffer[i]<48 || buffer[i]>122){
      break;
    }
    i++;
    count++;
  }
  return count;
}

char * pass_fix(char * pass, int len){
  int i;
  char * new_pass = malloc(sizeof(char)*(len+1));
  for (i = 0; i<len;i++){
    new_pass[i] = pass[i];
  }
  new_pass[len+1] = 0;
  return new_pass;
}

void create_acc(){
  char usr[100];
  char pass[31];
  int i;
  for (i = 0; i<31;i++){
    pass[i] = 0;
  }
  int con = 1;
  while (con){
    printf("Enter a username for this account\n");
    fgets(usr, sizeof(usr), stdin);
    remover(usr);
    char location[100];
    strcpy(location, "accounts/");
    strcat(location, usr);
    int file = open(location, O_RDWR);
    if (file>0) {
      printf("An account already exists with this name\n");
    }
    else {
      file = open(location, O_RDWR | O_CREAT, 0666);
      printf("Enter a password for this account (max 30 characters)\n");
      fgets(pass,sizeof(pass), stdin);
      int len = pass_length(pass);
      char * new_pass = pass_fix(pass, len);
      write(file, pass, sizeof(new_pass));
      printf("Account has been created\n\n");
      con = 0;
    }
  }
}

/*void login(){

}
*/