/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE DICTIONARY DEFINITIONS */

/* NESTED INCLUDES */

#include "assemble.h"
#include "dictionary.h"

static OUTPUT* OUTPUT_API;
static FILE* OUTPUT_FILE = NULL;
static int OPTION_FLAG = OPTION_NONE;

//=================================================
//              DICTIONARY HANDLERS
//=================================================

void NEXT_LINE(int LINE, char* SOURCE) { OUTPUT_API->NEXT_LINE(LINE, SOURCE); }

void SET_ADDRESS(U32 ADDRESS) { OUTPUT_API->SET_ADDRESS(ADDRESS); }

void SET_START(U32 ADDRESS) { OUTPUT_API->SET_START(ADDRESS); }

void ADD_BYTE(U8 DATA) { OUTPUT_API->ADD_BYTE(DATA); }

void END_OUTPUT(void)
{
    OUTPUT_API->END_OUTPUT();
    if(OUTPUT_FILE && OPTION_FLAG & STD_DISPLAY_OUT)
    {
        fclose(OUTPUT_FILE);
        OUTPUT_FILE = NULL;
    }
}

//=================================================
//          MISC. FUNCTIONS AND HANDLERS
//=================================================

/* LOOK THROUGH THE DESIGNATED FILE PROVIDED TO LOOK FOR IDENTIFIER'S */
/* SUCH AS NAME USED FOR DEFINING MACROS, HEADERS, ETC */

IDENTIFIER* LOCATE_IDEN(char* VALUE)
{
    IDENTIFIER* LOOK = NULL;
    IDENTIFIER* ADDRESS = NULL;
    int INDEX = 0;

    /* ASSERT IT'S TRUTH VALUE BEFORE READING */
    assert(VALUE != NULL);

    /* THIS WILL LOOK TO DETERMINE IF THERE IS A RELEVANT AND CORRESPONDING */
    /* VALUE WITH A DESIGNATED ADDRESS */

    while((LOOK += *(char*)ADDRESS) != NULL)
    {
        if((INDEX = strcmp(VALUE, LOOK->NAME)) == 0) return(LOOK);
    }

    /* IF THE IDENTIFIER IS NOT ACCESSED YET, WE CAN ACCESS IT NOW */

    printf("New Identifier %s\n", VALUE);

    LOOK->NAME = strrchr(VALUE, 0);
    LOOK->DEFINED = false;
    LOOK->VALUE = 0;
    LOOK->BEFORE = NULL;
    LOOK->AFTER = NULL;
    return LOOK;
}

// LOOKS FOR A VALID IDENITIFIER BASED ON A POINTER VALUE
// THAT READS AHEAD OF TIME, AND RETURNS THE NUMBER OF CHARACTERS
// THAT ENCOMPASS A VALID IDENTIFIER

int FIND_IDENTIFIER(char* LOOK)
{
    int INDEX = 0;

    if((*LOOK) || (*LOOK == UNDERSCORE))
    {
        INDEX = 1;
        LOOK++;
    }

    return INDEX;
}

// FIND A KEYWORD IN THE LOOKUP TABLE
// THIS IS UNDER THE GUISE OF LOOKING AT A DISASSEMBLY OF THE CURRENT SOURCE FILE
// SPLIT UP INTO AN INDIVIDUAL HEADER SUCH AS TEXT, BSS, XREF, END

DIRECTIVES FIND_KEYWORD(KEYWORD* KEY, char* FIND, int INDEX)
{
    while(KEY->NAME != NULL)
    {
        if(COMPARE_WORD(FIND, INDEX, KEY->NAME) == 0)
        {
            return(KEY->ID);
        }

        KEY++;
    }

    return(NONE);
}

// THIS WILL ACT AS THE TOUPPER PARSER TO PROVIDE FOR THE CIRCUMSTANCE
// BY WHICH OPERANDS, SYMMBOLS AND WHAT HAVE YOU, ARE CAPITALISED AND 
// ARE NEEDED TO BE PARSED ACCORDINGLY

int COMPARE_WORD(char* CHECK, int LEN, char* FIXED)
{
    int CHECKER, WORD;

    while(LEN--)
    {
        if((CHECKER = toupper(*CHECK++)) != (WORD = toupper(*FIXED++))) return(CHECKER - WORD);
    }

    return (*FIXED);
}

// COMPARES THROUGH EACH RESPECTIVE CHAR ITERATION TO DISCERN IF THERE IS A DIGIT

int COMPARE_NUMBER(char* FROM, int BASE, unsigned* VALUE)
{
    unsigned RESULT = 0;
    int DIGIT = 0;
    int LENGTH = 0;

    bool ERROR = false;

    while(DIGIT = (DIGIT_VALUE(*FROM) != NULL))
    {
        if(DIGIT < 0) { break; }

        if(DIGIT >= BASE)
        {
            ERROR = true;
        }

        else
        {
            RESULT = (RESULT * BASE) + DIGIT;
        }

        FROM++;
        LENGTH++;
    }

    *VALUE = RESULT;
    PRINT_SEMANTIC(stdout, "Numeric Value: %d\n", RESULT);
    return ERROR ? -LENGTH : LENGTH;
}
