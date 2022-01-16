#!/bin/sh

set -xe

CC=g++
CFLAGS="-Wall -Wextra"

$CC $CFLAGS main.cpp Item.cpp -o output -std=c++20
