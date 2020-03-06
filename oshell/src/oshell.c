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

#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "headers/oshell.h"


/*************/
/* Variables */
/*************/

int sigalrm_flag = 0;


/*************/
/* Functions */
/*************/

void parseCmdLine(char* line, char** arguments) {
	int i = 0;

	line[strlen(line) - 1] = '\0';
	arguments[i] = strtok(line, " ");

	while(arguments[i++] && i < MAX_ARGS)
        arguments[i] = strtok(NULL, " ");
}

char readCharInput(void) {
	char c = getchar();

	while(getchar() != '\n');

	return c;
}

void cmd_exit(char** arguments, vector* cmd_list) {
    if(arguments[1] != NULL)
        fprintf(stderr, "Arguments passed after the exit command have been discarded\n");

    vector_free(cmd_list);

    exit(0);
}

void cmd_cd(char** arguments) {
	// Go to home directory
    if(arguments[1] == NULL || strcmp(arguments[1], "~") == 0) {
        chdir(getenv("HOME"));
    }

    // Go up a directory
    else if(strcmp(arguments[1], "..") == 0) {
        chdir("..");
    }

    // Go in a particular directory
    else if((char) arguments[1][0] == '/') {
        int v = chdir(arguments[1]);

        if(v != 0) {
            fprintf(stderr, "Unknown directory, or unable to open it\n");
        }
    }

    // Go in a subdirectory
    else {
        int v = chdir(arguments[1]);

        if(v != 0) {
            fprintf(stderr, "Unknown subdirectory, or unable to open it\n");
        }
    }
}

void cmd_showlist(vector* cmd_list) {
    vector_print(cmd_list);
}

vector* cmd_memdump(vector* cmd_list) {
    return vector_export(cmd_list);
}

vector* cmd_loadmem(vector* cmd_list) {
    return vector_import(cmd_list);
}

static char* get_state_details(char state) {
    switch(state) {
        case 'R' : return "R (running)";
        case 'S' : return "S (sleeping)";
        case 'D' : return "D (waiting)";
        case 'Z' : return "Z (zombie)";
        case 'T' : return "T (stopped)";
        default : return "unknown";
    }
}

static void cmd_sys_netstats() {
    FILE* fptr;

    // We try to open this file (if it exists)
    if((fptr = fopen("/proc/net/dev", "r")) == NULL) {
        fprintf(stderr, "Unable to get netstat information\n");

        return;
    }

    char buffer[255], ifname[255];

    unsigned long int r_pkts, r_err, r_drop;
    unsigned long int s_pkts, s_err, s_drop;

    unsigned long int r_err_rate, r_drop_rate, s_err_rate, s_drop_rate;

    unsigned long int dummy;

    // We skip the first two lines
    for(int i = 0; i < 2; i++)
        fgets(buffer, 255, fptr);

    // We get and print the information, line by line
    while(fgets(buffer, 255, fptr)) {
        sscanf(
            buffer,
            "%[^:]: %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
            ifname, &dummy, &r_pkts, &r_err, &r_drop, &dummy, &dummy, &dummy, &dummy, &dummy, &s_pkts, &s_err, &s_drop
        );

        r_err_rate = r_err / r_pkts;
        r_drop_rate = r_drop / r_pkts;
        s_err_rate = s_err / s_pkts;
        s_drop_rate = s_drop / s_pkts;

        printf(
            "[%s]: Rx(pkts: %lu, err: %lu%c, drop: %lu%c) - Tx(pkts: %lu, err: %lu%c, drop: %lu%c)\n",
            ifname, r_pkts, r_err_rate, '%', r_drop_rate, '%', s_pkts, s_err_rate, '%', s_drop_rate, '%'
        );
    }

    fclose(fptr);
}

static void cmd_sys_devstats() {
    printf("sys devstats\n");
}

static void cmd_sys_stats(char** arguments) {
    /* missing argument */
    if(arguments[2] == NULL) {
        fprintf(stderr, "Missing <pid>\n");
    }

    /* no missing argument */
    else {
        // We check if the PID is valid
        for(unsigned long i = 0; i < strlen(arguments[2]); i++) {
            if(!isdigit(arguments[2][i])) {
                fprintf(stderr, "Wrong <pid>\n");

                return;
            }
        }

        /* We get stats about the process with this PID */
        FILE* fptr;

        // We construct the filename where stats are located
        char filename[12 + strlen(arguments[2])];

        strcpy(filename, "/proc/");
        strcat(filename, arguments[2]);
        strcat(filename, "/stat");

        // We try to open this file (if it exists)
        if((fptr = fopen(filename, "r")) == NULL) {
            fprintf(stderr, "Unable to get stats about this process\n");

            return;
        }

        // We get information of the file
        int dummy_d;
        unsigned dummy_u;
        unsigned long dummy_lu;
        long int dummy_ld;

        char process_name[255];
        char process_state;
        long int process_num_threads;

        fscanf(
            fptr,
            "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld",
            &dummy_d, process_name, &process_state, &dummy_d, &dummy_d, &dummy_d, &dummy_d, &dummy_d, &dummy_u, &dummy_lu, &dummy_lu, &dummy_lu, &dummy_lu, &dummy_lu, &dummy_lu, &dummy_ld, &dummy_ld, &dummy_ld, &dummy_ld, &process_num_threads
        );

        fclose(fptr);

        // We print the information
        char* process_name_trim = process_name;

        process_name_trim++;
        process_name_trim[strlen(process_name_trim) - 1] = 0;

        printf("Process name: %s\n", process_name_trim);
        printf("Process state: %s\n", get_state_details(process_state));
        printf("Process thread(s): %ld\n", process_num_threads);
    }
}

void cmd_sys(char** arguments) {
    /* missing argument */
    if(arguments[1] == NULL) {
        fprintf(stderr, "Missing argument\n");
    }

    /* 'sys netstats' command */
    else if(strcmp(arguments[1], "netstats") == 0) {
        cmd_sys_netstats();
    }

    /* 'sys devstats' command */
    else if(strcmp(arguments[1], "devstats") == 0) {
        cmd_sys_devstats();
    }

    /* 'sys stats' command */
    else if(strcmp(arguments[1], "stats") == 0) {
        cmd_sys_stats(arguments);
    }

    /* unknown argument */
    else {
        fprintf(stderr, "Unknown argument\n");
    }
}

void exec_once(char** arguments, vector* cmd_list) {
    exec_sequential(arguments, cmd_list, 1);
}

static void sigalrm_handler(int sig) {
    fprintf(stderr, "Process timeout\n");

    sigalrm_flag = sig;
}

void exec_sequential(char** arguments, vector* cmd_list, int number) {
    for(int i = 0; i < number; i++) {
        pid_t pid = fork();

        // If work has failed
        if(pid < 0) {
            perror("Fork error");
        }

        // If we are in the child process
        else if (pid == 0) {
            // Execute the command
            if(execvp(arguments[0], arguments) < 0) {
                perror("Execution error");
                exit(1);
            }
        } else {
            // Set the signal and the alarm for the timeout
            signal(SIGALRM, sigalrm_handler);
            alarm(TIME_LIMIT);

            // Status of the child process
            int status;

            // We wait for child or timeout
            while((wait(&status)) != -1 && sigalrm_flag == 0);

            // We add the command to the command list (timeout or not)
            vector_add(cmd_list, arguments[0], pid, WEXITSTATUS(status));

            // If a timeout occurs, we kill the child process
            if(sigalrm_flag == 1) {
                kill(pid, 9);
            }

            // We reset the timeout
            alarm(0);
            sigalrm_flag = 0;
        }
    }
}

void exec_parallel(char** arguments, vector* cmd_list, int number) {
    // Create the child processes
    for(int i = 0; i < number; i++) {
        pid_t pid = fork();

        // If work has failed
        if(pid < 0) {
            perror("Fork error");
        }

        // If we are in the child process
        else if(pid == 0) {
            // Execute the command
            if(execvp(arguments[0], arguments) < 0) {
                perror("Execution error");
                exit(1);
            }
        }
    }

    // Parent process waits for child processes
    while(number > 0) {
        // Status of the child process
        int status;

        // We wait for child
        pid_t pid = waitpid(-1, &status, 0);

        // We add the command to the command list
        vector_add(cmd_list, arguments[0], pid, WEXITSTATUS(status));

        number--;
    }
}
