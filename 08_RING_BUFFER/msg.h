#ifndef __MSG_H__
#define __MSG_H__

struct my_msg {
    int pid;
    char command[128];
    char pathname[128];
};

#endif 