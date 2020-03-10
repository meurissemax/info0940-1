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
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "headers/oshell.h"
#include "headers/vector.h"


/**********************************/
/* Global variables and functions */
/**********************************/

// The command list
vector* cmd_list;

// Function to handle the SIGINT (CTRL + C)
void sigint_exit(int sig);

void sigint_exit(int sig) {
    // Useless, but to avoid a warning for "unused variable"
    sig++;

    // Free the command list
    if(cmd_list)
        vector_free(cmd_list);

    printf("\n");

    // Properly exit
    exit(0);
}


/********/
/* Main */
/********/

int main() {
    /* --------- */
    /* Variables */
    /* --------- */

    char line[MAX_CMD_SIZE]; 
    char* arguments[MAX_ARGS];

    int copies;
    int parallel = false;

    cmd_list = vector_init();

    /* ------- */
    /* Signals */
    /* ------- */

    // Set the signal to handle CTRL + C
    signal(SIGINT, sigint_exit);

    /* --------- */
    /* Main loop */
    /* --------- */

    do {
        /* -------------- */
        /* Initialization */
        /* -------------- */

        // We print the OShell
        printf("OShell> ");
        fgets(line, sizeof(line), stdin);

        if(line[0] == '\n')
            continue;

        // We parse the written command
        parseCmdLine(line, arguments);

        /* ----------------- */
        /* Built-in commands */
        /* ----------------- */

        /* 'exit' command */
        if(strcmp(arguments[0], "exit") == 0) {
            cmd_exit(arguments, cmd_list);
        }

        /* 'cd' command */
        else if(strcmp(arguments[0], "cd") == 0) {
            cmd_cd(arguments);

            // Don't pass through multiple copies
            continue;
        }

        /* 'showlist' command */
        else if(strcmp(arguments[0], "showlist") == 0) {
            cmd_showlist(cmd_list);

            // Don't pass through multiple copies
            continue;
        }

        /* 'memdump' command */
        else if(strcmp(arguments[0], "memdump") == 0) {
            cmd_memdump(cmd_list);

            // Don't pass through multiple copies
            continue;
        }

        /* 'loadmem' command */
        else if(strcmp(arguments[0], "loadmem") == 0) {
            cmd_loadmem(cmd_list);

            // Don't pass through multiple copies
            continue;
        }

        /* 'sys' command */
        else if(strcmp(arguments[0], "sys") == 0) {
            cmd_sys(arguments);

            // Don't pass through multiple copies
            continue;
        }

        /* --------------------- */
        /* Non built-in commands */
        /* --------------------- */

        // Number of times to execute a specific command
        do {
            printf("\tCopies> ");

            copies = readCharInput() - '0';
        } while(copies <= 0 || copies > 9);

        /* Only once execution */
        if(copies == 1) {
            exec_once(arguments, cmd_list);
        }

        /* Multiple executions */
        else if(copies > 1) {
            printf("\t[S]equential (default) or [P]arallelize> ");

            parallel = (toupper(readCharInput()) == 'P') ? true : false;

            // Sequential executions
            if(!parallel) {
                exec_sequential(arguments, cmd_list, copies);
            }

            // Parallel executions
            else {
                exec_parallel(arguments, cmd_list, copies);
            }
        }
    } while(true);

    // Theoretically, it will be never executed due to the
    // 'while(true)' statement, but we can never be too
    // careful...
    vector_free(cmd_list);

    return 0;
}
