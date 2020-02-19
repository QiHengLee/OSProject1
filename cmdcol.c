#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "commando.h"

void cmdcol_add(cmdcol_t *col, cmd_t *cmd) {
    if(col->size == MAX_CMDS) {
        printf("Size exceeded");
        return;
    }
    col->cmd[col->size] = cmd;
    col->size += 1;
}
// Add the given cmd to the col structure. Update the cmd[] array and
// size field. Report an error if adding would cause size to exceed
// MAX_CMDS, the maximum number commands supported.

void cmdcol_print(cmdcol_t *col) {
    printf("%-5s", "JOB");
    printf("#");
    printf("%-8s", "PID");
    printf("%5s", "STAT");
    printf("%11s", "STR_STAT");
    printf("%5s", "OUTB");
    printf("COMMAND");

    for(int i = 0; i < col->size; i++) {
        cmd_t curr_cmd = col->cmd[i];
        printf("%-4d", i);
        printf("#");
        printf("%-8d", curr_cmd->pid);
        printf("%5d", curr_cmd->status);
        printf("%11s", curr_cmd->str_status);
        printf("%5s", curr_cmd->output_size);
        int j = 0;
        while(curr_cmd->argv[j] != NULL)) {
            printf("%s ", cmd->argv[j]);
            j++;
        }
    }
}
// Print all cmd elements in the given col structure.  The format of
// the table is
//
// JOB  #PID      STAT   STR_STAT OUTB COMMAND
// 0    #17434       0    EXIT(0) 2239 ls -l -a -F 
// 1    #17435       0    EXIT(0) 3936 gcc --help 
// 2    #17436      -1        RUN   -1 sleep 2 
// 3    #17437       0    EXIT(0)  921 cat Makefile 
// 
// Widths of the fields and justification are as follows
// 
// JOB  #PID      STAT   STR_STAT OUTB COMMAND
// 1234 #12345678 1234 1234567890 1234 Remaining
// left  left    right      right rigt left
// int   int       int     string  int string
//
// The final field should be the contents of cmd->argv[] with a space
// between each element of the array.

void cmdcol_update_state(cmdcol_t *col, int block) {
    for(int i = 0; i < col->size; i++) {
        cmd_update_state(col->cmd[i], block);
    }
}
// Update each cmd in col by calling cmd_update_state() which is also
// passed the block argument (either NOBLOCK or DOBLOCK) 

void cmdcol_freeall(cmdcol_t *col) {
    for(int i = 0; i < col->size; i++) {
        cmd_free(col->cmd[i]);
    }
}
// Call cmd_free() on all of the constituent cmd_t's.
