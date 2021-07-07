#!/bin/bash
# clang formatting
#
echo "Start formatting source files.."
find . -not -path "./.*" -name "*.cpp" -o -name "*.c" -o -name "*.h" | xargs -I {} clang-format -i {}
echo "All done."
