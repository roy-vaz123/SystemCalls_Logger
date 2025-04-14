# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g

# Source and object files
SRCS = $(wildcard *.cpp)
TARGET = ../build/testing_program/test

# Default rule
all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p ../build/testing_program
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -rf ../build/testing_program
