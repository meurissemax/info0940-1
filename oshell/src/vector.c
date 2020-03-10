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
#include <fcntl.h>
#include <sys/stat.h>

#include "headers/vector.h"


/**************/
/* Structures */
/**************/

typedef struct item {
	char* command;
	pid_t pid;
	int exit_code;
} item;

struct _vector {
	// Capacity of the vector
	int capacity;

	// Number of elements in the vector
	int total;

	// Elements of the vector
	item** items;
};


/*************/
/* Functions */
/*************/

vector* vector_init(void) {
	// We allocate the vector
	vector* v = malloc(sizeof(vector));

	if(v == NULL) {
		perror("Memory allocation error");
		exit(1);
	}

	// We set the elements of the vector
	v->capacity = VECTOR_INIT_CAPACITY;
	v->total = 0;
	v->items = malloc(sizeof(item*) * v->capacity);

	if(v->items == NULL) {
		free(v);

		perror("Memory allocation error");
		exit(1);
	}

	return v;
}

/**
 * Resize a vector. The new capacity of the vector is defined
 * by the 'capacity' paramater. All existing values in the
 * vector are kept in the resized vector.
 *
 * @param 	v 			a pointer to a vector
 * @param 	capacity 	the new capacity of the vector
 */
static void vector_resize(vector* v, int capacity) {
	if(v == NULL || capacity < v->capacity)
		return;

	// We reallocate items of the vector
	item** items = realloc(v->items, sizeof(item*) * capacity);

	if(items == NULL) {
		vector_free(v);

		perror("Memory allocation error");
		exit(1);
	}

	// We update elements of the vector
	v->capacity = capacity;
	v->items = items;
}

void vector_add(vector* v, char* command, pid_t pid, int exit_code) {
	if(v == NULL || command == NULL)
		return;

	// If the vector is full, we double his capacity
	if(v->capacity == v->total)
		vector_resize(v, v->capacity * 2);

	// We allocate the item to add
	item* i = malloc(sizeof(item));

	if(i == NULL) {
		vector_free(v);

		perror("Memory allocation error");
		exit(1);
	}

	// We allocate the command in order to copy it (else we will store
	// a pointer to a command that will change during the execution)
	char* c_command = malloc(sizeof(char) * (strlen(command) + 1));

	if(c_command == NULL) {
		vector_free(v);

		perror("Memory allocation error");
		exit(1);
	}

	// We copy the command
	strcpy(c_command, command);

	// We set the elements of the item
	i->command = c_command;
	i->pid = pid;
	i->exit_code = exit_code;

	// We add the item to the vector
	v->items[v->total++] = i;
}

void vector_print(vector* v) {
	if(v == NULL)
		return;

	// We check if there is elements in the vector
	if(v->total > 0) {

		// We iterate over each element
		for(int i = 0; i < v->total; i++) {
			printf("(%s;%d;%d)", v->items[i]->command, v->items[i]->pid, v->items[i]->exit_code);

			if(i == v->total - 1) {
				printf("\n");
			} else {
				printf("->");
			}
		}
	}
}

void vector_export(vector* v, char* path) {
	if(v == NULL || path == NULL)
		return;

	int fd;

	// We try to open the file
	if((fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IXGRP)) == -1) {
		fprintf(stderr, "Unable to open '%s'.\n", path);

		return;
	}

	// We write, at first, the number of elements in the vector
	if(write(fd, &v->total, sizeof(v->total)) == -1)
		return;

	// We iterate over each element to write
	for(int i = 0; i < v->total; i++) {
		int cmd_length = strlen(v->items[i]->command);

		if(write(fd, &cmd_length, sizeof(cmd_length)) == -1)
			return;

		if(write(fd, v->items[i]->command, sizeof(char) * (cmd_length + 1)) == -1)
			return;

		if(write(fd, &v->items[i]->pid, sizeof(v->items[i]->pid)) == -1)
			return;

		if(write(fd, &v->items[i]->exit_code, sizeof(v->items[i]->exit_code)) == -1)
			return;
	}

	// We close the file
	close(fd);

	// We free the current vector and return a new (empty) vector
	vector_reset(v);
}

void vector_import(vector* v, char* path) {
	if(v == NULL || path == NULL)
		return;

	int fd;

	// We try to open the file
	if((fd = open(path, O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to read '%s'.\n", path);

		return;
	}

	// We free the current vector
	vector_reset(v);

	// Number of elements in the vector
	int total = 0;
	
	if(read(fd, &total, sizeof(total)) == -1)
		return;

	// We iterate over each element to import
	for(int i = 0; i < total; i++) {
		int cmd_length = 0, exit_code;
		pid_t pid;

		if(read(fd, &cmd_length, sizeof(cmd_length)) == -1)
			return;

		char* command = malloc(sizeof(char) * (cmd_length + 1));

		if(read(fd, command, sizeof(char) * (cmd_length + 1)) == -1)
			return;
		
		if(read(fd, &pid, sizeof(pid)) == -1)
			return;

		if(read(fd, &exit_code, sizeof(exit_code)) == -1)
			return;

		vector_add(v, command, pid, exit_code);

		free(command);
	}

	// We close the fil
	close(fd);
}

void vector_reset(vector* v) {
	if(v == NULL)
		return;

	for(int i = 0; i < v->total; i++) {
		free(v->items[i]->command);
		free(v->items[i]);
	}

	free(v->items);

	v->capacity = VECTOR_INIT_CAPACITY;
	v->total = 0;
	v->items = malloc(sizeof(item*) * v->capacity);
}

void vector_free(vector* v) {
	if(v == NULL)
		return;
	
	for(int i = 0; i < v->total; i++) {
		free(v->items[i]->command);
		free(v->items[i]);
	}

	free(v->items);
	free(v);
}
