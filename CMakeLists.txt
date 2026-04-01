cmake_minimum_required(VERSION 3.21)
project(file_system C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(file_system
        src/commands.c
        src/commands.h
        src/constants.h
        src/general_header.h
        src/main.c
        src/main.h
        src/output_message.h
        src/return_values.h
        src/structures.h
        src/utilities.c
        src/utilities.h
        src/vfs.c
        src/vfs.h)
