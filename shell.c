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

//remove front spaces
char * rmfs( char * line) {
  while (*line == ' ') {
    line++;
  }
  return line;
}

//remove back spaces
char * rmbs( char * line) {
  char * current;
  while (*line != '\0') {
    line++;
  }
  line--;
  while (*line == ' ') {
    *line = '\0';
    line--;
  }
  return current;
}

char ** parse_args( char * line ){
  char ** args = malloc(sizeof(char*) * 10);
  char * current = line;
  int i = 0;
  while (current != NULL) {
    args[i] = strsep(&current, " ");
    i++;
  }
  args[i] = NULL;
  return args;
}

char ** parse_many( char * line ){
  char ** args = malloc(sizeof(char*) * 10);
  char * current = line;
  int i = 0;
  while (current != NULL) {
    args[i] = rmbs(rmfs(strsep(&current, ";")));
    i++;
  }
  args[i] = NULL;
  return args;
}

char ** parse_redir( char * line ){
  char ** args = malloc(sizeof(char*) * 10);
  char * current = line;
  int i = 0;
  while (current != NULL) {
    args[i] = rmbs(rmfs(strsep(&current, "<>")));
    i++;
  }
  args[i] = NULL;
  return args;
}

//add error messages (maybe)
int redirect (char * line, int x) {

	

	// > redirect
	if (x > 0) {
		char ** inputs = parse_redir(line);

	  	int file = open(inputs[1],O_WRONLY);

   		char ** args = parse_args(inputs[0]);
    		if(fork() == 0) {
			
			dup2(file,STDOUT_FILENO);
       			execvp(args[0], args);	
    		}
		close(file);
	}
	
	//else if(x < 0)
	return 0;
}

int main(int argc, char * argv[]){

  int running = 1;
  while (running){
    char name[500];
    if (argc > 1) {
        strcpy(name, argv[1]);
    }else{
        fgets(name, 500, stdin);
        int i = 0;
        while (name[i] != '\n') {
            i++;
        }
        name[i] = '\0';
    }

    //printf("%p\n", strstr(name,";"));

    //add in redirect check
    if(strstr(name, ";") != NULL) {
      char ** commands = parse_many(name);
      int i = 0;
      while (commands[i] != NULL){
        char ** args = parse_args(commands[i]);
        if(fork() == 0) {
            execvp(args[0], args);
        }
        else {
          wait(NULL);
        }
        i++;
      }
    }else{

        if (strstr(name,">") != NULL) {
          //printf("detects >\n");
          redirect(name,1);
        }

        else {

          char ** args = parse_args(name);

          //make it work for ' exit'
          //https://stackoverflow.com/questions/1488372/mimic-pythons-strip-function-in-c
          if (strcmp(args[0], "exit") == 0){
            running = 0;
          }

          //make it work for 'cd' with no secondary args
          if (strcmp(args[0],"cd") == 0) {
              chdir(args[1]);
          }



          else if (fork() == 0){
            execvp(args[0], args);
          }

      // int status;
      // wait(&status);
      // exit(0);
      }
    }
  }
  return 0;
}
