#!/bin/bash
# clang formatting
echo "Start formatting source files.."
# find . -type d \( -path "./.*" -o -path "./datasheets" -o -path "./media" \) -prune -false -o -name "*.h" -o -name "*.hpp" -o -name "*.cpp"
# find . -type d \( -path "./.*" -o -path "./datasheets" -o -path "./media" \) -prune -false -o -name "*.h" -o -name "*.hpp" -o -name "*.cpp" | xargs  clang-format -i {}
find . -type d \( -path "./.*" -o -path "./datasheets" -o -path "./media" \) -prune -false -o -name "*.h" -o -name "*.hpp" -o -name "*.cpp" | xargs -I {} clang-format -i {}
echo "All done."
