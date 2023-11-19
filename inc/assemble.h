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

#include <stdio.h>

#if defined(USE_ASM)
#define USE_ASM
#else
#define USE_ASM

#define         INSTRUCTION_EXEC(VALUE)                  \
static INSTRUCTION* TYPE ## VALUE(int, char*, char*, int*)

typedef struct ASSEMBLER
{
    FILE* INPUT_FILE;
    FILE* OUTPUT_FILE;
    FILE* LISTING_FILE;
    FILE* SYMBOL_FILE;
    char IDENTIFIER;

} ASSEMBLER;

typedef struct FILE_SEMANTIC
{
    char* MNEMONIC;
    char* INSTRUCTION_COUNT;
    char* WRITE_BUFFER;
    char* LINE_BUFFER[0];
    INSTRUCTION* INSTR_BASE;
    bool END_OF_FILE;

} FILE_SEMANTIC;

void ASSEMBLE_FILE(FILE_SEMANTIC* FILE_STATE, FILE* INPUT);
void ASSEMBLE_LINE(FILE_SEMANTIC* FILE_STATE, const char* SOURCE);

#endif
#endif
