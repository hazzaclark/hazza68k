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

#define         TEXT_SECTION        0x0001
#define         SIGNED_8BIT_MIN     (-128)
#define         SIGNED_8BIT_MAX     127
#define         SIGNED_16BIT_MIN    (-32768)
#define         SIGNED_16BIT_MAX    32767

#define DIGIT_VALUE(DIGIT) \
    (((DIGIT) >= '0' && (DIGIT) <= '9') ? ((DIGIT) - '0') : \
    ((DIGIT) >= 'a' && (DIGIT) <= 'f') ? ((DIGIT) - 'a' + 10) : \
    ((DIGIT) >= 'A' && (DIGIT) <= 'F') ? ((DIGIT) - 'A' + 10) : \
    (ERROR))

#define IS_REG_TYPE(t) ((t) & (DATA_REG | ADDRESS_REG))

#define SET_TOKEN(SYM, ID, TEXT, LEN) do { \
    (SYM)->ID = (ID); \
    (SYM)->TEXT = (TEXT); \
    (SYM)->LENGTH = (LEN); \
} while(0)

#define IS_LIST_CHAR(c) ((c) & (SLASH | MINUS))

typedef struct DIRECTIVE
{
    int ID;
    char* (*DIRECTIVE_ACTION)(int VALUE);

} DIRECTIVE;

typedef struct IDENTIFIER
{
    char* NAME;
    bool DEFINED;
    U16 VALUE;

    struct IDENTIFIER* BEFORE;
    struct IDENTIFIER* AFTER;

} IDENTIFIER;

typedef struct KEYWORD
{
    char* NAME;
    DIRECTIVES ID;

} KEYWORD;

typedef struct SYM_ID
{
    char SYMBOL;
    DIRECTIVES ID;

} SYM_ID;

IDENTIFIER* LOCATE_IDEN(char* VALUE);
DIRECTIVE* FIND_DIRECTIVE(int ID);
char* PROCESS_DIRECTIVE(INPUT* INPUT);
char* PROCESS_INSTRUCTION(INPUT* INPUT);
char* PROCESS_INPUT(int LINE, char* BUFFER);


int FIND_IDENTIFIER(char* LOOK);
DIRECTIVES FIND_KEYWORD(KEYWORD* KEY, char* FIND, int INDEX);
int COMPARE_WORD(char* CHECK, int LEN, char* FIXED);
int COMPARE_NUMBER(char* FROM, int BASE, unsigned* VALUE);

DIRECTIVES FIND_REGISTER(char* STRING, int LOOK, int* POS);
DIRECTIVES FIND_SYMBOL(char* FIND);

static inline int FIND_QUOTED(const char *FROM, char Q) 
{
    const char *PTR = FROM;
    int LEN = Q;
    while (*PTR && *PTR != QUOTE) 
    {
        if (*PTR == ESCAPE && *(PTR + 1)) 
        {
            LEN += 2; 
            PTR += 2;
        } 
        else 
        {
            LEN++;
            PTR++;
        }
    }
    return LEN;
}


#endif
#endif 
