/* COPYRIGHT (C) HARRY CLARK 2024 */

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

#define         PRINT_SEMANTIC(STATE, COMMENT, ADDRESS) fprintf(stderr, COMMENT)
#define         PRINT_INTERNAL(STATE) fputs(stderr, " ", STATE)

#define         FLAG_68000                  00000
#define         FLAG_68010                  00010
#define         FLAG_68020                  00020
#define         FLAG_68030                  00030
#define         FLAG_68040                  00040

#define         OPTION_68000                1
#define         OPTION_68010                2
#define         OPTION_68020                3
#define         OPTION_68030                4
#define         OPTION_68040                5
#define         OPTION_NONE                 0

#define         M68K_OPTION_TARGET      TARGET_CPU

#define         STD_DISPLAY_TEXT                0
#define         STD_DISPLAY_OUT                 1
#define         STD_DISPLAY_HELP                2

//=================================================
//=================================================

typedef struct OPCODE
{
    unsigned SIZE;
    unsigned OPCODE_NAME;

    EA_MODE ARGS;

} OPCODE;

typedef struct MNEOMONIC
{
    char NAME;
    struct OPCODE OPCODES;
    struct MNEOMONIC* BEFORE;
    struct MNEOMONIC* AFTER;

} MNEOMONIC;

typedef struct OPTIONS
{
    const char* NAME;
    const char* HELP;
    int SET;
    int RESET;

} OPTIONS;

//=================================================
//=================================================

OPCODE* FIND_OPCODE(char* MATCH, int LEN);
void INIT_OUTPUT(char* SOURCE);
void NEXT_LINE(int LINE, char* SOURCE);
void SET_ADDRESS(U32 ADDRESS);
void SET_START(U32 ADDRESS);
void ADD_BYTE(U8 DATA);
void END_OUTPUT(void);

//=================================================
//=================================================

OPTIONS* FIND_OPTION(const char* VALUE);
int PARSE_ARGS(int argc, char** argv);
void DISPLAY_HELP(const char* NAME);
int HANDLE_OPTION(const char* ARG);

#endif
#endif
