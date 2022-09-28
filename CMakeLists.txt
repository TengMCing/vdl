cmake_minimum_required(VERSION 3.23)
project(vdl C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wconversion -Wpedantic -Wno-extra-semi -Wno-gnu-zero-variadic-macro-arguments -Wno-documentation-unknown-command -Wno-documentation -Weverything")

add_executable(vdl main.c vecdef.h vecmem.h vecutil.h vecerror.h vecgc.h)
