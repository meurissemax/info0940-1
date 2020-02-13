/*
Group: XX
Member1: ID - LastName - FirstName
Member2: ID - LastName - FirstName
*/

#ifndef oshell_h
#define oshell_h

// You can add new header(s), prototype(s) and constant(s)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int MAX_CMD_SIZE = 256;                // DO NOT MODIFY
static const int MAX_ARGS = 256;                    // DO NOT MODIFY

char readCharInput(void);                           // DO NOT MODIFY
void parseCmdLine(char* line, char** arguments);    // DO NOT MODIFY

#endif /* oshell_h */
