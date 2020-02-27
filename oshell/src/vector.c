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
#include <string.h>

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

	char* cmd_str = malloc(sizeof(char) * strlen(command));
	strcpy(cmd_str, command);

	i->command = cmd_str;
	i->pid = pid;
	i->exit_code = exit_code;

	v->items[v->total++] = i;
}

void vector_print(vector* v) {
	if(v->total > 0) {
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

vector* vector_export(vector* v) {
	FILE* fptr;

	if((fptr = fopen("memdump.bin", "wb")) == NULL) {
		perror("Unable to export vector in a binary file.");
	}

	fwrite(&v->total, sizeof(int), 1, fptr);

	for(int i = 0; i < v->total; i++) {
		int char_length = strlen(v->items[i]->command);

		fwrite(&char_length, sizeof(int), 1, fptr);
		fwrite(v->items[i]->command, sizeof(char), char_length + 1, fptr);

		fwrite(&v->items[i]->pid, sizeof(pid_t), 1, fptr);
		fwrite(&v->items[i]->exit_code, sizeof(int), 1, fptr);
	}

	fclose(fptr);

	vector_free(v);

	return vector_init();
}

vector* vector_import(vector* v) {
	vector_free(v);

	vector* imported_v = vector_init();

	FILE* fptr;

	if((fptr = fopen("memdump.bin", "rb")) == NULL) {
		perror("Unable to import vector from a binary file.");
	}

	int nb_items = 0;

	fread(&nb_items, sizeof(int), 1, fptr);

	for(int i = 0; i < nb_items; i++) {
		int char_length = 0;

		pid_t pid;
		int exit_code;

		char* command = malloc(sizeof(char) * char_length);

		fread(&char_length, sizeof(int), 1, fptr);
		fread(command, sizeof(char), char_length + 1, fptr);

		fread(&pid, sizeof(pid_t), 1, fptr);
		fread(&exit_code, sizeof(int), 1, fptr);

		vector_add(imported_v, command, pid, exit_code);

		free(command);
	}

	fclose(fptr);

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
