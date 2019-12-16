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

char home[50];

void gethome() {
  getcwd(home,50);
}

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
  //char * args[10];
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
      int fileWrite = open(inputs[2],O_CREAT|O_WRONLY|O_TRUNC,0777);

      char ** args = parse_args(inputs[0]);

      dup2(fileRead,STDIN_FILENO);
      dup2(fileWrite,STDOUT_FILENO);

      execvp(args[0], args);

      close(fileRead);
      close(fileWrite);

      free(inputs);
      free(args);
    }
  }

  // > redirect
  if (x > 0) {
    if(fork() == 0) {
      char ** inputs = parse_redir(line);

      int file = open(inputs[1],O_CREAT|O_WRONLY|O_TRUNC,0777);

      char ** args = parse_args(inputs[0]);
      dup2(file,STDOUT_FILENO);
      execvp(args[0], args);
      close(file);

      free(args);
      free(inputs);
    }
  }

  // < redirect
  if (x < 0) {
     if(fork() == 0) {
      char ** inputs = parse_redir(line);
      int file = open(inputs[1],O_RDONLY);

      char ** args = parse_args(inputs[0]);
      dup2(file,STDIN_FILENO);
      execvp(args[0], args);
      close(file);

      free(args);
      free(inputs);
    }
  }
}

void mario (char * name) {
  char ** commands = parse_pipe(name);
  char ** args = parse_args(commands[1]);

  if (fork() == 0) {
    FILE * p = popen(commands[0],"r");
    dup2(fileno(p),STDIN_FILENO);
    execvp(args[0],args);
    pclose(p);
  }

  free(commands);
  free(args);
}

int run (char * name) {

  if (strstr(name,"|") != NULL) {
    mario(name);
  }

  else if (strstr(name,">") != NULL && strstr(name,"<") != NULL) {
    redirect(name,0);
  }

  else if (strstr(name,">") != NULL) {
    redirect(name,1);
  }

  else if (strstr(name,"<") != NULL) {
    redirect(name,-1);
  }

  else {
    char ** args = parse_args(rmfs(name));

    if (strcmp(args[0], "exit") == 0){
      return 0;
    }

    else if (strcmp(args[0],"cd") == 0) {
      if (args[1] != NULL) {
        chdir(args[1]);
      }

      else {
        chdir(home);
      }
    }

    else if (fork() == 0){
      execvp(args[0], args);
    }

    else {
      wait(NULL);
    }

    free(args);
  }
  return 1;
}