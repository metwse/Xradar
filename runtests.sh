#!/bin/bash

export MODE=debug

make -C tests -j

for test in $(ls ./target/debug/*.test); do
    valgrind $test > $test.log
done

gcovr *
