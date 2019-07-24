#!/bin/bash

find . -iname '*.[hc]pp' | grep -v '/build/' | entr ./make.sh
