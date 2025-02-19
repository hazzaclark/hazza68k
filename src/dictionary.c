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

    if((OUTPUT_FILE) && (!(OPTION_FLAG & STD_DISPLAY_OUT)))
    {
        fclose(OUTPUT_FILE);
        OUTPUT_FILE = NULL;
    } 
}

static SYM_ID SYM_IDS[] =
{
    {PLUS,              SYM_PLUS},
    {MINUS,             SYM_MINUS},
    {ASTERIX,           MUL},
    {SLASH,             DIV},
    {PERCENT,           MOD},
    {OPAREN,            SYM_OPAREN},
    {CPAREN,            SYM_CPAREN},
    {COLON,             SYM_COLON},
    {SEMICOLON,         SYM_SEMICOLON},
    {HASH,              SYM_HASH},
    {COMMA,             SYM_COMMA},
    {HAT,               EOR},
    {BANG,              NOT},
    {AMPERSAND,         AND},
    {PIPE,               OR},
    {PARAM_EOS             }
};

static DIRECTIVE DIR_LIST[] = 
{
    {TEXT,              NULL},
    {DATA,              NULL},
    {BSS,               NULL},
    {ORG,               NULL},
    {START,             NULL},
    {ALIGN,             NULL},
    {EQU,               NULL},
    {END,               NULL},
    {DC,                NULL},
    {DS,                NULL},
    {NONE}
};

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

DIRECTIVE* FIND_DIRECTIVE(int ID)
{
    DIRECTIVE* LOOK;

    for(LOOK = DIR_LIST; LOOK->ID != NONE; LOOK++)
    {
        if(LOOK->ID == ID) { return LOOK; }
    }

    return NULL;
}

SCOPE SECTION_SCOPE(void)
{
    return (SECTION_TO_SCOPE[CURRENT_SECTION->SECTION_BLOCK] | SCOPE_ADDRESS);
}

char* PROCESS_DIRECTIVE(INPUT* INPUT)
{
    DIRECTIVE* DIR;

    if((DIR = FIND_DIRECTIVE((int)INPUT->LABEL)) == NULL) 
        return "Directive not implemented";
    
    return DIR->DIRECTIVE_ACTION((int)INPUT);
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

int FIND_KEYWORD(KEYWORD* KEYWORDS, char* STRING, int LEN) {
    for (int i = 0; KEYWORDS[i].NAME != NULL; i++) {
        if (strncmp(KEYWORDS[i].NAME, STRING, LEN) == 0) {
            return KEYWORDS[i].ID;
        }
    }
    return NONE;
}

// THIS WILL ACT AS THE TOUPPER PARSER TO PROVIDE FOR THE CIRCUMSTANCE
// BY WHICH OPERANDS, SYMMBOLS AND WHAT HAVE YOU, ARE CAPITALISED AND 
// ARE NEEDED TO BE PARSED ACCORDINGLY

int COMPARE_WORD(char* FIND, int INDEX, char* NAME) {
    // Compare the first INDEX characters of FIND and NAME
    if (strncmp(FIND, NAME, INDEX) == 0 && NAME[INDEX] == '\0') {
        return 0; // Return 0 if they match
    }
    return 1; // Return 1 if they don't match
}

// COMPARES THROUGH EACH RESPECTIVE CHAR ITERATION TO DISCERN IF THERE IS A DIGIT

int COMPARE_NUMBER(char* FROM, int BASE, unsigned* VALUE)
{
    int RESULT = 0;
    int DIGIT = 0;
    int LENGTH = 0;

    bool ERROR = false;

    while((DIGIT = DIGIT_VALUE(*FROM) >= 0))
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
    return ERROR ? -LENGTH : LENGTH;
}

DIRECTIVES FIND_SYMBOL(char* FIND)
{
    SYM_ID* LOOK;

    for(LOOK = SYM_IDS; LOOK->SYMBOL != PARAM_EOS; LOOK++)
    {
        if(*FIND == LOOK->SYMBOL) 
	{ 
		PRINT_REGISTER(stdout, "Found Symbol: '%c\n", LOOK->SYMBOL); return LOOK->ID; 
	}
    }

    return NONE;
}

// FIND WHAT LOOKS TO BE A REGISTER DECLARATIVE WITHIN THE DIRECTIVE DEFINTION

DIRECTIVES FIND_REGISTER(char* STRING, int LOOK, int* POS)
{
    int RESULT = 0;

    switch (tolower(LOOK))
    {
        case 'a':
		PRINT_REGISTER(stdout, "Address Register 'A%d'\n", RESULT);
		*POS = RESULT;
		return ADDRESS_REG;

        case 'd':
		PRINT_REGISTER(stdout, "Data Register: 'D%d'\n", RESULT);
            	*POS = RESULT;
            	return DATA_REG;

        case 'f':
            if (STRING[1] == 'p' && strlen(STRING) == 3)
            {
                RESULT = DIGIT_VALUE(STRING[2]);
                if (RESULT >= 0 && RESULT <= 7)
                {
			            PRINT_REGISTER(stdout, "FPU Register: 'FP%d'\n", RESULT);
                    	*POS = RESULT;
                    	return FPU_REG;
                }
            }
            break;

        default:
            break;
    }

    if (strncmp(STRING, "sp", 2) == 0)
    {
        PRINT_SEMANTIC(stdout, "SP Register 'A7'\n", RESULT);
        *POS = 7;
        return ADDRESS_REG;
    }

    if (strncmp(STRING, "fp", 2) == 0 && strlen(STRING) == 2)
    {
        PRINT_SEMANTIC(stdout, "FP Register 'A6'\n", RESULT);
        *POS = 6;
        return ADDRESS_REG;
    }

    return NONE;
}

char* PROCESS_INSTRUCTION(INPUT* INPUT)
{
    char* ERROR;
    OPCODE* LOOK;

    if (INPUT->LABEL != NULL) 
    {
        ERROR = INPUT->LABEL->TEXT;
        if (INPUT->LABEL->LENGTH > 0) 
        {
            return ERROR;
        }
    }

    for(LOOK = INPUT->ACTION->OP; LOOK != NULL; LOOK = LOOK->NEXT)
    {
        int* SIZE = 0;

        switch (LOOK->SIZE)
        {
            case SIZE_UNDEF: SIZE += (INPUT->SIZE == SIZE_UNDEF); break;
            case SIZE_BYTE: SIZE += (INPUT->SIZE == SIZE_UNDEF || INPUT->SIZE == SIZE_BYTE); break;
            case SIZE_WORD: SIZE += (INPUT->SIZE == SIZE_UNDEF || INPUT->SIZE == SIZE_WORD); break;
            case SIZE_LONG: SIZE += (INPUT->SIZE == SIZE_UNDEF || INPUT->SIZE == SIZE_LONG); break; 
            
        
            default:
                if((INPUT->SIZE == SIZE_UNDEF) && (LOOK->SIZE & SIZE_WORD))
                {
                    INPUT->SIZE = SIZE_WORD;
                }

                else
                {
                    SIZE += ((LOOK->SIZE & INPUT->SIZE) != 0);
                }

                break;
        }
    } 

    // HAVE WE ENCOUNTERED AN ERROR AT ALL?

    if(LOOK == NULL) return( "Invalid Instruction" );
    return NULL;  
}
