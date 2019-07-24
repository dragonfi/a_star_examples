#!/bin/bash

python3 -m cProfile -o profile.stats benchmark.py
gprof2dot -f pstats profile.stats | dot -Tpng -o profile.png

