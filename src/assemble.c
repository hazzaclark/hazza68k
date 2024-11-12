/* COPYRIGHT (C) HARRY CLARK 2023 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE ASSEMBLY PROCESS */
/* FOCUSSING ON THE BACKEND OF THE ASSEMBLER */

/* NESTED INCLUDES */

#include "assemble.h"
#include "dictionary.h"

#undef USE_DISASM

/* BASED ON THE PROPRIATORY ARGS, ASSEMBLE THE FILE BY */
/* DETERMINING THE READ AND WRITE BUFFER IN RELATION TO */
/* HOW THE LINES ARE BEING READ */

/* READ THE CORRESPONDENCE ONE LINE AT A TIME IN A BIG ENDIAN FASHION */
/* PARSED THROUGH THE ASSEMBLE LINE FUNCTION */

void ASSEMBLE_FILE(FILE_SEMANTIC* FILE_STATE, TEXT* TEXT_INPUT, TEXT* CALLBACK)
{   
    char* LINE_WRITER = 0;

    LINE_WRITER = (char*)FILE_STATE->LINE_BUFFER;
    FILE_STATE->END_OF_FILE = 0;

    while(!FILE_STATE->END_OF_FILE && fgets(FILE_STATE->WRITE_BUFFER, *FILE_STATE->LINE_BUFFER - *LINE_WRITER, (FILE*)CALLBACK) != NULL)
    {
        size_t ARBITARY_LINE_INDEX = 0;
        char NEWLINE_CHAR = 0;
        char NULL_TERM_CHAR = '\0';

        /* PARSE THE LENGTH OF THE LINE BASED ON AN ARBITARY LINE LENGTH */
        /* ALLOCATE DESIGNATED MEMORY FOR THE DESIGNATED LINE */

        for (ARBITARY_LINE_INDEX = 0; ; ARBITARY_LINE_INDEX++)
        {
            NEWLINE_CHAR = FILE_STATE->WRITE_BUFFER[ARBITARY_LINE_INDEX];

            /* THE COMPILER WILL NOW DETERMINE WHICH TYPE OF DIRECTIVE THE CORRESPONDENCE USE */
            /* FOR CHAR PARSING */

            /* THE FOLLOWING DIRECTIVES TERMINATES THE LINE TO PROVIDE ANY CONTENT */
            /* BEING ACCIDENTLY SPILLED OVER INTO AN UNKNOWN STATE */

            switch (NEWLINE_CHAR)
            {
                case '\0':
                case '\r':
                case '\n':
                    break;
            
                default:
                    if (NEWLINE_CHAR == '\0' || NEWLINE_CHAR == '\r' || NEWLINE_CHAR == '\n')
                    {
                        FILE_STATE->WRITE_BUFFER[ARBITARY_LINE_INDEX] = '\0';
                    }
                    break;
            }
        }

        /* ALLOCATED THE DESIGNATED MEMORY TO BE ABLE TO READ A NEWLINE */
        /* BASED ON A GIVEN LINE INDEX */

        /* ONCE NO LONGER NECESSARY, FREE UP THAT MEMORY */ 

        FILE_STATE->WRITE_BUFFER += ARBITARY_LINE_INDEX;
        LINE_WRITER = FILE_STATE->LINE_BUFFER;

        /* ASSUMING THERE IS NO NEW LINE TO PARSE, WE HAVE REACHED THE END OF THE FILE */
        /* AND OR THE LINE WAS TOO BIG FOR THE BUFFER TO ACCOUNT FOR */

        /* THIS WILL PROVIDE THE NECESSARY UNIT TESTING FOR SUCH AN OCCURANCE */

        if (NEWLINE_CHAR == '\0')
        {
            if(FILE_STATE->LINE_CHAR == 0)
            {
                /* READS NEW LINES UNTIL THE END OF THE FILE  */

                while (FILE_STATE->LINE_CHAR == '\r' && FILE_STATE->LINE_CHAR == '\n' && FILE_STATE->LINE_CHAR == 0)
                {
                    FILE_STATE->LINE_CHAR++;
                }
            }
        }

        else if(FILE_STATE->LINE_BUFFER[NEWLINE_CHAR - 1])
        {
            FILE_STATE->WRITE_BUFFER = (char*)sizeof(FILE_STATE->LINE_BUFFER[NEWLINE_CHAR -1]);
            continue;
        }

        FILE_STATE->LINE_BUFFER[(int)NEWLINE_CHAR] = '\0';
        FILE_STATE->INSTRUCTION_COUNT = 0;

        ASSEMBLE_LINE(FILE_STATE, FILE_STATE->LINE_BUFFER);

        /* EVALUATE THE MACRO SPECS FOR EACH FILE */
        /* ASSUMING WE DON'T REACH EOF WITH NO MACROS, THEN THE ASSEMBLY WAS A SUCCESS */

        /* TYPICALLY SPEAKING, THERE CAN AND ALWAYS COULD BE SOME WHITESPACE AT THE END OF A FILE */
        /* SUCH AS AN END: DIRECTIVE */ 

        /* THEREFORE, WE NEED TO ACCOMODATE FOR THAT IN THE EVENT OF NOT BEING ABLE TO DISCERN AN EOF */ 

        switch (*FILE_STATE->MODE)
        {
            case MODE_NORMAL:
                break;

            case MODE_MACRO:
                MACRO_TERMINATE(FILE_STATE);
                printf("MACRO statement found on line %lu, missing an ENDM directive", FILE_STATE->SOURCE_LINE);

            case MODE_REPEAT:
                MACRO_TERMINATE(FILE_STATE);
                printf("REPT statement found on line %lu, missing an ENDR directive", FILE_STATE->SOURCE_LINE);
                break;

            case MODE_WHILE:
                MACRO_TERMINATE(FILE_STATE);
                printf("WHILE statement found on line %lu, missing an ENDW directive", FILE_STATE->SOURCE_LINE);
                break;
        }
    }
}

void ASSEMBLE_LINE(FILE_SEMANTIC* FILE_STATE, char* SOURCE)
{
    UNK LABEL_LENGTH = 0;
    char* SOURCE_LINE_PTR = 0;
    char* LABEL_DIR = 0;
    UNK DIRECTIVE_LENGTH = 0;

    FILE_STATE->SOURCE_LINE = SOURCE;
    SOURCE_LINE_PTR = SOURCE;

    /* PARSE THE CURRENT LABEL LENGTH ASSUMING THAT THERE IS ANY AT ALL */
    /* LABELS ARE ENCOMPASSED BY DEFINNG A FUNCTION OR A METHOD IN ASSEMBLY */
    /* THROUGH THE MEANS OF:                                                 */
    /* MY_LABEL:                                                             */
    /*          MOVE.L D0, (-SP)                                             */ 

    LABEL_LENGTH = strspn(SOURCE_LINE_PTR, LABEL_CHARS);
    SOURCE_LINE_PTR += strspn(SOURCE_LINE_PTR, "\t");

    if(LABEL_LENGTH == 0) { LABEL_DIR = NULL; }

    else { SOURCE_LINE_PTR += LABEL_LENGTH; }

}


/* ADD DIRECTIVE DEFINITIONS FOR THE ASSEMBLER TO LOOK OUT FOR WHEN PARSING LINES */
/* THIS WILL BE THE ASSEMBLERS WAY OF BEING ABLE TO IDENTIFY ELEMENTS */

void ADD_DIRECTIVE_DEFINITION(char IDENTIFIER)
{
    struct FILE_SEMANTIC* FILE_STATE = malloc(sizeof(struct FILE_SEMANTIC));

    switch (IDENTIFIER)
    {
    case 0:
        FILE_STATE->FILE_SYMBOL.TYPE = SYMBOL_CONST;
        FILE_STATE->FILE_SYMBOL.SHARED_VALUE += sizeof(IDENTIFIER);
        break;

    case 1:
        FILE_STATE->FILE_SYMBOL.TYPE = SYMBOL_VAR;
        FILE_STATE->FILE_SYMBOL.SHARED_VALUE += sizeof(IDENTIFIER);
        break;
    
    default:
        break;
    }

    free(FILE_STATE);
}

/* WHAT IT SAYS ON THE TIN, STORE THE RELEVANT MACRO SIZE SPECIFIER */
/* IN RELATION TO AN ARBITRARY SIZE AND LENGTH, ASSUMING THAT THERE IS NO */
/* NULL TERMINATING CHARACTER */

void STORE_MACRO_SIZE_SPEC(void)
{
    size_t SIZE_LENGTH;
    char* LINE_POINTER = NULL;

    /* SKIP THE NULL TERM CHAR, IF ONE EXISTS */

    LINE_POINTER++;

    SIZE_LENGTH = strspn(LINE_POINTER, DIRECTIVE_CHARS);

    /* RETURN AND STORE THE SIZE */

    LINE_POINTER += SIZE_LENGTH;    
}

void STORE_MACRO_PARAMS(void)
{
    struct MACROS* MACRO = malloc(sizeof(struct MACROS));

    /* IDENTIFY THE EOL FOR THE MACRO OFFSET */
    /* THIS IS DONE BY CASTING THE OFFSET TOWARDS WHERE THE MACRO IS */

    while(*MACRO->LINE_POINTER != '\0')
    {
        MACRO->IDENTIFIER += *(char*)(malloc(sizeof(MACRO->LINE_POINTER)));

        /* HANDLE NESTED PARENTHESES THAT CAN BE FOUND IN MACRO DECLARATIVES */
        /* BY SKIPPING THE EOL CHARACTER */

        if((*MACRO->IDENTIFIER) == '(')
        {
            UINT PARAM_DEPTH = 1;

            /* IDENTIFY THE DEPTH BY WHICH THE LINE IS PARSED AND READ */
            /* THIS WILL HELP IN DETERMINING THE EOL AS WELL AS OTHER ABSTRACTIONS */

            if(MACRO->LINE_POINTER += EOF)
            {
                MACRO->IDENTIFIER += *(char*)(malloc(sizeof(MACRO->LINE_POINTER)));

                /* COUROUTINE TO DETERMINE WHICH NESTED CHAR IS PRESENT */

                switch (*MACRO->IDENTIFIER)
                {
                    case '(':
                        PARAM_DEPTH++;
                        continue;
                    case ')':
                        PARAM_DEPTH--;
                         continue;
                    case ';':
                        continue;

                    default:
                        break;
                }
            }

            if((*MACRO->IDENTIFIER) == '\0')
            {
                break;
            }

            MACRO->MACRO_PARAM_START = MACRO->LINE_POINTER;
        }
    }

    free(MACRO);
}

/* PARSE THE LINE BASED ON RELEVANT LOCATION OF THE SOURCE LINE */
/* AS WELL AS BASED ON WHICH INSTRUCTION IS CURRENTLY BEING EXECUTED IN THE PC */

void PARSE_LINE(FILE_SEMANTIC* FILE_STATE, char* LINE, char* LABEL, char* POINTER)
{
    int RESULT = 0;
    struct ASSEMBLER* ASM = malloc(sizeof(struct ASSEMBLER));
    struct LOCATION* LOCATION = malloc(sizeof(struct LOCATION));

    ULONG* PC_START_OFFSET = 0;
    UNK* PC_START_OFFSET_POS = (UNK*)FILE_STATE->OFFSET_POS;

    /* PARSE THE SOURCE LINE BASED ON THE CURRENT STRING */

    switch(RESULT)
    {
        case 0:
            FILE_STATE->END_OF_FILE = false;
            
            if(!FILE_STATE)
            {
                free(FILE_STATE);
            }

            else 
            {
                LOCATION += *(UNK*)FILE_STATE->OFFSET_POS;
                LOCATION += *(UNK*)PC_START_OFFSET;
                LOCATION += *(ULONG*)PC_START_OFFSET_POS;
                LOCATION = (void*)LABEL ? POINTER : (void*)&LINE;
            }
        break;

        case 1:
            break;

        case 2:
            break;
    }

    free(ASM);
    free(LOCATION);
}

/* EXTRACT CORRESPONDING INFORMATION PERTAINING TOWARDS MACRO SIZE AND IT'S RELEVANT SPECIFIER */
/* THIS CAN BE DONE BY ELIMINATING THE WHITESPACE BETWEEN CHAR'S TO EXTRACT AND STORE PARAMS */

void EXTRACT_MACRO_SPEC()
{
    LOCATION* LOCATION = malloc(sizeof(struct LOCATION));
    MACROS* MACROS = malloc(sizeof(struct MACROS));
    FILE_SEMANTIC* FILE_STATE = malloc(sizeof(struct FILE_SEMANTIC));

    char* CHARACTER_SPACE = 0;
    char* LINE_POINTER = 0;
    char* PARAMS = 0;
    char* PARAM_POINTER = 0;
    UNK SIZE_LENGTH = 0;
    unsigned PARAM_SPACING = 0;

    /* ASSUME THAT A MACRO SPEC EXISTS WITHIN THE CURRENT CONTEXT */
    /* ITERATE THROUGH EACH RESPECTIVE ELEMENT AND STORE */ 

    if(LINE_POINTER[0] == '.')
    {
        LINE_POINTER++;
        SIZE_LENGTH = strspn(LINE_POINTER, DIRECTIVE_CHARS);
        PARAMS[0] += ((FILE_STATE || LINE_POINTER || SIZE_LENGTH));

        LINE_POINTER += SIZE_LENGTH;
    }

    else
    {
        *PARAMS = 0;
    }

    /* STORE THE POINTER AT THE START OF THE SOURCE */

    PARAM_POINTER += *LINE_POINTER += strspn(LINE_POINTER, "\t");

    /* EVLAUATE UNDER THE ASSUMPTION OF THERE BEING AN ENCAPSULATED FUNCTION TYPE */
    /* FOR EXAMPLE, SONIC THE HEDGEHOG'S SOURCE CODE OUTLINED FUNCTION TYPES FOR INCREMENTING DATA TO REGISTERS */ 

    if(!(PARAM_POINTER = 0) && (CHARACTER_SPACE += '('))
    {
        CHARACTER_SPACE += *LINE_POINTER++;

        switch (*CHARACTER_SPACE)
        {
            case '(':
                PARAM_SPACING++;
                break;

            case ')':
                PARAM_SPACING--;
                break;

            case ';':
            case '\0':
                PARAMS = (char*)sizeof(PARAM_SPACING);
                break;
        
            default:
                fprintf(stderr, "No valid Character could be evaluated for this space: %s\n", CHARACTER_SPACE);
                exit(1);
                break;
        }
    }

    /* NOW, DETERMINE ANY AND ALL DUPLICATES FROM THE PRE-EXISTING PARAMETERS */
    /* WE CAN DO SO BY ADDING TO THE PRE-EXISTING TYPE */ 

    PARAMS += *(char*)PARAMS;

    /* INVOKE THE CORRESPONDING MACRO BY DETERMINING THE LOCATIOON */

    LOCATION->FILE_PATH = MACROS->MACRO_CHAR;
    LOCATION->LINE_POS = 0;
    LOCATION->PREV = FILE_STATE->SOURCE_LINE;

    /* ITERATE OVER EACH RESPECTIVE LINE THAT THE MACRO ENCOMPASSES */
    /* TAKE INTO ACCOUNT TAB INDENTATION */

    for (char INDEX = (*(char*)MACROS->LINE_POINTER); (INDEX != 0); INDEX++)
    {
        LOCATION->LINE_POS++;
    }
    
}
