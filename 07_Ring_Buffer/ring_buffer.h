#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#define TASK_COMMAND_LENGTH 16


struct event {
    int pid;
    int ppid;
    int uid;
    int retval;
    bool is_exit;
    char comm[TASK_COMMAND_LENGTH];
};
#endif