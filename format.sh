#!/bin/bash

SRC_DIR="src"

find "$SRC_DIR" -type f \( -name "*.cpp" -o -name "*.h" \) -exec clang-format -i {} \;