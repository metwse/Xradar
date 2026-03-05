#!/bin/bash

export MODE=debug

make -C tests -j

for test in $(ls ./tests/target/*.test); do
    valgrind --fair-sched=yes $test > $test.log
done

gcovr *
