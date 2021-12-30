#!/bin/bash

TESTS_PATH="/p/course/cs537-yuvraj/tests/p1"

for utility in my-look my-rev pipe; do
    echo "*** Tests for $utility:"

    # for testnum in $( ls "$TESTS_PATH/p1-testing/$utility/tests/*.desc -v" ); do
    for testdesc in $( ls "$TESTS_PATH/$utility" -v | grep ".desc" ); do

        # echo -n $testdesc
        echo -n "  $testdesc - "
        echo $(head -1 $TESTS_PATH/$utility/$testdesc)
    done
done
