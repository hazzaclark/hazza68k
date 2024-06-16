## COPYRIGHT (C) HARRY CLARK 2023

## MOTOROLA 68000 ASSEMBLER
  
CC = gcc
CFLAGS = -std=c90 -Wall -Wextra -Wparentheses -Werror 

SRCS = main.c assemble.c dictionary.c macro.c
OBJS = $(SRCS:.c=.o)

TARGET = mdemu

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
