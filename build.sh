#!/bin/bash

[ -d build ] || cmake -B build
cmake --build build
