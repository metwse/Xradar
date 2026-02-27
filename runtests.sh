#!/bin/bash

export MODE=debug

make -j all

for test in $(ls ./target/debug/*.test); do
    valgrind $test > $test.log
done

gcovr *
