#!/bin/bash

make clean
make ./target/generateGraph
make ./target/enumSDSystem

mkdir -p results
echo 'exptype,input,numCount,duration,isFinish' >> results/common_delay.csv
mkdir -p data

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

gs=(25 50 75 100 150 200 250 300 400 500 750 1000 2000 3000 4000)
ps=(0.01)
types=(0 1 2 3 4 5 6 7 8 9 10 11)

for g in ${gs[@]}; do
    for p in ${ps[@]}; do
        for type in ${types[@]}; do
            for ((t = 0; t < 3; t++ )); do
            echo "data/random-$g-$p-$t.grh"
                ./target/generateGraph $g $p $t "data/random-$g-$p-$t.grh"
                ./target/enumSDSystem "data/random-$g-$p-$t.grh" \
                    -tlim 600 \
                    -outname target/output.txt \
                    -common-outname results/common_delay.csv \
                    -type $type
            done
        done
    done
done