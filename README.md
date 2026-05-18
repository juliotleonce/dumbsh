# dumbsh - A Custom Linux Shell from Scratch

`dumbsh` (Dumb Shell) is a lightweight, educational Linux shell implementation written in C. 
It serves as a practical example of how modern shells like `bash`, `zsh`, or `fish` work under the hood, focusing on the fundamental stages of command execution: tokenization, parsing, and abstract syntax tree (AST) construction.

This project is a key part of my journey toward becoming a systems and low-level programmer, serving as hands-on practice for my future career.

## Features

`dumbsh` currently supports the following features:

- **Interactive REPL:** A standard Read-Eval-Print Loop with a custom prompt (`$dumbsh# `).
- **Command Execution:** Executes external programs found in the system's `PATH`.
- **Pipelines:** Supports connecting commands via pipes (`cmd1 | cmd2`), allowing the output of one command to be the input of another.
- **Logical Operators:** Supports conditional execution using `&&` (AND) and `||` (OR) operators.
- **Quoting:** Handles single (`'`) and double (`"`) quotes for arguments containing spaces or special characters.
- **Robust Tokenization & Parsing:** Uses a hand-written lexical analyzer and a recursive descent parser to build an Abstract Syntax Tree (AST) for reliable command execution.
- **Built-in Commands:** Includes basic built-ins like `exit` to terminate the shell session.

## Future Features

We are actively working on expanding `dumbsh`. Planned features for the near future include:

- **I/O Redirection:** Supporting `>`, `<`, and `>>` for redirecting standard input, output, and error.
- **Environment Variables:** Ability to expand and manage environment variables (e.g., `$HOME`, `$PATH`).
- **More Built-in Commands:** Adding `cd`, `pwd`, `export`, and `unset`.
- **Background Execution:** Full support for running commands in the background using the `&` operator.
- **Command History:** Navigating and re-executing previous commands using arrow keys.
- **Tab Completion:** Auto-completing command names and file paths.

## Project Structure

- `src/`: Implementation of the shell logic.
  - `main.c`: Entry point and demonstration of the lexer/parser.
  - `tokenizer.c`: Lexical analyzer.
  - `parser.c`: Recursive descent parser and AST builder.
  - `sys_proc.c`: System calls wrapper for process management.
- `headers/`: Header files defining the core data structures (Tokens, AST Nodes).
- `CMakeLists.txt`: Build configuration.

## Getting Started

### Prerequisites

- A C compiler (GCC or Clang)
- CMake (version 3.10 or higher)
- `xstdlib`: This project depends on a custom utility library that you can find on my github repo [here](https://github.com/juliotleonce/xstdlibc).

### Building

To build the project, use the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

This will generate the `dumbsh` executable.

### Running

To start the interactive shell, simply run the generated executable:

```bash
./dumbsh
```

You can then type commands just like in any other shell. For example:
```bash
$dumbsh# ls -l | grep ".c"
$dumbsh# echo "Hello, world!" && exit
```

## Core Concepts

The development of `dumbsh` is guided by standard compiler construction principles applied to shell design:

1.  **Lexical Analysis (Tokenization):** The raw input string is scanned and broken down into discrete *tokens* (e.g., words, operators like `|` or `&&`, and quoted strings). This stage handles the complexities of shell syntax like escaping and quoting.
2.  **Recursive Descent Parsing:** The stream of tokens is consumed by a top-down parser. It follows a set of grammar rules to recognize the structure of the command, such as identifying if a command is part of a pipeline or a logical sequence.
3.  **Abstract Syntax Tree (AST):** The parser produces an AST, a hierarchical tree representation of the command.
    - **Leaf Nodes:** Represent simple commands with their arguments.
    - **Branch Nodes:** Represent operations that connect commands, such as Pipes (`|`), AND (`&&`), and OR (`||`) operators.
4.  **Execution Engine:** The shell traverses the AST and executes the commands.
    - **Process Management:** Uses `fork()` and `execvp()` to run external programs.
    - **Inter-Process Communication:** Implements pipelines using `pipe()` and `dup2()` to connect the standard output of one process to the standard input of another.
    - **Logical Control Flow:** Evaluates the exit status of commands to decide whether to continue execution in `&&` and `||` sequences.
5.  **Separation of Concerns:** The project is modularized into distinct components for tokenization, parsing, and execution, making the codebase easier to understand and extend for educational purposes.

## License

This project is open-source and intended for educational use.
