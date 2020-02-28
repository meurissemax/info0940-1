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

#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "headers/oshell.h"


/*************/
/* Variables */
/*************/

int sigalrm_flag = 0;


/*************/
/* Functions */
/*************/

void parseCmdLine(char* line, char** arguments) {
	int i = 0;

	line[strlen(line) - 1] = '\0';
	arguments[i] = strtok(line, " ");

	while(arguments[i++] && i < MAX_ARGS)
        arguments[i] = strtok(NULL, " ");
}

char readCharInput(void) {
	char c = getchar();

	while(getchar() != '\n');

	return c;
}

void cmd_exit(char** arguments, vector* cmd_list) {
    if(arguments[1] != NULL)
        fprintf(stderr, "Arguments passed after the exit command have been discarded\n");

    vector_free(cmd_list);

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
            fprintf(stderr, "Unknown directory, or unable to open it\n");
        }
    }

    // Go in a subdirectory
    else {
        int v = chdir(arguments[1]);

        if(v != 0) {
            fprintf(stderr, "Unknown subdirectory, or unable to open it\n");
        }
    }
}

void cmd_showlist(vector* cmd_list) {
    vector_print(cmd_list);
}

vector* cmd_memdump(vector* cmd_list) {
    return vector_export(cmd_list);
}

vector* cmd_loadmem(vector* cmd_list) {
    return vector_import(cmd_list);
}

void exec_once(char** arguments, vector* cmd_list) {
    exec_sequential(arguments, cmd_list, 1);
}

static void sigalrm_handler(int sig) {
    fprintf(stderr, "Process timeout\n");

    sigalrm_flag = sig;
}

void exec_sequential(char** arguments, vector* cmd_list, int number) {
    for(int i = 0; i < number; i++) {
        pid_t pid = fork();

        // If work has failed
        if(pid < 0) {
            perror("Fork error");
        }

        // If we are in the child process
        else if (pid == 0) {
            // Execute the command
            if(execvp(arguments[0], arguments) < 0) {
                perror("Execution error");
                exit(1);
            }
        } else {
            // Set the signal and the alarm for the timeout
            signal(SIGALRM, sigalrm_handler);
            alarm(TIME_LIMIT);

            // Status of the child process
            int status;

            // We wait for child or timeout
            while((wait(&status)) != -1 && sigalrm_flag == 0);

            // We add the command to the command list (timeout or not)
            vector_add(cmd_list, arguments[0], pid, WEXITSTATUS(status));

            // If a timeout occurs, we kill the child process
            if(sigalrm_flag == 1) {
                kill(pid, 9);
            }

            // We reset the timeout
            alarm(0);
            sigalrm_flag = 0;
        }
    }
}

void exec_parallel(char** arguments, vector* cmd_list, int number) {
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
                exit(1);
            }
        }
    }

    // Parent process waits for child processes
    while(number > 0) {
        // Status of the child process
        int status;

        // We wait for child
        pid_t pid = waitpid(-1, &status, 0);

        // We add the command to the command list
        vector_add(cmd_list, arguments[0], pid, WEXITSTATUS(status));

        number--;
    }
}
