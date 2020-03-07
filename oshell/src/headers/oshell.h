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

// Time limit before a process timeout
#define TIME_LIMIT 5


/*******************/
/* Constant values */
/*******************/

// Can't modify the 2 following constants
static const int MAX_CMD_SIZE = 256;
static const int MAX_ARGS = 256;


/**************/
/* Prototypes */
/**************/

/* ------- */
/* Parsing */
/* ------- */

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


/* ----------------- */
/* Built-in commands */
/* ----------------- */

/**
 * Execute the built-in 'exit' command.
 *
 * @param 	arguments 	array of string which contains the command and its arguments
 * @param 	cmd_list 	a pointer to the command list
 */
void cmd_exit(char** arguments, vector* cmd_list);

/**
 * Execute the built-in 'cd' command.
 *
 * @param 	arguments 	array of string which contains the command and its arguments
 */
void cmd_cd(char** arguments);

/**
 * Execute the built-in 'showlist' command.
 *
 * @param 	cmd_list 	a pointer to the command list
 */
void cmd_showlist(vector* cmd_list);

/**
 * Execute the built-in 'memdump' command.
 *
 * @param 	cmd_list 	a pointer to the command list
 *
 * @return 	a pointer to a vector
 */
vector* cmd_memdump(vector* cmd_list);

/**
 * Execute the built-in 'loadmem' command.
 *
 * @param 	cmd_list 	a pointer to the command list
 *
 * @return 	a pointer to a vector
 */
vector* cmd_loadmem(vector* cmd_list);

/**
 * Execute the built-in 'sys' command.
 *
 * @param 	arguments 	array of string which contains the command and its arguments
 */
void cmd_sys(char** arguments);


/* --------------------- */
/* Non built-in commands */
/* --------------------- */

/**
 * Execute a command (with its arguments) once and
 * save the history of the execution in vector 'cmd_list'.
 *
 * @param 	arguments 	array of string which contains the command and its arguments
 * @param 	cmd_list 	a pointer to the command list
 */
void exec_once(char** arguments, vector* cmd_list);

/**
 * Execute a command (with its arguments) 'number' times
 * sequentially and save the history of the execution
 * in vector 'cmd_list'.
 *
 * @param 	arguments 	array of string which contains the command and its arguments
 * @param 	cmd_list 	a pointer to the command list
 * @param 	number 		the number of times to run the command
 */
void exec_sequential(char** arguments, vector* cmd_list, int number);

/**
 * Execute a command (with its arguments) 'number' times
 * in a parallel way and save the history of the execution
 * in vector 'cmd_list'.
 *
 * @param 	arguments 	array of string which contains the command and its arguments
 * @param 	cmd_list 	a pointer to the command list
 * @param 	number 		the number of times to run the command
 */
void exec_parallel(char** arguments, vector* cmd_list, int number);

#endif
