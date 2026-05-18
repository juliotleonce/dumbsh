#include "../headers/executor.h"

#include <unistd.h>
#include <asm-generic/errno-base.h>

XResult(int) exec_cmd_node(const ASTNode *node);
XResult(int) exec_sequence_node(const ASTNode *node);
XResult(int) exec_pipe_node(const ASTNode *node);

XResult(int) exec_node(const ASTNode *node) {
    ASTNodeType type = node->type;

    switch (type) {
        case AST_NODE_CMD:
            return exec_cmd_node(node);
        case AST_NODE_PIPELINE:
            return exec_pipe_node(node);
        case AST_NODE_AND:
        case AST_NODE_OR:
            return exec_sequence_node(node);
    }

    return OK(int, 0);
}

XResult(int) exec_cmd_node(const ASTNode *node) {
    Proc proc = UNWRAP(sys_fork());

    if (sys_on_child_proc(proc)) {
        sys_make_foreground();

        XString *cmd = node->leaf.cmd;
        XArray_(XString) args = node->leaf.argv;
        XResult(int) exec_ret = sys_exec(cmd, args);
        if (IS_ERR(exec_ret)) {
            switch (ERR_CODE(exec_ret)) {
                case EACCES:
                    _exit(126);
                case ENOENT:
                    _exit(127);
                default:
                    _exit(ERR_CODE(exec_ret));
            }
        }

        _exit(UNWRAP(exec_ret));
    }

    int exit_status = UNWRAP(sys_wait(proc));
    switch (exit_status) {
        case 0:
            return OK(int, 0);
        case 126:
            return ERR(int, 126, "Permission refuse.");
        case 127:
            return ERR(int, 127, "Commande introuvable");
        default:
            return ERR(int, exit_status, "Command exited abnormally");
    }

}

XResult(int) exec_sequence_node(const ASTNode *node) {
    Proc proc1 = UNWRAP(sys_fork());
    if (sys_on_child_proc(proc1)) {
        int exit_code = TRY(exec_node(node->branch.left), int);
        _exit(exit_code);
    }

    int exit_status = UNWRAP(sys_wait(proc1));
    if (node->type == AST_NODE_AND && exit_status != 0) {
        return OK(int, exit_status);
    }

    Proc proc2 = UNWRAP(sys_fork());
    if (sys_on_child_proc(proc2)) {
        int exit_code = TRY(exec_node(node->branch.right), int);
        _exit(exit_code);
    }

    int proc2_exit_status = UNWRAP(sys_wait(proc2));
    return OK(int, proc2_exit_status);
}

XResult(int) exec_pipe_node(const ASTNode *node) {
    PipeChannel pipe = UNWRAP(sys_new_pipe_channel());
    Proc proc1 = UNWRAP(sys_fork());

    if (sys_on_child_proc(proc1)) {
        sys_pipe_stdout(pipe);
        int exit_code = TRY(exec_node(node->branch.left), int);
        _exit(exit_code);
    }

    Proc proc2 = UNWRAP(sys_fork());
    if (sys_on_child_proc(proc2)) {
        sys_pipe_stdin(pipe);
        int exit_code = TRY(exec_node(node->branch.right), int);
        _exit(exit_code);
    }

    sys_close_pipe_channel(pipe);
    int proc1_exit_status = UNWRAP(sys_wait(proc1));
    int proc2_exit_status = UNWRAP(sys_wait(proc2));
    int exit_code = proc1_exit_status | proc2_exit_status;
    return OK(int, exit_code);
}
