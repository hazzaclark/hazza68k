## COPYRIGHT (C) HARRY CLARK 2023

## MOTOROLA 68000 ASSEMBLER
  
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wparentheses -Werror -pedantic

SRCS = main.c assemble.c dictionary.c macro.c opcode.c
OBJS = $(SRCS:.c=.o)

TARGET = hazza68k

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
