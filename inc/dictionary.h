/* COPYRIGHT (C) HARRY CLARK 2023 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE FOCUSSES ON THE DICTONARY ENTIRES PERTAINING TOWARDS IDENTIFIERS */
/* AND DIRECTIVES IN RELATION TO 68K ASSEMBLY */

#ifndef DICTIONARY
#define DICTIONARY

/* NESTED INCLUDES */

#include "common.h"

/* SYSTEM INCLUDES */

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define     DICT_HASH_TABLE_ENTRIES         4096
#define     DICT_IDENTIFIER                 '   '

#if defined(USE_DICTIONARY)
#define USE_DICTIONARY
#else
#define USE_DICTIONARY

typedef struct DICTIONARY_ENTRY
{
    UINT TYPE;
    void(*OFFSET)(void);
    
    void* NEXT;
    void* PREVIOUS;
    void* PARENT;
    void* LEFT;
    void* RIGHT;

    bool ENTRY_SUCCESS;
    UNK IDENTIFIER_LENGTH;

    bool HASH_CASES;
    void(*HASH_TABLE);

} DICTIONARY_ENTRY;

UINT(*COMPARE_IDENTIFIERS)(void* LEFT_NODE, void* RIGHT_NODE, UNK COUNT);
DICTIONARY_ENTRY DICTIONARY_LOOKUP(struct DICTIONARY_ENTRY* ENTRY, const char* IDENTIFIER, UNK IDENTIFIER_LENGTH);


bool DICTIONARY_INIT(struct DICTIONARY_ENTRY* ENTRY);
void DICTIONARY_FREE(void);
bool DICTIONARY_REMOVE(struct DICTIONARY_ENTRY* ENTRY, const char* IDENTIFIER, UNK IDENTIFIER_LENGTH);
bool DICTIONARY_CREATE_DIRECTIVE(struct DICTIONARY_ENTRY** ENTRY, const char* IDENTIFIER, UNK IDENTIFIER_LENGTH);

#endif
#endif
