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
    char* RESULT;

    /* ASSUME THERE IS A CURRENT FILE BEING PASSED THROUGH */
    /* EVALUATE LENGTH FROM THE START TO THE END */

    while(fgets(BUFFER, MAX_BIT_ARGS, SOURCE))
    {
        BUFFER[strcspn(BUFFER, "\n")] = PARAM_EOS;
        LINE++;

        if (strlen(BUFFER) == 0)
        {
            fprintf(stderr, "Warning: Skipping empty line %d\n", LINE);
            continue;
        }

        printf("%4d|%s\n", LINE, BUFFER);

        /* CHECK THROUGH EACH CORRESPONDING LINE */

        if((RESULT = PROC_INPUT(BUFFER)) != NULL)
        {
            printf("%s\n", LINE, BUFFER, RESULT);
        }    
    }

    return 0;
}

/* PROCESS THE INPUT PROVIDED FROM THE INPUT LINE */
/* THE FOLLOWING WILL BREAK DOWN THE CORRESPONDENCE INTO SMALLER SECTIONS */
/* IN ORDER TO PARSE EXPRESSIONS AND CALLS THE RELEVANT ROUTINE TO HANDLE SAID INPUT */

char* PROC_INPUT(char* BUFFER)
{   
    DIRECTIVE_SYM* HEAD, **TAIL, *SYM, *TOP;
    INPUT* INP = (INPUT*)malloc(sizeof(INP));
    int LEN = 0;

    TAIL = &HEAD;
    SYM = (DIRECTIVE_SYM*)malloc(sizeof(DIRECTIVE_SYM));
    TOP = (DIRECTIVE_SYM*)malloc(sizeof(DIRECTIVE_SYM));

     memset(INP, 0, sizeof(INPUT));

    while(NEXT_SYM(&BUFFER, SYM))
    {
        // IF WE ENCOUNTER AN ERROR SYMBOL

        if(SYM->ID == ERROR) return(SYM->ERROR);

        // IF WE ENCOUNTER A COMMENT SYMBOL, THEN
        // DISCERN WHETHER THIS IS EOL AND MOVE ONTO THE NEXT LINE

        if(((SYM->ID == MUL) && (LEN == 0)) || (SYM->ID == SEMICOLON)) break;

        // ADD TO THE END OF THE SYM LIST

        *TAIL = SYM;
        TAIL = &SYM->NEXT;
        LEN++;
    }

    // IF THERE ARE NO MORE SYMBOLS WITHIN THE BUFFER

    *TAIL = NULL;

    // NOW ANALYSE THE LINE TO SEE WHAT THE FINAL OUTCOME WAS OF THE PREVIOUS
    // THIS WILL BE DONE BY LOOKING AT THE HEAD OF THE LIST AND CHECKING FOR EMPTY LINES

    TAIL = &HEAD;

    // INIT BUFFER FOR PARSING LINE

    PARSED* P_LINE = (PARSED*)malloc(sizeof(P_LINE));
    memset(P_LINE, 0, sizeof(*P_LINE));
    P_LINE->SYM = HEAD;

    // START AT THE BEGINNING ON THE LIST, AND WORK OUR WAY DOWNWARDS

    SYM = HEAD;

    if(SYM->ID == SYM_IDENTIFIER)
    {
        P_LINE->LABEL = SYM;
        SYM = SYM->NEXT;
    }

    if(SYM->ID == COLON) SYM = SYM->NEXT;

    // NOW WE WILL PROCESS THE INSTRUCTIONS OR DIRECTIVES PROVIDED IN SEPERATE FUNCTIONS

    if(SYM->ID == SYM_OPCODE)
    {
        INP->ACTION = SYM;
        INP->INSTR = true;
        INP->ARGS = 0;

        // IS THIS THE END OF THE INSTRUCTION?

        if((SYM = *(TAIL = &(SYM->NEXT))) == NULL) return PROCESS_INSTRUCTION(INP);

        // DEFINE EXPLICIT SIZING PER EACH INSTRUCTION (ASSUMING IT HAS BEEN CAST)

        switch (SYM->ID)
        {
            case BYTE:
                INP->SIZE = SIZE_BYTE;
                SYM = *(TAIL = &(SYM->NEXT));
                break;

            case WORD:
                INP->SIZE = SIZE_WORD;
                SYM = *(TAIL = &(SYM->NEXT));
                break;

            case LONG:
                INP->SIZE = SIZE_LONG;
                SYM = *(TAIL = &(SYM->NEXT));
                break;
        
            default:
                break;
        }

        // GATHER ALL OF THE CORRESPONDING ARGUMENTS ASSOCIATED
        // WITH THE EXPLICIT SIZING

        // THIS WILL ALLOW US TO EVALUATE EXPRESSIONS TO PROCESS THE INSTRUCTION

        while(SYM != NULL)
        {
            // ASSUME THAT THERE ARE TOO MANY ARGS PROVIDED

            if(INP->ARGS == PARAM_INSTR_ARGS) return ("Too many Arguments for a valid Opcode");

            if(SYM->ID == HASH)
            {
                HEAD = (DIRECTIVE_SYM*)malloc(sizeof(SYM));
                HEAD->ID = EXP;
                HEAD->EXPR = TOP;
                HEAD->NEXT = SYM;
                SYM = *TAIL;
                SYM->NEXT = HEAD;
            }

            else
            {
                HEAD->ID = EXP;
                HEAD->EXPR = TOP;
                HEAD->NEXT = SYM;
                *TAIL = HEAD;
                TAIL = &(HEAD->NEXT);
                *TAIL = SYM;
                SYM = HEAD;
            }
        }

        return PROCESS_INSTRUCTION(INP);
 
    }

    // DO WE HAVE A VALID DIRECTIVE WITHIN THIS DECLARATION!?

    if(FIND_DIRECTIVE(SYM->ID) != NULL)
    {
        INP->ACTION = SYM;
        INP->INSTR = false;
        INP->ARGS = 0;

        // WHEN WE REACH THE END OF THE INSTRICTION

        if((SYM = *(TAIL = &(SYM->NEXT))) == NULL) return PROCESS_DIRECTIVE(INP);

        switch (SYM->ID)
        {
            case BYTE:
                INP->SIZE = SIZE_BYTE;
                SYM = *(TAIL = &(SYM->NEXT));
                break;

            case WORD:
                INP->SIZE = SIZE_WORD;
                SYM = *(TAIL = &(SYM->NEXT));
                break;

            case LONG:
                INP->SIZE = SIZE_LONG;
                SYM = *(TAIL = &(SYM->NEXT));
                break;
        
            default:
                break;
        }
    }

    return PROCESS_DIRECTIVE(INP);
}

/* FIND THE NEXT AVAILABLE SYMBOL BASED OFF OF THE POINTER REFERENCE */
/* ADDS TO THE SYMBOL RECORD TO VALIDATE WHICH SYMBOLS ARE BEING PASSED THROUGH */
/* FROM THE SOURCE FILE */

int NEXT_SYM(char** PTR, DIRECTIVE_SYM* SYM)
{
    char* STRING;
    char QUOTE_SYM;
    int INDEX = 0;

    DIRECTIVES ID;

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
            if((INDEX = FIND_IDENTIFIER(STRING + 1)) > 0)
            {
                if((SYM->ID = FIND_KEYWORD(KEYWORDS, STRING + 1, INDEX)) != 0)
                {
                    SYM->NEXT = STRING;
                    SYM->LENGTH = 1 + INDEX;
                    *PTR = STRING + SYM->LENGTH;
                }

                else
                {
                    SYM->ID = ERROR;
                    SYM->TEXT = STRING;
                    SYM->LENGTH = 1 + INDEX;
                    SYM->ERROR = "Unrecognised Keyword";
                    *PTR = STRING + SYM->LENGTH;
                }
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

            HANDLE_IDENTIFIERS(STRING, SYM, (const char**)PTR);

            break;
    }
}

bool HANDLE_IDENTIFIERS(char* STRING, struct DIRECTIVE_SYM* SYM, const char** PTR)
{
    int LENGTH, SYM_TYPE;
    int REG_NUM = 0;
    int NEXT_REG;
    char* CUR_POS = 0;

    // GET IDENTIFIER LENGTH

    LENGTH = FIND_IDENTIFIER(STRING);
    
    // HANDLE NON-IDENTIFIER CASES

    if(LENGTH <= 0) 
    {
        SYM_TYPE = FIND_SYMBOL(STRING);
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

    SYM_TYPE = FIND_REGISTER(CUR_POS, LENGTH, &NEXT_REG);
    if(SYM_TYPE != NONE)
    {
        // HANDLE REGISTER LIST

        if(IS_REG_TYPE(SYM_TYPE) && IS_LIST_CHAR(STRING[LENGTH]))
        {
            CUR_POS = STRING + LENGTH;
            
            if(SYM_TYPE == ADDRESS_REG)
                REG_NUM = 8;
            
            SYM->REG_NUM = 1 << REG_NUM;

            // PROCESS REGISTER LIST

            while(IS_LIST_CHAR(*CUR_POS))
            {
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

        SYM->REG_NUM += REG_NUM;
        *PTR = STRING + SYM->LENGTH;
        return true;
    }

    return false;
}
