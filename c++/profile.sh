#!/bin/bash

cd build
gprof AStar | gprof2dot -s | dot -Tpng > ../profile.png
