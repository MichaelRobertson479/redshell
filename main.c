#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include "shell.h"

int main(int argc, char * argv[]){

  gethome();
  int running = 1;

  while (running) {

    char name[500];

    fgets(name, 500, stdin);
    int i = 0;
    while (name[i] != '\n') {
      i++;
    }
    name[i] = '\0';
  
    if(strstr(name, ";") != NULL) {
      char ** commands = parse_many(name);

      int i = 0;
      while (commands[i] != NULL){
        running = run(commands[i]);
        i++;
      }
    }

    else{
      running = run(name);
    }
  }


  return 0;
}
