
CC = g++
LD = g++

SRCS = $(wildcard *.cpp)

OBJS = $(patsubst %.cpp, %.o, $(SRCS))


TARGET = hello

all: $(TARGET)


%.o: %.cpp
	$(CC) -c -g $^

$(TARGET): $(OBJS)
	$(LD) -o $@ $^


.PHONY: all clean
clean:
	$(RM) $(OBJS) $(TARGET)