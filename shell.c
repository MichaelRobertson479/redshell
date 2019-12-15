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

char ** parse_pipe( char * line ){
  char ** args = malloc(sizeof(char*) * 10);
  char * current = line;
  int i = 0;
  while (current != NULL) {
    args[i] = rmbs(rmfs(strsep(&current, "|")));
    i++;
  }
  args[i] = NULL;
  return args;
}

//add error messages (maybe)
void redirect (char * line, int x) {

// double redirect
if (x == 0) {
if(fork() == 0) {
char ** inputs = parse_redir(line);

int fileRead = open(inputs[1],O_RDONLY);
int fileWrite = open(inputs[2],O_TRUNC|O_WRONLY);

    char ** args = parse_args(inputs[0]);

dup2(fileRead,STDIN_FILENO);
dup2(fileWrite,STDOUT_FILENO);

        execvp(args[0], args);

close(fileRead);
close(fileWrite);
}
}

// > redirect
if (x > 0) {
    if(fork() == 0) {

char ** inputs = parse_redir(line);

  int file = open(inputs[1],O_TRUNC|O_WRONLY);

    char ** args = parse_args(inputs[0]);
dup2(file,STDOUT_FILENO);
        execvp(args[0], args);
close(file);
    }
}

  // < redirect
  if (x < 0) {
    //printf("thisisatest\n");
     if(fork() == 0) {

char ** inputs = parse_redir(line);

  int file = open(inputs[1],O_RDONLY);

    char ** args = parse_args(inputs[0]);
dup2(file,STDIN_FILENO);
        execvp(args[0], args);
close(file);
      }
  }

}

// void mario (char * name) {

  
//   char ** inputs = parse_pipe(name);
  
//   int desc[2];
//   pipe(desc);

//   if (fork() == 0) {
//     dup2(desc[1],STDOUT_FILENO);
//     char ** args0 = parse_args(inputs[0]);
//     execvp(args0[0],args0);
//   }


//   if (fork() == 0) {
//     dup2(desc[0],STDIN_FILENO);
//     char ** args1 = parse_args(inputs[1]);
//     close(desc[1]);
//     execvp(args1[0],args1);
//   }

//   close(desc[1]);
//   close(desc[0]);
// }

//currently doesn't work if spaces
void mario (char * name) {
  char ** commands = parse_pipe(name);
  printf("command 1: %s\n",commands[1]);
  char ** args = malloc(sizeof(char*) * 2);
  args[0] = commands[1];
  args[1] = NULL;

  FILE * p;

  if (fork() == 0) {
    dup2(fileno(p),STDIN_FILENO);
    p = popen(commands[0],"r");
    printf("gets here 6\n");
    int i;
    wait(&i);
    execvp(args[0],args);
    
    printf("gets here 7\n");
  }

}

int run (char * name) {

        if (strstr(name,"|") != NULL) {
          printf("detects pipe\n");
          mario(name);
        }

        else if (strstr(name,">") != NULL && strstr(name,"<") != NULL) {
          //printf("detects double arrow\n");
          redirect(name,0);
        }


        else if (strstr(name,">") != NULL) {
          //printf("detects >\n");
          redirect(name,1);
        }

        else if (strstr(name,"<") != NULL) {
          //printf("detects <\n");
          redirect(name,-1);
        }

        else {

          char ** args = parse_args(rmfs(name));

          if (strcmp(args[0], "exit") == 0){
            return 0;
          }

          //make it work for 'cd' with no secondary args
          else if (strcmp(args[0],"cd") == 0) {

   if (args[1] != NULL) {
       chdir(args[1]);
   }

   else {
printf("to be implemented later\n");
            }
          }

          else if (fork() == 0){
            execvp(args[0], args);
          }
 else {
   wait(NULL);
 }
}

return 1;
}

int main(int argc, char * argv[]){

  int running = 1;

  while (running) {

    char name[500];

    if (argc > 1) {
        strcpy(name, argv[1]);
    }

    else {
        fgets(name, 500, stdin);
        int i = 0;
        while (name[i] != '\n') {
            i++;
        }
        name[i] = '\0';
    }

    //add in redirect check
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