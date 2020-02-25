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

/***********/
/* Headers */
/***********/

#include <stdio.h>
#include <stdlib.h>

#include "headers/vector.h"


/*************/
/* Functions */
/*************/

vector* vector_init() {
	vector* v = malloc(sizeof(vector));

	if(v == NULL) {
		perror("Error while initializing vector.");
		exit(1);
	}

	v->capacity = VECTOR_INIT_CAPACITY;
	v->total = 0;
	v->items = malloc(sizeof(item*) * v->capacity);

	return v;
}

void vector_resize(vector* v, int capacity) {
	item** items = realloc(v->items, sizeof(item*) * capacity);

	if(items == NULL) {
		perror("Error while resizing the vector.");
		exit(1);
	}

	v->items = items;
	v->capacity = capacity;
}

void vector_add(vector* v, char* command, pid_t pid, int exit_code) {
	item* i = malloc(sizeof(item));

	if(i == NULL) {
		perror("Error while creating new item.");
		exit(1);
	}

	i->command = command;
	i->pid = pid;
	i->exit_code = exit_code;

	v->items[v->total++] = i;
}

void vector_print(vector* v) {
	if(v->total == 0) {
		printf("List is empty.\n");
	} else {
		for(int i = 0; i < v->total; i++) {
			item* e = v->items[i];

			printf("(%s;%d;%d)", e->command, e->pid, e->exit_code);

			if(i == v->total - 1) {
				printf("\n");
			} else {
				printf("->");
			}
		}
	}
}

void vector_free(vector* v) {
	for(int i = 0; i < v->total; i++) {
		free(v->items[i]);
	}

	free(v->items);
	free(v);
}
