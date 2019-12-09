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

int main () {

	char ** args = {"ls", ">", "README.md",NULL};

	execvp(args[0],args);	
	
	return 0;
}
