
CC = g++
LD = g++

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
PROJ_DIR = $(dir (MKFILE_PATH))

BUILD_DIR = $(PROJ_DIR)build/
SRC_DIR = $(PROJ_DIR)src/


SRCS = $(wildcard $(PROJ_DIR)*.cpp)

OBJS = $(patsubst $(PROJ_DIR)%.cpp, $(BUILD_DIR)%.o, $(SRCS))


TARGET = hello

all: $(TARGET)

$(info $(OBJS))

$(BUILD_DIR)%.o: $(PROJ_DIR)%.cpp
	$(CC) -c -g $^ -o $@

$(TARGET): $(OBJS)
	$(LD) -o $@ $^


.PHONY: all clean
clean:
	$(RM) $(OBJS) $(TARGET)