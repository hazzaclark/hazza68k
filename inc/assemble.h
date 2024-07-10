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
#define         DIRECTIVE_MAX                   5
#define         DIRECTIVE_LENGTH_MAX            10

#define         PARAM_FALSE_IF                0
#define         PARAM_TOTAL                   1
#define         PARAM_SUFFIX                  ''

#define PRINT_SEMANTIC(STATE, COMMENT, ADDRESS) fprintf(stderr,"%lu:\n", STATE, COMMENT, ADDRESS)
#define PRINT_INTERNAL(STATE) fputs(stderr, " ", STATE)

typedef struct ASSEMBLER
{
    U32* GPR;
    ULONG* PC;
    FILE* INPUT_FILE;
    FILE* OUTPUT_FILE;
    FILE* LISTING_FILE;
    FILE* SYMBOL_FILE;
    FILE* ERROR_FILE;
    U32** LISTING_COUNT;
    char* FILE_PATH;
    char IDENTIFIER;
    ULONG(*LINE_NUMBER);

} ASSEMBLER;

typedef struct FILE_SEMANTIC
{
    bool END_OF_FILE;
    char* MNEMONIC;
    char* INSTRUCTION_COUNT;
    char* WRITE_BUFFER;
    char LINE_BUFFER[1024];
    char* SOURCE_LINE;
    struct INSTRUCTION* INSTR_BASE;
    char LINE_CHAR;
    UINT* CURRENT_BOOL_EXPR;
    int FILE_MODE;

    UNK* OFFSET_POS;

    UINT CURRENT_IF;
    UINT FALSE_IF;
    const char* AFTER_IF;

    union FILE_SYMBOL
    {
        char SYMBOL;
        unsigned TYPE;
        ULONG* SHARED_VALUE;

    } FILE_SYMBOL;

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
    MODE_MACRO
    
} FILE_MODE;

typedef enum SYMBOL_MODE
{
    SYMBOL_CONST,
    SYMBOL_VAR,
    SYMBOL_MACRO,
    SYMBOL_LABEL

} SYMBOL_MODE;

typedef struct DIRECTIVES
{
    char* KEY;
    int(*COMPARE_TYPES);
    UNK* DIRECTIVE_COUNT;

} DIRECTIVES;

typedef struct NODE_ENTRY
{
    char* SOURCE_LINE;
    unsigned VALUE;
    unsigned TYPE;
    void* POINTER;

    struct NODE_ENTRY* NEXT;
    struct NODE_ENTRY* PREV;
    char* IDENTIFIER;
    UNK* IDENTIFIER_SIZE; 

} NODE_ENTRY;

typedef struct MACROS
{
    char* LINE_POINTER;
    char* MACRO_CHAR;
    char* MACRO_PARAM_START;
    char** MACRO_TOTAL_PARAMS;
    char* IDENTIFIER;

    UINT* PARAM_COUNT;


} MACROS;

typedef struct LOCATION
{
    void(*PREV);
    char* FILE_PATH;
    ULONG LINE_POS;

} LOCATION;

typedef struct TEXT
{
    void(*USER_DATA);
    void(*READ_CHAR)(void* USER_DATA, int CHAR);
    void(*READ_LINE)(void* USER_DATA, char* BUFFER, UNK BUFFER_SIZE);

} TEXT;
 
void ASSEMBLE_FILE(FILE_SEMANTIC* FILE_STATE, FILE* INPUT);
void ASSEMBLE_LINE(FILE_SEMANTIC* FILE_STATE, char* SOURCE);
void PARSE_LINE(FILE_SEMANTIC* FILE_STATE, char* LINE, char* LABEL, char* POINTER);

void MACRO_TERMINATE(FILE_SEMANTIC* SEMANTIC);
void MACRO_TERMINATE_WHILE(FILE_SEMANTIC* SEMANTIC);
unsigned MACRO_BIT_SIZE(unsigned BIT_SIZE);
void OUT_OF_MEMORY(void);

bool ASSEMBLE_FILE_CALLBACK
(
    FILE* INPUT_FILE,
    FILE* OUTPUT_FILE,
    FILE* LISTING_FILE,
    FILE* SYMBOL_FILE,
    char* INPUT_FILE_PATH,
    void USER_DATA(void)
);

bool ASSEMBLE_BASE
(
    TEXT* INPUT_CALLBACK,
    TEXT* OUTPUT_CALLBACK,
    TEXT* ERROR_CALLBACK,
    TEXT* LISTING_CALLBACK,
    TEXT* SYMBOL_CALLBACK,
    char* INPUT_FILE_PATH,
    void(*USER_DATA(void))

);

#endif
#endif
