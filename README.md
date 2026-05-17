# dumbsh - A Custom Linux Shell from Scratch

`dumbsh` (Dumb Shell) is a lightweight, educational Linux shell implementation written in C. 
It serves as a practical example of how modern shells like `bash`, `zsh`, or `fish` work under the hood, focusing on the fundamental stages of command execution: tokenization, parsing, and abstract syntax tree (AST) construction.

This project is a key part of my journey toward becoming a systems and low-level programmer, serving as hands-on practice for my future career.

## Features

- **Lexical Analysis (Tokenizer):** Converts raw input strings into a stream of meaningful tokens.
  - Supports words, single quotes (`'`), and double quotes (`"`).
  - Recognizes special operators: `|` (pipe), `&&` (AND), `||` (OR), and `&` (background).
- **Abstract Syntax Tree (AST):** Builds a structured representation of commands to handle complex execution logic.
- **Recursive Descent Parser:** Implements a parser that understands:
  - Simple commands and arguments.
  - Pipelines (`cmd1 | cmd2`).
  - Logical sequences (`cmd1 && cmd2` or `cmd1 || cmd2`).
- **Educational Design:** Clean code structure aimed at students and developers curious about systems programming.

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

This will generate the `dumpsh` executable.

### Running

Currently, `dumbsh` is in its early stages. You can run the executable to see the tokenization and parsing of a sample command:

```bash
./dumpsh
```

## Core Concepts

The development of `dumbsh` is guided by standard compiler construction principles applied to shell design:

1.  **Lexical Analysis (Tokenization):** The raw input string is scanned and broken down into discrete *tokens* (e.g., words, operators like `|` or `&&`, and quoted strings). This stage handles the complexities of shell syntax like escaping and quoting.
2.  **Recursive Descent Parsing:** The stream of tokens is consumed by a top-down parser. It follows a set of grammar rules to recognize the structure of the command, such as identifying if a command is part of a pipeline or a logical sequence.
3.  **Abstract Syntax Tree (AST):** The parser produces an AST, a hierarchical tree representation of the command.
    - **Leaf Nodes:** Represent simple commands with their arguments.
    - **Branch Nodes:** Represent operations that connect commands, such as Pipes (`|`), AND (`&&`), and OR (`||`) operators.
4.  **Separation of Concerns:** The project is modularized into distinct components for tokenization and parsing, making the codebase easier to understand and extend for educational purposes.

## License

This project is open-source and intended for educational use.
