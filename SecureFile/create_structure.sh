#!/bin/bash

# Create main folder BTech
mkdir BTech

# Change directory to BTech
cd BTech

# Create C file
touch main.c

# Create Python file
touch main.py

# Create two text files
touch file1.txt
touch file2.txt

# Create subfolders sem1 and sem2
mkdir sem1
mkdir sem2

# Change directory to sem1
cd sem1

# Create 10 files with different extensions
for i in {1..5}
do
    touch file${i}.txt
done

for i in {6..8}
do
    touch file${i}.py
done

for i in {9..10}
do
    touch file${i}.txt
done

# Change directory back to BTech
cd ..

# Change directory to sem2
cd sem2

# Create 12 files with different extensions
for i in {1..4}
do
    touch file${i}.txt
done

for i in {5..9}
do
    touch file${i}.py
done

for i in {10..12}
do
    touch file${i}.txt
done

# Change directory back to BTech
cd ..
