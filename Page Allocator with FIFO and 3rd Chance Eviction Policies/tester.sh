#!/bin/bash

rm test.out
touch test.out
for policies in 1 2
do
for trials in 1 4 5 8 12 16
do
for COUNT in 1 2 3 4 5 6 7 8 9 10 11 12
do
    FILE="result-$policies-$trials-input_$COUNT"

    ./proj3 $policies $trials sample_input/input_$COUNT

    diff <(sort "output/$FILE") <(sort "sample_output/$FILE")

    if [ $? -eq 0 ]
    then
        echo "Policy $policies -> Trial $trials: Input $COUNT passed!\n" >> test.out
    else
        echo "Policy $policies -> Trial $trials: Input $COUNT failed!\n" >> test.out
    fi
done
done
done
clear