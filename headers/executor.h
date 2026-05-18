#ifndef DUMBSH_EXECUTOR_H
#define DUMBSH_EXECUTOR_H

#include "parser.h"
#include "sys_proc.h"

XResult(int) exec_node(const ASTNode *node);

#endif