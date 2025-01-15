/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE ASSEMBLY PROCESS */
/* FOCUSSING ON THE BACKEND OF THE ASSEMBLER */

/* NESTED INCLUDES */

#include "assemble.h"
#include "dictionary.h"

#undef USE_DISASM

static MNEOMONIC* MNEOMONIC_BASE = NULL;

KEYWORD KEYWORD_BIT[] = 
{
    { "b",              BYTE        },
    { "w",              WORD        },
    { "l",              LONG        },
    { NULL }
};

KEYWORD KEYWORDS[] = 
{
    { "text",           TEXT        },
    { "data",           DATA        },
    { "bss",            BSS         },
	{ "org",            ORG         },
	{ "start",          START       },
	{ "align",          ALIGN       },
	{ "equ",            EQU         },
	{ "end",            END         },
	{ "dc",             DC          },
	{ "ds",             DS          },

	{ "pc",             PC          },
	{ "sr",             SR          },
	{ "ccr",            CCR         },
	{ "usp",            USP         },
	{ "vbr",            VBR         },
	{ "sfc",            SFC         },
	{ "dfc",            DFC         },
	{ NULL }
};

/*=================================================
          OPCODE AND OPTION HANDLERS
=================================================*/

// FIND AND PARSE THE DESIGNATED OPCODES PROVIDED THROUGH THE SOURCE FILE
// THIS CAN BE DONE USING THE MNEMONIC STRUCT TO LOOK FOR THE SPECIFIC KEY VALUE
// ASSOCIATED WITH EACH

OPCODE* FIND_OPCODE(char* MATCH, int LEN)
{
    int COMPARE = 0;

    COMPARE = strncmp(MATCH, MNEOMONIC_BASE->NAME, LEN);
    
    if (COMPARE == 0) 
    {
        printf("Found Opcode: %s\n", MNEOMONIC_BASE->NAME);
        return MNEOMONIC_BASE->OPCODES; 
    }

    MNEOMONIC_BASE = (COMPARE < 0) ? MNEOMONIC_BASE->BEFORE : MNEOMONIC_BASE->AFTER;
    return NULL;
}

int PASS_FILE(FILE* SOURCE)
{
    char BUFFER[MAX_BIT_ARGS];

    int LINE = 0;

    /* ASSUME THERE IS A CURRENT FILE BEING PASSED THROUGH */
    /* EVALUATE LENGTH FROM THE START TO THE END */

    while(fgets(BUFFER, MAX_BIT_ARGS, SOURCE))
    {
        BUFFER[strlen(BUFFER) - 1] = EOF;
        LINE++;

        /* CHECK THROUGH EACH CORRESPONDING LINE */

        NEXT_LINE(LINE, BUFFER);
    }

    return 0;
}

/* PROCESS THE INPUT PROVIDED FROM THE INPUT LINE */
/* THE FOLLOWING WILL BREAK DOWN THE CORRESPONDENCE INTO SMALLER SECTIONS */
/* IN ORDER TO PARSE EXPRESSIONS AND CALLS THE RELEVANT ROUTINE TO HANDLE SAID INPUT */

char* PROC_INPUT(int LINE, char* BUFFER)
{   
    DIRECTIVE_SYM* HEAD, **TAIL, *SYM, *TOP;
    int LEN = 0;

    TAIL = &HEAD;
    SYM = (DIRECTIVE_SYM*)malloc(sizeof(DIRECTIVE_SYM));
    
}

/* FIND THE NEXT AVAILABLE SYMBOL BASED OFF OF THE POINTER REFERENCE */
/* ADDS TO THE SYMBOL RECORD TO VALIDATE WHICH SYMBOLS ARE BEING PASSED THROUGH */
/* FROM THE SOURCE FILE */

int NEXT_SYM(char** PTR, DIRECTIVE_SYM* SYM)
{
    char* STRING;
    DIRECTIVES SYM_INDEX;

    int INDEX = 0;

    // INIT SETUP FOR SYMBOL LOOKUP
    memset(SYM, 0, sizeof(DIRECTIVE_SYM));
    STRING = *PTR;

    // DETERMINE THE END OF A STRING LITERAL OR EOF

    while(isspace(*STRING)) STRING++; 

    // IS THE POINTER AT THE END OF THE STRING?

    if(*STRING == PARAM_EOS) { *PTR = STRING; } return false;

    // NOW PARSE EACH OF THE CORRESPONDING CHAR DIRECTIVES THAT COULD BE FOUND WITHIN THE SOURCE FILE
    // THIS RANGES FROM ANY AND ALL SORTS OF ASCII LEXICALS

    switch (*STRING)
    {
        case PERIOD:
            INDEX = FIND_IDENTIFIER(STRING + 1);
            
            if(INDEX > 0 && FIND_KEYWORD(KEYWORD_BIT, STRING + 1, INDEX) != NONE)
            {
                SYM->TEXT = STRING;
                SYM->LENGTH = INDEX + 1;
            }

            else
            {
                SYM->ID = ERROR;
                SYM->TEXT = STRING;
                SYM->LENGTH = INDEX + 1;
                SYM->ERROR = "Unrecognised Keyword";
                *PTR = STRING = SYM->LENGTH;
            }

            return true;

        case DOLLAR:
            INDEX = COMPARE_NUMBER(STRING + 1, 16, &(SYM->VALUE));

            if(INDEX > 0)
            {
                SYM->ID = NUMBER;
                SYM->CONTAINS = SYM->VALUE;
                SYM->TEXT = STRING;
                SYM->LENGTH = 1 + INDEX;
                *PTR = STRING + SYM->LENGTH;
                return true;
            }

            if(INDEX == 0)
            {
                SYM->ID = ADDRESS;
                SYM->TEXT = STRING;
                SYM->LENGTH = 1;
                *PTR = STRING + 1;
                return true;
            }

            SYM->ID = ERROR;
            SYM->TEXT = STRING;
            SYM->LENGTH = 1 + INDEX;
            SYM->ERROR = "Malformed Hex value";
            *PTR = STRING + SYM->LENGTH;
            return true;

        case QUOTE:
        case QUOTES:
            
            char QUOTE_SYM;

            QUOTE_SYM = *STRING++;
            INDEX = FIND_QUOTED(STRING, QUOTE_SYM);

            if(STRING[INDEX] != QUOTE_SYM)
            {
                SYM->ID = ERROR;
                SYM->TEXT = STRING - 1;
                SYM->LENGTH = INDEX + 1;
                SYM->ERROR = (QUOTE_SYM = QUOTES) ? "Malformed String Constant" : 0;
                *PTR = STRING + 1;
                return true;
            }

            SYM->ID = (QUOTE_SYM == QUOTE) ? CHAR : 0;
            SYM->TEXT = STRING;
            SYM->LENGTH = INDEX;
            *PTR = STRING + INDEX + 1;

            return true;

    
        default:

            if(isdigit(*STRING))
            {
                SYM->ID = NUMBER;
                SYM->CONTAINS = SYM->VALUE;
                SYM->TEXT = STRING;
                SYM->LENGTH = INDEX + 2;
                *PTR = STRING + SYM->LENGTH;
            }

            else
            {
                SYM->ID = ERROR;
                SYM->TEXT = STRING;
                SYM->LENGTH = INDEX - 1;
                SYM->ERROR = "Malformed Hex Value";
                *PTR = STRING + SYM->LENGTH;
            }

            // HANDLE ALL OTHER PRE-REQS THAT DONT INVOLVE ENUMERATION SUCH AS REGISTERS
            // OPERANDS, ETC

            HANDLE_IDENTIFIERS(STRING, SYM, PTR);

            break;
    }
}

bool HANDLE_IDENTIFIERS(const char* STRING, struct DIRECTIVE_SYM* SYM, const char** PTR)
{
    int LENGTH, REG_NUM, SYM_TYPE;
    char SEPARATOR;
    int NEXT_REG;
    const char* CUR_POS;

    // GET IDENTIFIER LENGTH

    LENGTH = FIND_IDENTIFIER(STRING);
    
    // HANDLE NON-IDENTIFIER CASES

    if(LENGTH <= 0) 
    {
        SYM_TYPE = FIND_SYMBOL(*STRING);
        if(SYM_TYPE != NONE)
        {
            *PTR = STRING + 1;
            return true;
        }

        SYM->ERROR = "UNRECOGNISED SYMBOL";
        *PTR = STRING + 1;
        return true;
    }

    // CHECK FOR REGISTER
    SYM_TYPE = FIND_REGISTER(STRING, LENGTH, &REG_NUM);
    if(SYM_TYPE != NONE)
    {
        // HANDLE REGISTER LIST
        if(IS_REG_TYPE(SYM_TYPE) && IS_LIST_CHAR(STRING[LENGTH]))
        {
            CUR_POS = STRING + LENGTH;
            
            if(SYM_TYPE == ADDRESS_REG)
                REG_NUM += 8;
            
            SYM->REG_NUM = 1 << REG_NUM;

            // PROCESS REGISTER LIST
            while(IS_LIST_CHAR(*CUR_POS))
            {
                SEPARATOR = *CUR_POS++;
                SYM->LENGTH++;

                LENGTH = FIND_IDENTIFIER(CUR_POS);
                if(LENGTH <= 0)
                {
                    SYM->ERROR = "MISSING REGISTER IN LIST";
                    *PTR = CUR_POS;
                    return true;
                }

                SYM_TYPE = FIND_REGISTER(CUR_POS, LENGTH, &NEXT_REG);
                if(!IS_REG_TYPE(SYM_TYPE))
                {
                    SYM->ERROR = "INVALID REGISTER IN LIST";
                    *PTR = CUR_POS;
                    return true;
                }
            }

            *PTR = CUR_POS;
            return true;
        }

        // HANDLE SINGLE REGISTER
        SYM->REG_NUM = REG_NUM;
        *PTR = STRING + LENGTH;
        return true;
    }
}
