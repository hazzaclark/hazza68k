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

#define         INSTRUCTION_EXEC(VALUE)                  \
static INSTRUCTION* TYPE ## VALUE(int, char*, char*, int*)

#define         DIRECTIVE_CHARS         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?_"
#define         LABEL_CHARS             DIRECTIVE_CHARS ".@"


#define         PARAM_FALSE_IF                0
#define         PARAM_TOTAL                   1
#define         PARAM_SUFFIX                  ''

typedef struct ASSEMBLER
{
    U32* GPR;
    ULONG* PC;
    FILE* INPUT_FILE;
    FILE* OUTPUT_FILE;
    FILE* LISTING_FILE;
    FILE* SYMBOL_FILE;
    U32** LISTING_COUNT;
    char IDENTIFIER;

} ASSEMBLER;

typedef struct FILE_SEMANTIC
{
    bool END_OF_FILE;
    char* MNEMONIC;
    char* INSTRUCTION_COUNT;
    char* WRITE_BUFFER;
    char* LINE_BUFFER[0];
    char* SOURCE_LINE;
    struct INSTRUCTION* INSTR_BASE;
    S32* LINE_CHAR;
    UINT* CURRENT_BOOL_EXPR;

    FILE_MODE MODE;

} FILE_SEMANTIC;

typedef struct LINE_SEMNATIC
{
    struct LINE_SEMNATIC* NEXT_LINE;
    UINT HEAD;
    UINT TAIL:1;
    char* SOURCE;

} LINE_SEMNATIC;

typedef enum FILE_MODE
{
    MODE_NORMAL,
    MODE_REPEAT,
    MODE_MACRO,
    
} FILE_MODE;

typedef enum SYMBOL_MODE
{
    SYMBOL_CONST,
    SYMBOL_VAR,
    SYMBOL_MACRO,
    SYMBOL_LABEL,

} SYMBOL_MODE;

typedef struct DIRECTIVES
{
    char* KEY;
    UINT(*COMPARE_DIRECTIVES)(const char*, const char*, UNK*);
    UNK* DIRECTIVE_COUNT;

} DIRECTIVES;

typedef struct NODE_ENTRY
{
    unsigned TYPE;
    void* POINTER;

    struct NODE_ENTRY* NEXT;
    struct NODE_ENTRY* PREV;
    char* IDENTIFIER;
    UNK* IDENTIFIER_SIZE; 

} NODE_ENTRY;


void ASSEMBLE_FILE(FILE_SEMANTIC* FILE_STATE, FILE* INPUT, ASSEMBLER* ASSEMBLER);
void ASSEMBLE_LINE(FILE_SEMANTIC* FILE_STATE, char* SOURCE);
void PARSE_LINE(FILE_SEMANTIC* FILE_STATE, char* LINE, char* LABEL, char* POINTER);

void MACRO_TERMINATE(FILE_SEMANTIC* SEMANTIC);
void MACRO_TERMINATE_WHILE(FILE_SEMANTIC* SEMANTIC);
unsigned MACRO_BIT_SIZE(unsigned BIT_SIZE);
void OUT_OF_MEMORY(void);

#endif
#endif
