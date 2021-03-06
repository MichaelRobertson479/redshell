char * rmfs( char * line);
//INPUT: Takes in a string which is usually an argument from a shell command
//OUTPUT: Returns the string with all front spaces removed

char * rmbs( char * line);
//INPUT: Takes in a string which is usually an argument from a shell command
//OUTPUT: Returns the string with all back spaces removed

char ** parse_args( char * line );
//INPUT: Takes in a line from the shell which is to be parsed
//OUTPUT: Splits the string on all spaces and returns an array of strings

char ** parse_many( char * line );
//INPUT: Takes in a line from the shell which is to be parsed
//OUTPUT: Splits the string on all semicolons and returns an array of strings
//USE: This parse method is used when the user types in multiple commmands to the shell in one line, separated by semicolons

char ** parse_redir( char * line );
//INPUT: Takes in a line from the shell which is to be parsed
//OUTPUT: Splits the string on either < or > and returns an array of strings
//USE: This parse method is used when the user is trying to use a command with redirection in the shell

char ** parse_pipe( char * line );
//INPUT: Takes in a line from the shell which is to be parsed
//OUTPUT: Splits the string on | and returns an array of strings
//USE: This parse method is used when the user is trying to use a command with pipes in the shell

void redirect (char * line, int x);
//INPUT: Takes in a line from the shell which is to be parsed and an integer that represents whether said string includes a > or a <. If the integer is 0, a double redirect occurs. If the integer is positive, a > redirect occurs, and if the integer is negative, a < redirect occurs
//OUTPUT: Void
//USE: By looking at the value of the integer, the method will run one of three separate algorithms: one to redirect the contents of a file as the input of a command, one to redirect the output of a command into a certain file, and one that will combine both previous cases

void mario (char * name);
//INPUT: Takes in a line from the shell which is to be parsed and includes a |, symbolizing that it is a command that will require the use of pipes
//OUTPUT: Void
//USE: Runs the first command and uses the output of said command as the input for the second command

void gethome();
//INPUT: None
//OUTPUT: Void, but it updates a global variable with the path of the current working directory
//USE: This method is used for the edge case where the user types in cd with no arguments. The behavior that occurs just changes the directory to the home directory, which we need to get using the getcwd() function, called in this method

int run (char * name);
//INPUT: Takes in the commands from the shell which are going to be run and parses through the string, running each command with the appropriate methods
//OUTPUT: Returns either a 0 or a 1, signifying whether or not "exit" was typed by the use (causing the shell to exit). If "exit" was typed, a 0 is returned, which will stop the forever loop in the main method. Otherwise, a 1 is returned and the loop continues running.