#!/bin/bash

# Create 5 text files
for i in {1..5}; do
    echo "This is text file $i" > "text_file_$i.txt"
done

# Create 6 C files
for i in {1..6}; do
    echo "#include <stdio.h>" > "source_file_$i.c"
    echo "int main() {" >> "source_file_$i.c"
    echo "    printf(\"This is source file $i\");" >> "source_file_$i.c"
    echo "    return 0;" >> "source_file_$i.c"
    echo "}" >> "source_file_$i.c"
done
