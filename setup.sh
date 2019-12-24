#!/bin/bash

rm -rf build.tools
mkdir build.tools && cd build.tools
cmake .. -DENABLE_FUZZING=Off -DCMAKE_C_COMPILER=clang
cd ..

rm -rf build.fuzz
mkdir build.fuzz && cd build.fuzz
cmake .. -DENABLE_FUZZING=On -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cd ..

rm -rf build.cov
mkdir build.cov && cd build.cov
cmake .. -DENABLE_FUZZING=On -DCOVERAGE=On -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cd ..
