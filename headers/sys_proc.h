#ifndef DUMBSH_SYS_PROC_H
#define DUMBSH_SYS_PROC_H

#include <xstdlib/xstdlib.h>
#include <sys/types.h>

typedef struct Proc {
    pid_t pid;
    int status;
} Proc;

typedef struct PipeChannel {
    int fd_in;
    int fd_out;
} PipeChannel;

DEFINE_XRESULT_OF(Proc);
DEFINE_XRESULT_OF(PipeChannel);
DEFINE_XRESULT_OF(int);

bool sys_on_child_proc(Proc proc);
void sys_close_pipe_channel(PipeChannel channel);
void sys_pipe_stdin(PipeChannel channel);
void sys_pipe_stdout(PipeChannel channel);
XResult(PipeChannel) sys_new_pipe_channel();
XResult(Proc) sys_fork();
XResult(Proc) sys_wait(Proc proc);
XResult(int) sys_exec(const XString *command, XArray_(XString) args);

#endif