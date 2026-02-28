#!/bin/bash

export MODE=debug

make -j all

for test in $(ls ./target/debug/*.test); do
    valgrind --fair-sched=yes $test > $test.log
done

gcovr *
