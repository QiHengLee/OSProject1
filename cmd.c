#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "commando.h"

cmd_t * cmd_new(char *argv[]) {
    cmd_t *cmd = malloc(sizeof(cmd_t));
    // copy all arguments into the cmd->argv
    // make sure the cdm->argv is null terminated
    int count;
    parse_into_tokens(argv, cmd->argv, &count);
    strcpy(cmd->name, strdup(argv[0]));
    cmd->pid = NULL;
    cmd->finished = 0;
    cmd->status = -1;
    snprintf(cmd->str_status,5,"%s","INIT");
    cmd->output = NULL;
}

void cmd_free(cmd_t *cmd) {
    free(cmd->output);
    free(cmd);
}

void cmd_start(cmd_t *cmd) {
    pipe(cmd->out_pipe);
    snprintf(cmd->str_status,4,"%s","RUN");
    pid_t child = fork();
    if(child == 0) {
        // int fd_backup = dup(STDOUT_FILENO);     //backup
        cmd->pid = getpid();
        dup2(cmd->out_pipe[PWRITE], STDOUT_FILENO);
        close(cmd->out_pipe[PREAD]);
        execvp(cmd->name, cmd->argv);
    }
    else {
        close(cmd->out_pipe[PWRITE]);
    }
}

void cmd_update_state(cmd_t *cmd, int block) {
    if(cmd->finished == 1) {    //Child is finished, do nothing
        return;
    }
    pid_t pid = cmd->pid;
    int status;
    waitpid(pid, &status, block);       
    if(WIFEXITED(status)) {             //If child exits
        int retval = WEXITSTATUS(status);       //return value of child
        cmd->status = retval;
        snprintf(cmd->str_status,8,"%s","EXIT(0)");
        cmd->finished = 1;
        cmd_fetch_output(cmd);
        printf("@!!! %s[#%d]: %s", cmd->name, cmd->pid, cmd->str_status);
    }
}

char *read_all(int fd, int *nread) {
    int max_size = 1024;
    int curr_position = 0;

    char *buf = malloc(max_size);

    while(1) {
        int curr_read_bytes = 0;
        if(curr_position >= max_size) {
            max_size *= 2;
            buf = realloc(buf, max_size);

            if(buf == NULL) {
                //print error if can't find sufficient space for reallocation?
            }
        }

        int max_read = max_size - curr_position;
        int curr_read_bytes = read(fd, buf+curr_position, max_read);

        if(curr_read_bytes <= 0) {      //If read returns 0 or lesser then 
            *nread = curr_position;     //Total bytes read 
            break;
        }

        curr_position += curr_read_bytes;       //Increment current position with current read bytes
    }
    
    int last_position = curr_position + 1;
    buf[last_position] = '\0';     
    
    return buf;
}

void cmd_fetch_output(cmd_t *cmd) {
    if (cmd->finished != 1) {
        printf("%s[#%d] not finished yet", cmd->name, cmd->pid);
    }
    else [
        cmd->output = read_all(cmd->out_pipe[PREAD], &cmd->output_size);
        close(cmd->out_pipe[PREAD]);
    ]
}

void cmd_print_output(cmd_t *cmd) {
    if (cmd->output == NULL) {
        printf("%s[#%d] : output not ready", cmd->name, cmd->pid);
    }
    else {
        write(STDOUT_FILENO, cmd->output, cmd->output_size);
    }
}
