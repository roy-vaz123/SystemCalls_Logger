# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
INCLUDES = -Iinclude

# Source and object files
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TARGET = ../build/syscall_logger

# Default rule
all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p ../build
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -rf ../build
