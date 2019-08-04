#!/bin/bash

for i in **/*.cpp **/**/*.cpp; do iwyu -Isrc/ --std=c++17 $i; done
