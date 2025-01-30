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

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#if defined(USE_ASM)
#define USE_ASM
#else
#define USE_ASM

#define         MAX_OUTPUT_BUFFER       16

#define         DIRECTIVE_CHARS         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?_"
#define         LABEL_CHARS             DIRECTIVE_CHARS ".@"
#define         DIRECTIVE_MAX                   5
#define         DIRECTIVE_LENGTH_MAX            10

#define         PARAM_FALSE_IF                0
#define         PARAM_TOTAL                   1
#define         PARAM_SUFFIX                  ''

#define         PARAM_EOS                   '\0'
#define         PARAM_INSTR_ARGS            2
#define         PARAM_DIR_ARGS              8


#define         PRINT_SEMANTIC(STATE, COMMENT, ADDRESS) fprintf(stderr, COMMENT)
#define		PRINT_REGISTER(STATE, COMMENT, ADDRESS) fprintf(stdout, COMMENT, ADDRESS)
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

#define         STD_DISPLAY_NA                  -1

//=================================================
//=================================================

typedef struct OPCODE
{
    unsigned SIZE;
    char* OPCODE_NAME;

    EA_MODE ARGS;

    U16 BASE_CODE, EXT_CODE;
    unsigned BIT_SIZE;
    unsigned OFFSET;

    struct OPCODE* NEXT;

} OPCODE;

typedef struct MNEOMONIC
{
    char* NAME;
    struct OPCODE* OPCODES;
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

typedef struct INPUT
{
    struct DIRECTIVE_SYM* LABEL, *ACTION;
    UNK SIZE;

    int INSTR;
    int ARGS;

} INPUT;

typedef struct OUTPUT
{
    void(*INIT_OUTPUT)(char* SOURCE);
    void(*NEXT_LINE)(int LINE, char* SOURCE);
    void(*SET_ADDRESS)(U32 ADDRESS);
    void(*SET_START)(U32 ADDRESS);
    void(*ADD_BYTE)(U8 DATA);
    void(*END_OUTPUT)(void);

} OUTPUT;

typedef struct DIRECTIVE_SYM
{
    DIRECTIVES ID;
    char* TEXT;
    int LENGTH;

    S16 REG_NUM;
    unsigned CONTAINS;
    unsigned VALUE;

    OPCODE* OP;
    char* ERROR;
    struct DIRECTIVE_SYM* EXPR;
    struct DIRECTIVE_SYM* NEXT;

} DIRECTIVE_SYM;

typedef struct PARSED
{
    struct DIRECTIVE_SYM* SYM;
    int SIZE;
    struct DIRECTIVE_SYM* ACTION;
    struct DIRECTIVE_SYM* LABEL;

    bool IS_INSTR;

    union
    {
        struct { DIRECTIVE_SYM* ARGS[PARAM_INSTR_ARGS]; int COUNT; } INSTR;
        struct { DIRECTIVE_SYM* ARGS[PARAM_DIR_ARGS]; int COUNT; } DIR;
        
    } ARGS;

} PARSED;

typedef enum
{
    NOT_ASM,
    DATA_PARSE,
    DATA_VERIF,
    CODE_GEN

} PASS_MODE;

extern PASS_MODE ASSEMBLER_PASS = NOT_ASM;

//=================================================
//=================================================

OPCODE* FIND_OPCODE(char* MATCH, int LEN);
int PASS_FILE(FILE* SOURCE);
char* PROC_INPUT(char* BUFFER);
int NEXT_SYM(char** PTR, DIRECTIVE_SYM* SYM);

void NEXT_LINE(int LINE, char* SOURCE);
void SET_ADDRESS(U32 ADDRESS);
void SET_START(U32 ADDRESS);
void ADD_BYTE(U8 DATA);
void END_OUTPUT(void);

#endif
#endif
