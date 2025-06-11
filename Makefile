# Simple Makefile for C++ project
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = program
SOURCE = main.cpp

# Default target
all: $(TARGET)

# Build the program
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Clean build files
clean:
	rm -f $(TARGET) *.o

# Force rebuild
rebuild: clean all

.PHONY: all clean rebuild
