#!/bin/bash

mkdir build -p

cd build && \
echo "Running CMake..." && cmake ../ && \
echo "Running make..." && make && \
echo "Running program..." && ./AStar \
&& echo "Done :)" || echo "Failure :("
