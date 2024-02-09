#!/bin/bash

# make clean
make target/enumSDSystem

file="input/random-10-0.3-1.grh"
# file="data/random-10-0.1-1.grh"

# python3 analyze/generate-random-graph.py -n 10 -p 0.1 -s 1 -o input/random-10-0.1-1.grh
# types=(0 1 2 3 4 5 6 7 8 9 10 11)
# for type in ${types[@]}; do
#     ./target/enumSDSystem $file \
#         -tlim 120 \
#         -outname target/debug.txt \
#         -common-outname target/debug.common.csv \
#         -type $type
# done
# ./target/enumSDSystem $file \
#     -tlim 120 \
#     -outname target/debug.txt \
#     -common-outname target/debug.common.csv \
#     -type 9
./target/enumSDSystem $file \
    -tlim 120 \
    -outname target/debug.txt \
    -common-outname target/debug.common.csv \
    -type 12

# python3 analyze/generate-random-graph.py -n 20 -p 0.3 -s 10 -o input/random_20_3_10.grh
# python3 analyze/generate-random-graph.py -n 13 -p 0.2 -s 10 -o input/random_13_2_10.grh

# ./target/enumSDSystem input/simple01.grh \
# ./target/enumSDSystem input/test.grh \
# ./target/enumSDSystem input/random_3000_3_10.grh \
# ./target/enumSDSystem input/random_13_2_10.grh \
# ./target/enumSDSystem input/two-edge.grh \
# ./target/enumSDSystem input/random_20_3_10.grh \
#     -tlim 3600 \
#     -outname target/output.txt \
#     -common-outname results/common.csv \
#     -type 5

# gs=(10 15 20 30 50 100 200)
# ps=(0.1 0.3)
# types=(0 1 2 3 4 5)

# for g in ${gs[@]}; do
#     for p in ${ps[@]}; do
#         for type in ${types[@]}; do
#             for ((t = 0; t < 3; t++ )); do
#             echo "input/random-$g-$p-$t.grh"
#                 python3 analyze/generate-random-graph.py -n $g -p $p -s $t -o "input/random-$g-$p-$t.grh"
#                 ./target/enumSDSystem "input/random-$g-$p-$t.grh" \
#                     -tlim 1800 \
#                     -outname target/output.txt \
#                     -common-outname results/common.csv \
#                     -type $type
#             done
#         done
#     done
# done