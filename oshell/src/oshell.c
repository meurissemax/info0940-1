/**
 * INFO0940-1 - Operating systems
 * University of Liege
 * Academic year 2019-2020
 *
 * Group 1
 * 20161278 - MEURISSE Maxime
 * 20162864 - VERMEYLEN Valentin
 */

/***********/
/* Headers */
/***********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "headers/oshell.h"


/*************/
/* Functions */
/*************/

void parseCmdLine(char* line, char** arguments) {
	int i = 0;

	line[strlen(line) - 1] = '\0';
	arguments[i] = strtok(line, " ");

	while(arguments[i++] && i < MAX_ARGS) {
		arguments[i] = strtok(NULL, " ");
	}
}

char readCharInput(void) {
	char c = getchar();

	while(getchar() != '\n');

	return c;
}

void cmd_exit(char** arguments, vector* v) {
    if(arguments[1] != NULL)
        fprintf(stderr, "Arguments passed after the exit command have been discarded.\n");

    vector_free(v);

    exit(0);
}

void cmd_cd(char** arguments) {
	// Go to home directory
    if(arguments[1] == NULL || strcmp(arguments[1], "~") == 0) {
        chdir(getenv("HOME"));
    }

    // Go up a directory
    else if(strcmp(arguments[1], "..") == 0) {
        chdir("..");
    }

    // Go in a particular directory
    else if((char) arguments[1][0] == '/') {
        int v = chdir(arguments[1]);

        if(v != 0) {
            fprintf(stderr, "Unknown directory, or unable to open it.\n");
        }
    }

    // Go in a subdirectory
    else {
        int v = chdir(arguments[1]);

        if(v != 0) {
            fprintf(stderr, "Unknown subdirectory, or unable to open it.\n");
        }
    }
}

void cmd_showlist(vector* v) {
    vector_print(v);
}

vector* cmd_memdump(vector* v) {
    return vector_export(v);
}

vector* cmd_loadmem(vector* v) {
    return vector_import(v);
}

void exec_sequential(char** arguments, vector* v, int number) {
    for(int i = 0; i < number; i++) {
        pid_t pid = fork();

        // If work has failed
        if(pid < 0) {
            perror("Fork error");
        }

        // If we are in the parent process
        else if(pid > 0) {
            int status;

            // Wait for child
            wait(&status);

            // Add command to history
            vector_add(v, arguments[0], pid, status);
        }

        // If we are in the child process
        else {
            // Execute the command
            if(execvp(arguments[0], arguments) < 0) {
                perror("Execution error");
            }
        }
    }
}

void exec_parallel(char** arguments, vector* v, int number) {
    // Create the child processes
    for(int i = 0; i < number; i++) {
        pid_t pid = fork();

        // If work has failed
        if(pid < 0) {
            perror("Fork error");
        }

        // If we are in the child process
        else if(pid == 0) {
            // Execute the command
            if(execvp(arguments[0], arguments) < 0) {
                perror("Execution error");
            }
        }
    }

    // Parent process waits for child processes
    while(number > 0) {
        int status;

        // Wait for child
        pid_t pid = wait(&status);

        // Add command to history
        vector_add(v, arguments[0], pid, status);

        number--;
    }
}
