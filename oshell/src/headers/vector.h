/**
 * INFO0940-1 - Operating systems
 * University of Liege
 * Academic year 2019-2020
 *
 * Group 1
 * 20161278 - MEURISSE Maxime
 * 20162864 - VERMEYLEN Valentin
 */

#ifndef VECTOR_H
#define VECTOR_H

/***********/
/* Headers */
/***********/

#include <unistd.h>
#include <sys/types.h>


/*******************/
/* Constant values */
/*******************/

// Initial capacity of the vector
static const int VECTOR_INIT_CAPACITY = 10;


/**************/
/* Structures */
/**************/

typedef struct _vector vector;


/**************/
/* Prototypes */
/**************/

/**
 * Initialize an empty vector.
 *
 * @return 	a pointer to a an empty vector
 */
vector* vector_init(void);

/**
 * Add a element to a vector. An element is composed of :
 * 		- command : the command executed by a process
 * 		- pid : the pid of the process
 * 		- exit_code : the exit code of the process
 *
 * @param 	v 			a pointer to a vector
 * @param 	command 	the command ('string' format) executed by a process
 * @param 	pid 		the pid of the process
 * @param 	exit_code 	the exit code of the process
 */
void vector_add(vector* v, char* command, pid_t pid, int exit_code);

/**
 * Print the elements of a vector in stdout formatted as follow :
 * 		(command;pid;exit_code) -> ...
 *
 * @param v a pointer to a vector
 */
void vector_print(vector* v);

/**
 * Write the content of a vector to a binary file and free
 * the vector.
 *
 * @param 	v 	a pointer to a vector
 *
 * @return 	a pointer to a free vector
 */
vector* vector_export(vector* v);

/**
 * Read the content of a binary file and load it (by replacing
 * all elements) in a vector.
 *
 * @param 	v 	a pointer to a vector
 *
 * @return 	a pointer to the new loaded vector
 */
vector* vector_import(vector* v);

/**
 * Free a vector and all its components.
 *
 * @param 	v 	a pointer to a vector
 */
void vector_free(vector *v);

#endif
