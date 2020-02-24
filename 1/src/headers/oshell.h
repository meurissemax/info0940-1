/**
 * INFO0940-1 - Operating systems
 * University of Liege
 * Academic year 2019-2020
 *
 * Group 1
 * 20161278 - MEURISSE Maxime
 * 20162864 - VERMEYLEN Valentin
 */

#ifndef oshell_h
#define oshell_h


/***********/
/* Headers */
/***********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*******************/
/* Constant values */
/*******************/

// Can't modify the 2 following constants
static const int MAX_CMD_SIZE = 256;
static const int MAX_ARGS = 256;


/**************/
/* Prototypes */
/**************/

// Can't modify the 2 following prototypes
char readCharInput(void);
void parseCmdLine(char* line, char** arguments);

// Built-in commands
void cd_cmd(char** arguments);
void showlist_cmd(char** arguments);
void memdump_cmd(char** arguments);
void loadmem_cmd(char** arguments);

// Non built-in functions
void exec_once(char** arguments);

#endif
