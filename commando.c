#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "commando.h"


int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    int echo = 0;
    int count = 0;
    char bufCommand[MAX_CMDS];
    char *tokens[ARG_MAX + 1];
    char *env = "COMMANDO_ECHO";
    int countCommand;
    cmdcol_t cmdcol;

    while(1) {
        printf("@> ");

        if(fgets(bufCommand, MAX_CMDS, stdin) == NULL) {        //EOF
            break;
        }

        parse_into_tokens(bufCommand, tokens, &countCommand);

        if(countCommand == 0) {     //If there is no command or user just hit enter, we break out of loop
            break;
        }

        if(strcmp(tokens[1], "--echo") == 0 || getenv(env) != NULL) {           //If user input --echo or set environment variables
            echo = 1;
        }

        if(strcmp(tokens[0], "help") == 0) {
            if(echo) {
                printf("help\n");
            }
            printf("COMMANDO COMMANDS\n");
            printf("help : show this message\n");
            printf("exit : exit the program\n");
            printf("list : list all jobs that have been started giving information on each\n");
            printf("pause nanos secs : pause for the given number of nanoseconds and seconds\n");
            printf("output-for int : print the output for given job number\n");
            printf("output-all : print output for all jobs\n");
            printf("wait-for int : wait until the given job number finishes\n");
            printf("wait-all : wait for all jobs to finish\n");
            printf("command arg1 ... : non-built-in is run as a job\n");
        }

        if(strcmp(tokens[0], "exit") == 0 || COMMANDO_ECHO) {
            if(echo) {
                printf("exit\n");
            }
            break;
        }

        if(strcmp(tokens[0], "list") == 0 || COMMANDO_ECHO) {
            if(echo) {
                printf("list\n");
            }
            cmdcol_print(&cmdcol);

        }

        if(strcmp(tokens[0], "pause nanos secs") == 0 || COMMANDO_ECHO) {
            long nanos = atol(tokens[1]);
            int secs = atoi(tokens[2]);
            if(echo) {
                printf("pause nanos secs\n");
            }
            pause_for(nanos, secs);
        }

        if(strcmp(tokens[0], "output-for") == 0 || COMMANDO_ECHO) {
            int jobNumber = atoi(tokens[1]);
            if(echo) {
                printf("output-for %d \n", jobNumber);
            }
            cmd_print_output(cmdcol.cmd[jobNumber]);
        }

        if(strcmp(tokens[0], "output-all") == 0 || COMMANDO_ECHO) {
            if(echo) {
                printf("output-all\n");
            }
            for(int i = 0; i < cmdcol.size; i++) {
                cmd_print_output(cmdcol.cmd[i]);
            }
        }

        if(strcmp(tokens[0], "wait-for") == 0 || COMMANDO_ECHO) {
            int jobNumber = atoi(tokens[1]);
            if(echo) {
                printf("wait-for %d\n", jobNumber);
            }
            cmd_update_state(cmdcol.cmd[jobNumber], DOBLOCK);
        }

        if(strcmp(tokens[0], "wait-all") == 0 || COMMANDO_ECHO) {
            if(echo) {
                printf("output-all\n");
            }
            cmdcol_update_state(&cmdcol, DOBLOCK);
        }

        else {      //command args
            cmd_t *cmd = cmd_new(bufCommand);
            cmd_start(cmd);
        }
        
        cmdcol_update_state(&cmdcol, NOBLOCK);
        count++;
    }

    cmdcol_freeall(&cmdcol);
    return 0;
}