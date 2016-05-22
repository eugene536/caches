#!/usr/bin/env bash
mkdir build
cd build
cmake -Dtest=ON ../
make
./FixedCache
