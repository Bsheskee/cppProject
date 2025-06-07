# Student Grade Management System

A simple C++ program for managing student grades and calculating averages.

## Features
- Read student data from file
- Calculate individual and overall averages
- Determine final grades based on averages
- Display students above average

## Requirements
- C++ compiler with C++11 support
- Input file `dane.txt` with student data

## Compilation & Running

### Using Makefile (Recommended)
```bash
make
./program
```

### Manual compilation
```bash
g++ -std=c++11 -Wall -o program main.cpp
./program
```

### Windows
```cmd
g++ -std=c++11 -Wall -o program.exe main.cpp
program.exe
```

## Input File Format
The program expects a file named `dane.txt` with the following format:
```
Student Name
grade1 grade2 grade3 grade4 grade5 grade6
Next Student Name
grade1 grade2 grade3 grade4 grade5 grade6
```

## Clean Build
```bash
make clean
```

