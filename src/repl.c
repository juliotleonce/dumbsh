#include "../headers/repl.h"
#include "../headers/executor.h"
#include <xstdlib/xstdlib.h>

XResult(int) eval_line(XString *line);

void repl_run() {
    while (1) {
        XMEM_SCOPE {
            XString *line = xio_read_console_line("$dumbsh# ");
            if (xstring_equal_c_str(line, "exit")) return;

            XResult(int) eval_ret = eval_line(line);
            if (IS_ERR(eval_ret)) {
                printf("erreur: %s\n", ERR_MSG(eval_ret));
            }
        }
    }
}

XResult(int) eval_line(XString *line) {

    GUARD_BLOCK {
        XArray_(Token) tokens = TRY(tokenize(line), int);
        ASTNode *node = parse(tokens);
        TRY(exec_node(node), int);
    }

    return OK(int, 0);
}