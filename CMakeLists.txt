cmake_minimum_required(VERSION 4.2)
project(dumbsh C)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local" CACHE PATH "..." FORCE)
endif()

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")

find_package(xstdlib REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(dumbsh src/main.c
        headers/tokenizer.h
        src/tokenizer.c
        headers/parser.h
        src/parser.c
        headers/sys_proc.h
        src/sys_proc.c
        headers/executor.h
        src/executor.c
        headers/repl.h
        src/repl.c)

target_link_libraries(dumbsh PRIVATE xstdlib::xstdlib)

install(TARGETS dumbsh RUNTIME DESTINATION bin)
