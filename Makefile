## COPYRIGHT (C) HARRY CLARK 2024
## MOTOROLA 68000 ASSEMBLER

#########################################################################
##                              PREFIXES                               ##
#########################################################################

CC              =       $(PREFIX)gcc
WARNINGS        =       -std=c99 -Wall -Wextra -Wparentheses -Werror -pedantic
SRC             =       src
INC             =       inc

#########################################################################
##                           FILE ACCESS                               ##
#########################################################################

68K_FILES       =       $(wildcard $(SRC)/*.c)
68K_OBJS        =       $(68K_FILES:$(SRC)/%.c=$(SRC)/%.o)

#########################################################################
##                        TARGET LINKING                               ##
#########################################################################

## ADJUST THE FILE EXTENSION TO SUIT WHICHEVER FILE EXT YOU NEED
## CONSULT THE README FOR DIRECTIONS

TARGET          =       hazza68k

all: $(TARGET)

$(TARGET): $(68K_OBJS)
	$(CC) $(WARNINGS) -I$(INC) -o $@ $^

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(WARNINGS) -I$(INC) -c $< -o $@

#########################################################################
##                              CLEANUP                                ##
#########################################################################

clean:
	rm -f $(SRC)/*.o $(TARGET)

.PHONY: clean
