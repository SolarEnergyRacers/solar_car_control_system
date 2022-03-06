#!/bin/bash
# clang formatting
#
echo "Start formatting source files.."
# find . -not -path "./.*" -name "*.cpp" -o -name "*.c" -o -name "*.h" | xargs -I {} clang-format -i {}
# find . -type d \( -path "./.*" -o -path "./datasheets" -o -path "./medis" \) -prune -false -o -name "*.h" -o -name "*.hpp" -o -name "*.cpp" | xargs clang-format -i {}
find . -type d \( -path "./.*" -o -path "./datasheets" -o -path "./medis" \) -prune -false -o -name "*.h" -o -name ".hpp" -o -name "*.cpp" | xargs -I {} clang-format -i {}
echo "All done."
