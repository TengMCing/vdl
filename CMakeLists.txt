cmake_minimum_required(VERSION 3.23)
project(vdl C)

set(CMAKE_C_STANDARD 17)

add_executable(vdl main.c vecdef.h vecmem.h vecutil.h vecerror.h vecgc.h vectype.h)
