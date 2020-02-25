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
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "headers/oshell.h"
#include "headers/vector.h"


/********/
/* Main */
/********/

int main() {
    /*************/
    /* Variables */
    /*************/

    char line[MAX_CMD_SIZE]; 
    char* arguments[MAX_ARGS];

    int copies;
    int parallel = false;

    vector* v = vector_init();

    /*************/
    /* Main loop */
    /*************/

    do {
        /******************/
        /* Initialization */
        /******************/

        // We print the OShell
        printf("OShell> ");
        fgets(line, sizeof(line), stdin);

        if(line[0] == '\n')
            continue;

        // We parse the written command
        parseCmdLine(line, arguments);

        /*********************/
        /* Built-in commands */
        /*********************/

        /* 'exit' command */
        if(strcmp(arguments[0], "exit") == 0) {
            if(arguments[1] != NULL)
                perror("Arguments passed after the exit command have been discarded.\n");

            vector_free(v);

            exit(0);
        }

        /* 'cd' command */
        else if(strcmp(arguments[0], "cd") == 0) {
            cd_cmd(arguments);

            // Don't pass through multiple copies
            continue;
        }

        /* 'showlist' cmd */
        else if(strcmp(arguments[0], "showlist") == 0) {
            vector_print(v);

            // Don't pass through multiple copies
            continue;
        }

        /* 'memdump' cmd */
        else if(strcmp(arguments[0], "memdump") == 0) {
            memdump_cmd(arguments);

            // Don't pass through multiple copies
            continue;
        }

        /* 'loadmem' cmd */
        else if(strcmp(arguments[0], "loadmem") == 0) {
            loadmem_cmd(arguments);

            // Don't pass through multiple copies
            continue;
        }

        /*************************/
        /* Non built-in commands */
        /*************************/

        // Number of times to execute a specific command
        do {
            printf("\tCopies> ");

            copies = readCharInput() - '0';
        } while(copies <= 0 || copies > 9);

        /* Only once execution */
        if(copies == 1) {
            exec_once(arguments, v);
        }

        /* For multiple executions */
        else if(copies > 1) {
            printf("\t[S]equential (default) or [P]arallelize> ");

            parallel = (toupper(readCharInput()) == 'P') ? true : false;

            // Sequential executions
            if(!parallel) {
                for(int i = 0; i < copies; i++) {
                    exec_once(arguments, v);
                }
            }

            // Parallel executions
            else {
                exec_mult(arguments, copies);
            }
        }
    } while(true);
}
