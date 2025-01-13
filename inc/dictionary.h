/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE ASSEMBLY PROCESS */
/* FOCUSSING ON THE BACKEND OF THE ASSEMBLER */

#ifndef DICTIONARY
#define DICTIONARY

/* NESTED INCLUDES */

#include "common.h"
#include "disasm.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#if defined(USE_DICT)
#define USE_DICT
#else
#define USE_DICT

typedef struct DIRECTIVE
{
    int ID;
    char(*DIRECTIVE_ACTION)(int* VALUE);

} DIRECTIVE;

typedef struct IDENTIFIER
{
    char* NAME;
    bool DEFINED;
    U16 VALUE;

    struct IDENTIFIER* BEFORE;
    struct IDENTIFIER* AFTER;

} IDENTIFIER;


IDENTIFIER* LOCATE_IDEN(char* VALUE);
DIRECTIVE* FIND_DIRECTIVE(int ID);
char* PROCESS_DIRECTIVE(int* INPUT);
char* PROCESS_INSTRUCTION(int* INPUT);
char* PROCESS_INPUT(int LINE, char* BUFFER);

int FIND_IDENTIFIER(char* LOOK);

#endif
#endif 
