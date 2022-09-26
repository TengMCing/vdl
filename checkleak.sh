#!/bin/zsh

# Enable Malloc log
export MallocStackLogging=1

# Use the debug version to run the memory check
leaks --atExit --list -- ./cmake-build-debug/vdl
