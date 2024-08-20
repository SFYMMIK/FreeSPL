#!/bin/sh
# build.sh - Builds the FreeSPL project using Go and C

make -C ../src/c_core
go build -o freespl ../src/main.go
