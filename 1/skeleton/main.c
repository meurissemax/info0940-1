/*
Group: XX
Member1: ID - LastName - FirstName
Member2: ID - LastName - FirstName
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "oshell.h"

/*
Important:
    1. DO NOT MODIFY the given skeleton. Nevertheless, if it is easier for you, 
you can (only) wrap existing code within conditions (if/else, switch).
    2. Since I'm the TA, I can use static allocation. But you are a student, so
you MUST use dynamic allocation for your data structures. It is an Operating 
Systems course after all...
*/

int main() {
    
    char line[MAX_CMD_SIZE]; 
    char* arguments[MAX_ARGS];
    
    int copies;
    int parallel = false;
    
    do{
        printf("OShell> ");
        fgets(line, sizeof(line), stdin);
        
        if (line[0] == '\n')
            continue;
        
        parseCmdLine(line, arguments);
        
        // Add some stuff here ...

        // Exit the shell
        if(strcmp(arguments[0], "exit") == 0){
            if(arguments[1] != NULL)
                fprintf(stderr, "Arguments passed after the exit command have been discarded.\n");
            exit(0); // Can we use exit ? execvp does not work.
        }

        // Is it supposed to print something ? We do not do anything related to the directory...
        else if(strcmp(arguments[0], "cd") == 0){
            // Go to home directory
            if(arguments[1] == NULL || strcmp(arguments[1], "~") == 0){
                chdir(getenv("HOME"));
            }
            // Go up a directory
            else if(strcmp(arguments[1], "..") == 0){
                chdir("..");
                char s[100];
                printf("%s\n", getcwd(s, 100)); 
            }
            // Go in a particular directory
            else if ((char) arguments[1][0] == '/'){
                int v = chdir(arguments[1]);
                if(v != 0)
                    fprintf(stderr, "Unknown directory, or unable to open it.\n"); // Maybe make it more explicit, differentiate the cases.
            }
            // Go in a subdirectory
            else{
                int v = chdir(arguments[1]);
                if(v != 0)
                    fprintf(stderr, "Unknown subdirectory, or unable to open it.\n"); // Maybe make it more explicit, differentiate the cases.
            }
            // Don't pass through multiple copies.
            continue;
        }


        
        // Number of times to execute a specific command
        do {
            printf("\tCopies> ");
            copies = readCharInput() - '0';
        } while (copies <= 0 || copies > 9);
        
        // For multiple executions
        if (copies > 1){
            printf("\t[S]equential (default) or [P]arallelize> ");
            parallel = (toupper(readCharInput()) == 'P') ? true : false;
        }
        
        // Add another stuff here ...

        // Show list command.
        if(strcmp(arguments[0], "showlist") == 0){

        }

        
    }while(true);
}
