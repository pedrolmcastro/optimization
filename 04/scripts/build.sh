#!/usr/bin/env bash

cmake -DCMAKE_BUILD_TYPE=$1 . -B build
cmake --build build
