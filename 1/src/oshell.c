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

#include "headers/oshell.h"


/*************/
/* Functions */
/*************/

/* -----------------------------------------------------------------------------
 * Parse a command line into arguments.
 * /!\ DO NOT MODIFY /!\
 *
 * PARAMETERS
 * line         represents the line as a single string (unparsed).
 * arguments    represents an array of string which contains the command ([0]) 
 *              and its arguments ([1], [2], ... [255]).
 *
 * RETURN
 * /
 * ---------------------------------------------------------------------------*/
void parseCmdLine(char* line, char** arguments) {
	int i = 0;

	line[strlen(line) - 1] = '\0';
	arguments[i] = strtok(line, " ");

	while(arguments[i++] && i < MAX_ARGS) {
		arguments[i] = strtok(NULL, " ");
	}
}

/* -----------------------------------------------------------------------------
 * Read a character from the user input and discard up to the newline.
 * /!\ DO NOT MODIFY /!\
 *
 * PARAMETERS
 * /
 *
 * RETURN
 * char     a single character of user input.
 * ---------------------------------------------------------------------------*/
char readCharInput(void) {
	char c = getchar();

	while(getchar() != '\n');

	return c;
}

/* -----------------------------------------------------------------------------
 * Implements the 'cd' command.
 *
 * PARAMETERS
 * arguments    represents an array of string which contains the command ([0]) 
 *              and its arguments ([1], [2], ... [255]).
 *
 * RETURN
 * /
 * ---------------------------------------------------------------------------*/
void cd_cmd(char** arguments) {
	// Go to home directory
    if(arguments[1] == NULL || strcmp(arguments[1], "~") == 0) {
        chdir(getenv("HOME"));
    }

    // Go up a directory
    else if(strcmp(arguments[1], "..") == 0) {
        chdir("..");

        char s[100];

        printf("%s\n", getcwd(s, 100)); 
    }

    // Go in a particular directory
    else if((char) arguments[1][0] == '/') {
        int v = chdir(arguments[1]);

        if(v != 0)
            fprintf(stderr, "Unknown directory, or unable to open it.\n");
    }

    // Go in a subdirectory
    else {
        int v = chdir(arguments[1]);

        if(v != 0)
            fprintf(stderr, "Unknown subdirectory, or unable to open it.\n");
    }
}

/* -----------------------------------------------------------------------------
 * Implements the 'showlist' command.
 *
 * PARAMETERS
 * arguments    represents an array of string which contains the command ([0]) 
 *              and its arguments ([1], [2], ... [255]).
 *
 * RETURN
 * /
 * ---------------------------------------------------------------------------*/
void showlist_cmd(char** arguments) {
    printf('showlist cmd - TO DO\n');
}

/* -----------------------------------------------------------------------------
 * Implements the 'memdump' command.
 *
 * PARAMETERS
 * arguments    represents an array of string which contains the command ([0]) 
 *              and its arguments ([1], [2], ... [255]).
 *
 * RETURN
 * /
 * ---------------------------------------------------------------------------*/
void memdump_cmd(char** arguments) {
    printf('memdump cmd - TO DO\n');
}

/* -----------------------------------------------------------------------------
 * Implements the 'loadmem' command.
 *
 * PARAMETERS
 * arguments    represents an array of string which contains the command ([0]) 
 *              and its arguments ([1], [2], ... [255]).
 *
 * RETURN
 * /
 * ---------------------------------------------------------------------------*/
void loadmem_cmd(char** arguments) {
    printf('loadmem cmd - TO DO\n');
}
