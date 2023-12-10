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

typedef struct ASSEMBLER
{
    U32* GPR;
    ULONG* PC;
    char IDENTIFIER;
    FILE* INPUT_FILE;
    FILE* OUTPUT_FILE;
    FILE* LISTING_FILE;
    FILE* SYMBOL_FILE;
    U32* LISTING_COUNT;

} ASSEMBLER;

typedef struct FILE_SEMANTIC
{
    bool END_OF_FILE;
    char* MNEMONIC;
    char* INSTRUCTION_COUNT;
    char* WRITE_BUFFER;
    char* LINE_BUFFER[0];
    char* SOURCE_LINE;
    INSTRUCTION* INSTR_BASE;
    S32* LINE_CHAR;

} FILE_SEMANTIC;

typedef struct LINE_SEMNATIC
{
    

} LINE_SEMNATIC;

void ASSEMBLE_FILE(FILE_SEMANTIC* FILE_STATE, FILE* INPUT, ASSEMBLER* ASSEMBLER);
void ASSEMBLE_LINE(FILE_SEMANTIC* FILE_STATE, const char* SOURCE);

#endif
#endif
