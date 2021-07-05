#!/bin/bash
# clang formatting
#
find . -name "*.cpp" -o -name "*.c" -o -name "*.h" | grep '\./include' | xargs -I {} clang-format -style=file -i {} 
find . -name "*.cpp" -o -name "*.c" -o -name "*.h" | grep '\./interfaces' | xargs -I {} clang-format -style=file -i {} 
find . -name "*.cpp" -o -name "*.c" -o -name "*.h" | grep '\./lib' | xargs -I {} clang-format -style=file -i {} 
find . -name "*.cpp" -o -name "*.c" -o -name "*.h" | grep '\./src' | xargs -I {} clang-format -style=file -i {} 
find . -name "*.cpp" -o -name "*.c" -o -name "*.h" | grep '\./text' | xargs -I {} clang-format -style=file -i {}
echo 'fin.'

