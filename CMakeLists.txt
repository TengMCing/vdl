cmake_minimum_required(VERSION 3.23)
project(vdl C)

# The latest released standard is used to get those modern language features
set(CMAKE_C_STANDARD 17)

# Enable some CMake messages
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Now is using -Weverything to give as many warnings as possible. In the final build, -Wall -Wextra -Wconversion -Wpedantic -Werror should be used.
# -Wno-extra-semi: Stop warning ";" out of no where. Usually caused by macro.
# -Wno-gnu-zero-variadic-macro-arguments: A GNU feature (supported by gcc and clang) that needed to be used for this project.
# -Wno-documentation-unknown-command: A weird warning about the documentation.
# -Wno-documentation: Another weird warning about the documentation.
# -Wno-declaration-after-statement: A very old warning related to the C90 standard that could be omitted under C17.
# -Wno-gnu-statement-expression: A GNU feature that needed to to used to implement clean macros for modules like backtrace, Exception handling, etc.
# -Wno-shadow: Some constructors provided in this library need to nest statement expression together.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-extra-semi -Wno-gnu-zero-variadic-macro-arguments -Wno-documentation-unknown-command -Wno-documentation -Wno-declaration-after-statement -Wno-gnu-statement-expression -Wno-shadow -Weverything")

add_executable(
        vdl
        main.c
        include/vdl.h include/vdl_1_utilities.h include/vdl_2_exception.h include/vdl_2_exception_def.h include/vdl_3_backtrace.h include/vdl_3_backtrace_def.h include/vdl_5_vector_basic.h include/vdl_5_vector_basic_def.h include/vdl_6_garbage_collector.h include/vdl_6_garbage_collector_def.h include/vdl_7_vector_memory.h include/vdl_7_vector_memory_def.h include/vdl_8_vector_portal.h include/vdl_4_integer_overflow.h include/vdl_4_integer_overflow_def.h include/vdl_8_vector_portal_def.h)
