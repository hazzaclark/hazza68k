/* COPYRIGHT (C) HARRY CLARK 2023 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE ASSEMBLY PROCESS */
/* FOCUSSING ON THE BACKEND OF THE ASSEMBLER */

#ifndef ASSEMBLE
#define ASSEMBLE

/* NESTED INCLUDES */

#include "common.h"
#include "disasm.h"

/* SYSTEM INCLUDES */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#if defined(USE_ASM)
#define USE_ASM
#else
#define USE_ASM



#define         DIRECTIVE_CHARS         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?_"
#define         LABEL_CHARS             DIRECTIVE_CHARS ".@"
#define         DIRECTIVE_MAX                   5
#define         DIRECTIVE_LENGTH_MAX            10

#define         PARAM_FALSE_IF                0
#define         PARAM_TOTAL                   1
#define         PARAM_SUFFIX                  ''

#define PRINT_SEMANTIC(STATE, COMMENT, ADDRESS) fprintf(stderr, COMMENT)
#define PRINT_INTERNAL(STATE) fputs(stderr, " ", STATE)

typedef struct OPCODE
{
    unsigned SIZE;
    unsigned OPCODE_NAME;

    EA_MODE ARGS;

} OPCODE;

typedef struct MNEOMONIC
{
    char* NAME;
    OPCODE* OPCODES;
    MNEOMONIC* BEFORE;
    MNEOMONIC* AFTER;

} MNEOMONIC;

typedef struct OPTIONS
{
    char* NAME;
    char* HELP;
    int SET;
    int RESET;

} OPTIONS;


OPCODE* FIND_OPCODE(char* MATCH, int LEN);
void INIT_OUTPUT(char* SOURCE);
void NEXT_LINE(int LINE, char* SOURCE);
void SET_ADDRESS(U32 ADDRESS);
void SET_START(U32 ADDRESS);
void ADD_BYTE(U8 DATA);
void END_OUTPUT(void);

int PARSE_ARGS(int argc, char** argv);

#endif
#endif
