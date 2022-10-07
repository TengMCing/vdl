#!/bin/zsh

clang -Wno-extra-semi -Wno-gnu-zero-variadic-macro-arguments -Wno-documentation-unknown-command -Wno-documentation -Wno-declaration-after-statement -Weverything -DTEST_VDLASSERT -g tests.c -o tests && ./tests