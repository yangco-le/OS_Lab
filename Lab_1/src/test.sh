#!/bin/bash

for i in 1 2 3 4 5
do
    ./NextFit < "traces/trace$i" > "out/out$i"
done