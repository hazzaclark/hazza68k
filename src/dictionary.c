/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE DICTIONARY DEFINITIONS */

/* NESTED INCLUDES */

#include "assemble.h"

static OUTPUT* OUTPUT_API;
static FILE* OUTPUT_FILE = NULL;
static int OPTION_FLAG = OPTION_NONE;

//=================================================
//              DICTIONARY HANDLERS
//=================================================

void NEXT_LINE(int LINE, char* SOURCE)
{
    OUTPUT_API->NEXT_LINE(LINE, SOURCE);
}

void SET_ADDRESS(U32 ADDRESS)
{
    OUTPUT_API->SET_ADDRESS(ADDRESS);
}

void SET_START(U32 ADDRESS)
{
    OUTPUT_API->SET_START(ADDRESS);
}

void ADD_BYTE(U8 DATA)
{
    OUTPUT_API->ADD_BYTE(DATA);
}

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

    while((LOOK = &ADDRESS) != NULL)
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
    ADDRESS == LOOK;
    return LOOK;
}
