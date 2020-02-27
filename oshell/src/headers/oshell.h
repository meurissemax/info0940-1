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

#include "vector.h"


/*******************/
/* Constant values */
/*******************/

// Can't modify the 2 following constants
static const int MAX_CMD_SIZE = 256;
static const int MAX_ARGS = 256;


/**************/
/* Prototypes */
/**************/

/**
 * Parse a command line into arguments.
 *
 * @param 	line 		represents the line as a single string (unparsed)
 * @param 	arguments 	represents an array of string which contains the command ([0])
 * 						and its arguments ([1], [2], ..., [255]).
 */
void parseCmdLine(char* line, char** arguments);

/**
 * Read a character from the user input and discard up to the newline.
 *
 * @return 	a single character of user input
 */
char readCharInput(void);

/**
 * Execute the built-in 'exit' command.
 *
 * @param 	arguments 	array of string which contains the command and its arguments
 * @param 	v 			a pointer to a vector
 */
void cmd_exit(char** arguments, vector* v);

/**
 * Execute the built-in 'cd' command.
 *
 * @param 	arguments 	array of string which contains the command and its arguments
 */
void cmd_cd(char** arguments);

/**
 * Execute the built-in 'showlist' command.
 *
 * @param 	v 			a pointer to a vector
 */
void cmd_showlist(vector* v);

/**
 * Execute the built-in 'memdump' command.
 *
 * @param 	v 			a pointer to a vector
 * @return 	a pointer to a vector
 */
vector* cmd_memdump(vector* v);

/**
 * Execute the built-in 'loadmem' command.
 *
 * @param 	v 			a pointer to a vector
 *
 * @return 	a pointer to a vector
 */
vector* cmd_loadmem(vector* v);

/**
 * Execute a command (with its arguments) 'number' times
 * sequentially and save the history of the execution
 * in vector 'v'.
 *
 * @param 	arguments 	array of string which contains the command and its arguments
 * @param 	v 			a pointer to a vector
 * @param 	number 		the number of times to run the command
 */
void exec_sequential(char** arguments, vector* v, int number);

/**
 * Execute a command (with its arguments) 'number' times
 * in a parallel way and save the history of the execution
 * in vector 'v'.
 *
 * @param 	arguments 	array of string which contains the command and its arguments
 * @param 	v 			a pointer to a vector
 * @param 	number 		the number of times to run the command
 */
void exec_parallel(char** arguments, vector* v, int number);

#endif
