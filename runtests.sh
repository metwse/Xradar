#!/bin/bash

export MODE=debug

make -C tests -j

for test in $(ls ./target/tests/*.test); do
    valgrind --fair-sched=yes $test > $test.log
done

gcovr *
