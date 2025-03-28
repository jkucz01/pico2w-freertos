#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: ./build.sh <exec-name>"
    exit 1
fi

EXEC_NAME=$1
BUILD_DIR="build/$EXEC_NAME"

cmake -S . -B "$BUILD_DIR" -DEXEC_NAME="$EXEC_NAME"

cmake --build "$BUILD_DIR" --clean-first