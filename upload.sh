#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: ./upload.sh <exec-name>"
    exit 1
fi

EXEC_NAME=$1
BUILD_DIR="build/$EXEC_NAME"

picotool load "$BUILD_DIR/$EXEC_NAME.uf2"

picotool reboot