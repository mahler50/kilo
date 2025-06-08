CFLAGS := -Wall -Wextra -pedantic -std=c99

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

TARGET := kilo

# Default
$(TARGET): $(OBJS)
	gcc $(OBJS) -o $(TARGET)

# Debug
debug: CFLAGS += -g -O0 -DDEBUG
debug: clean $(TARGET)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean debug
