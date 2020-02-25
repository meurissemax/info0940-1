# INFO0940-1 - Operating systems
# University of Liege
# Academic year 2019-2020

# Group 1
# 20161278 - MEURISSE Maxime
# 20162864 - VERMEYLEN Valentin

CC = gcc
CFLAGS = -g --pedantic -Wall -Wextra -Wmissing-prototypes -std=c99
PROGRAM = oshell

FILES = main.c oshell.c vector.c

# To compile the project
all : 
	$(CC) $(FILES) $(CFLAGS) -o $(PROGRAM)

# To clean the workspace
clean :
	rm -f oshell
	rm -f -r oshell.dSYM
