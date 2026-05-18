#include "../headers/sys_proc.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

bool sys_on_child_proc(Proc proc) {
    return proc.pid == 0;
}

void sys_close_pipe_channel(PipeChannel channel) {
    close(channel.fd_in);
    close(channel.fd_out);
}

void sys_pipe_stdin(PipeChannel channel) {
    close(channel.fd_out);
    dup2(channel.fd_in, STDIN_FILENO);
    close(channel.fd_in);
}

void sys_pipe_stdout(PipeChannel channel) {
    close(channel.fd_in);
    dup2(channel.fd_out, STDOUT_FILENO);
    close(channel.fd_out);
}

int sys_open_file(const XString *path, int flags) {
    return open(path->c_str, flags, 0644);
}

void sys_redirect_stdout(int fd) {
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void sys_redirect_stdin(int fd) {
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void sys_redirect_append(int fd) {
    dup2(fd, STDOUT_FILENO);
    fcntl(STDOUT_FILENO, F_SETFL, O_APPEND);
    close(fd);
}

void sys_make_foreground() {
    setpgid(0, 0);
}

XResult(PipeChannel) sys_new_pipe_channel() {
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
        return ERR(PipeChannel, errno, strerror(errno));

    PipeChannel proc_pipe = { .fd_in = pipe_fd[0], .fd_out = pipe_fd[1] };
    return OK(PipeChannel, proc_pipe);
}

XResult(Proc) sys_fork() {
    pid_t pid = fork();

    if (pid == -1)
        return ERR(Proc, errno, strerror(errno));

    Proc proc = { .pid = pid };
    return OK(Proc, proc);
}

XResult(int) sys_wait(Proc proc) {
    int exit_status;
    if (waitpid(proc.pid , &exit_status, 0) == -1)
        return ERR(int, errno, strerror(errno));
    if (WIFEXITED(exit_status))
        return OK(int, WEXITSTATUS(exit_status));
    return ERR(int, -1, "Process exited abnormally");
}

XResult(int) sys_exec(const XString *command, XArray_(XString) args) {
    char *argv[args->length + 2];
    for (int i = 0; i < args->length; i++) {
        XString *arg = xarray_at(args, i);
        argv[i] = arg->c_str;
    }

    argv[args->length] = NULL;
    argv[args->length + 1] = NULL;

    execvp(command->c_str, argv);

    return ERR(int, errno, strerror(errno));
}