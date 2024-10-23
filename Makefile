# Makefile

# Compiler
CC = g++
# Compiler flags
CFLAGS = -Wall -Wextra -std=c++11
# Executable name
TARGET = priority

# Source files
SRCS = cpuScheduler.cpp

# Default target to build the executable
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean target to remove compiled files
clean:
	rm -f $(TARGET)

