/**
 * INFO0940-1 - Operating systems
 * University of Liege
 * Academic year 2019-2020
 *
 * Group 1
 * 20161278 - MEURISSE Maxime
 * 20162864 - VERMEYLEN Valentin
 *
 * This implementation is partially adapted from :
 * https://eddmann.com/posts/implementing-a-dynamic-vector-array-in-c/
 */

#ifndef VECTOR_H
#define VECTOR_H


/*******************/
/* Constant values */
/*******************/

// Initial capacity of the vector
static const int VECTOR_INIT_CAPACITY = 10;


/**************/
/* Structures */
/**************/

typedef struct item {
	char* command;
	pid_t pid;
	int exit_code;
} item;

typedef struct vector {
	item **items;
	int capacity;
	int total;
} vector;


/**************/
/* Prototypes */
/**************/

vector* vector_init(void);
void vector_resize(vector* v, int capacity);
void vector_add(vector* v, char* command, pid_t pid, int exit_code);
void vector_print(vector* v);
void vector_free(vector *v);

#endif
