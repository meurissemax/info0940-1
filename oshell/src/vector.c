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
	vector* v = malloc(sizeof(vector));

	if(v == NULL) {
		perror("Memory allocation error");
		exit(1);
	}

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
	item** items = realloc(v->items, sizeof(item*) * capacity);

	if(items == NULL) {
		vector_free(v);

		perror("Memory allocation error");
		exit(1);
	}

	v->capacity = capacity;
	v->items = items;
}

void vector_add(vector* v, char* command, pid_t pid, int exit_code) {
	if(v->capacity == v->total) {
		vector_resize(v, v->capacity * 2);
	}

	item* i = malloc(sizeof(item));

	if(i == NULL) {
		vector_free(v);

		perror("Memory allocation error");
		exit(1);
	}

	char* c_command = malloc(sizeof(char) * strlen(command));

	if(c_command == NULL) {
		vector_free(v);

		perror("Memory allocation error");
		exit(1);
	}

	strcpy(c_command, command);

	i->command = c_command;
	i->pid = pid;
	i->exit_code = exit_code;

	v->items[v->total++] = i;
}

void vector_print(vector* v) {
	if(v->total > 0) {
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

vector* vector_export(vector* v) {
	int fd;

	if((fd = open("memdump.bin", O_WRONLY | O_CREAT)) == -1) {
		fprintf(stderr, "Unable to export vector in a binary file.");
	}

	write(fd, &v->total, sizeof(v->total));

	for(int i = 0; i < v->total; i++) {
		int char_length = strlen(v->items[i]->command);

		write(fd, &char_length, sizeof(char_length));
		write(fd, v->items[i]->command, sizeof(char) * (char_length + 1));

		write(fd, &v->items[i]->pid, sizeof(v->items[i]->pid));
		write(fd, &v->items[i]->exit_code, sizeof(v->items[i]->exit_code));
	}

	close(fd);

	vector_free(v);

	return vector_init();
}

vector* vector_import(vector* v) {
	vector_free(v);

	vector* imported_v = vector_init();

	int fd;

	if((fd = open("memdump.bin", O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to import vector from a binary file.");
	}

	int nb_items = 0;

	read(fd, &nb_items, sizeof(nb_items));

	for(int i = 0; i < nb_items; i++) {
		int char_length = 0;

		pid_t pid;
		int exit_code;

		char* command = malloc(sizeof(char) * char_length);

		read(fd, &char_length, sizeof(char_length));
		read(fd, command, sizeof(char) * (char_length + 1));

		read(fd, &pid, sizeof(pid));
		read(fd, &exit_code, sizeof(exit_code));

		vector_add(imported_v, command, pid, exit_code);

		free(command);
	}

	close(fd);

	return imported_v;
}

void vector_free(vector* v) {
	for(int i = 0; i < v->total; i++) {
		free(v->items[i]->command);
		free(v->items[i]);
	}

	free(v->items);
	free(v);
}
