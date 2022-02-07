#!/bin/bash
# clang formatting
echo "Start formatting source files.."
find . -type d \( -path "./.*" -o -path "./datasheets" -o -path "./medis" \) -prune -false -o -name "*.h" -o -name ".hpp" -o -name "*.cpp" | xargs -I {} clang-format -i {}
echo "All done."
