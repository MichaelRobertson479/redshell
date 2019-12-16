char * rmfs( char * line);
char * rmbs( char * line);
char ** parse_args( char * line );
char ** parse_many( char * line );
char ** parse_redir( char * line );
char ** parse_pipe( char * line );
void redirect (char * line, int x);
void mario (char * name);
int run (char * name);
