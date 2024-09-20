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

void ASSEMBLE_FILE(FILE* INPUT)
{   
    struct FILE_SEMANTIC* FILE_STATE = malloc(sizeof(struct FILE_SEMANTIC));
    struct ASSEMBLER* ASSEMBLER = malloc(sizeof(struct ASSEMBLER));

    FILE_STATE->WRITE_BUFFER = (char*)FILE_STATE->LINE_BUFFER;
    FILE_STATE->END_OF_FILE = 0;

    while(!FILE_STATE->END_OF_FILE && fgets(FILE_STATE->WRITE_BUFFER, sizeof(FILE_STATE->LINE_BUFFER), INPUT))
    {
        size_t ARBITARY_LINE_INDEX = 0;
        char NEWLINE_CHAR = 0;

        /* PARSE THE LENGTH OF THE LINE BASED ON AN ARBITARY LINE LENGTH */
        /* ALLOCATE DESIGNATED MEMORY FOR THE DESIGNATED LINE */

        for (ARBITARY_LINE_INDEX = 0; ARBITARY_LINE_INDEX < strlen(FILE_STATE->WRITE_BUFFER); ARBITARY_LINE_INDEX++)
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

        /* ASSUMING THERE IS NO NEW LINE TO PARSE, WE HAVE REACHED THE END OF THE FILE */
        /* AND OR THE LINE WAS TOO BIG FOR THE BUFFER TO ACCOUNT FOR */

        /* THIS WILL PROVIDE THE NECESSARY UNIT TESTING FOR SUCH AN OCCURANCE */

        if (NEWLINE_CHAR == '\0')
        {
            if(FILE_STATE->LINE_CHAR == 0)
            {
                PRINT_SEMANTIC(stderr, "The line was too big to parse into the Internal Buffer\n", 0);

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

        /* OUTPUT THE CORRESPONDING MEMORY ADDRESS */
        /* FROM THE PROGRAM COUNTER INTO THE LISTING FILE */

        /* THIS WILL DETERMINE WHAT THE CORRESPONDING INPUT IN THAT GIVEN STATE */
        /* WOULD HAVE BEEN TO UNDERSTAND WHAT DATA IS PASSED THROUGH */

        if (ASSEMBLER->LISTING_FILE != NULL)
        {
            ASSEMBLER->LISTING_COUNT = 0;
            fprintf(ASSEMBLER->LISTING_FILE, (char*)ASSEMBLER->PC);
        }

        ASSEMBLE_LINE(FILE_STATE, FILE_STATE->LINE_BUFFER);

        /* OUTPUT THE CORRESPONDENCE FROM THE FILE LISTING */
        /* THIS IS GOVERNED BY HOW MANY LINES HAVE BEEN PARSED IN THE LEXER */

        /* BASED ON THE INDEXXING POINTER PASSED THROUGH THE LEXER */
        /* OUTPUT THE CORRESPONDING CHAR TO THE LISTING FILE */

        if (ASSEMBLER->LISTING_FILE != NULL)
        {
            fprintf(stderr, "Writing to listing file: %s\n", (char*)ASSEMBLER->PC); 
            fprintf(ASSEMBLER->LISTING_FILE, (char*)ASSEMBLER->PC, FILE_STATE->WRITE_BUFFER);
        }
    }

    if(ASSEMBLER->INPUT_FILE == NULL)
    {
        fprintf(stderr, "Input file could not be found. Check the path declaration\n");
        exit(1);
    }

    if(ASSEMBLER->OUTPUT_FILE == NULL)
    {
        fprintf(stderr, "Output file could not be found. Check the path declaration\n");
        exit(1);
    }

    fprintf(stdout, "Assembly Complete\n");

    free(FILE_STATE);
    free(ASSEMBLER);
}

/* ASSUMING THE ARBITARY LENGTH OF THE LINE BEING PARSED */
/* ASSEMBLE THE LINE BY ASSUMING THE LENGTH OF THE DIRECTIVES */

void ASSEMBLE_LINE(FILE_SEMANTIC* FILE_STATE, char* SOURCE)
{
    struct DIRECTIVES* DIRECTIVES_BASE = malloc(sizeof(struct DIRECTIVES));
    struct DICTIONARY_ENTRY* LOOKUP = malloc(sizeof(struct DICTIONARY_ENTRY));

    char* LABEL = NULL;
    UNK* LABEL_LENGTH = 0;
    char* DIRECTIVE_LENGTH = 0;
    char* LINE_POINTER = SOURCE;
    int FILE_MODE = 0;

    /* THIS EVOKES THAT IF THE SOURCE OF THE LINE IS INDEXXED AS ANY DERIVATIVE */
    /* RETURN THE CORRESPONDENCE */

    if(SOURCE[0] == '*')
    {
        memset(SOURCE, 0, strlen(SOURCE));
        return;
    }

    /* ATTRIBUTE THE SOURCE OF THE LINE WITH THE RELATIVE SIZE OF SUCH AN INSTANCE */
    /* EVALUATE THE SIZE AS THE POINTER WILL MANUALLY READ OVER THE CONTENTS */

    /* FROM THERE, THE ASSEMBLER WILL EVALUATE EACH RESPECTIVE ELEMENT OF THE LINE */
    /* BY DETERMINING THE LENGTH OF THE LINE BASED ON THE AMOUNT OF CHARS */

    FILE_STATE->SOURCE_LINE = SOURCE;
    LINE_POINTER = SOURCE;

    if(LABEL_LENGTH != 0)
    {
        LABEL_LENGTH += strcspn(LINE_POINTER, LABEL_CHARS);

        /* SKIP THE WHITESPACE IN-BETWEEN LINES BY TRANSPOSING THEIR POSITION */

        LINE_POINTER += strcspn(LINE_POINTER, "\t");

        /* EVALUATE THE LENGTH OF THE LABEL AMIDST THE TRANSPOSITION */

        LABEL_LENGTH += strcspn(LINE_POINTER, LABEL_CHARS);
    }

    else
    {
        LABEL = memcpy(FILE_STATE, LINE_POINTER, *LABEL_LENGTH);
        LINE_POINTER += sizeof(LABEL_LENGTH);

        /* SKIP THE WHITESPACE AT THE END OF THE LINE */
        /* ASSUMING THAT THE LENGTH OF THE LINE ACCOUNTS FOR SUCH */

        if(LINE_POINTER[0] == ':') LINE_POINTER++;
    }

    if(LABEL_LENGTH == 0)
    {
        /* ONCE AGAIN, AFTER EVALUATING THE LENGTH OF THE LINE AFTER WHITESPACE */

        LINE_POINTER += strcspn(LINE_POINTER, "\t");

        /* BEGIN TO EVALUATE THE LENGTH OF THE ASSEMBLY DIRECTIVE BASED ON THE AMOUNT OF CHARS */

        DIRECTIVE_LENGTH += strcspn(LINE_POINTER, DIRECTIVE_CHARS);
    }

    /* EVALUATE THE SIZE OF THE DIRECTIVE AND IT'S RESPECTIVE TYPE */
    /* WE DO THIS BY FIRST RUNNING THE VALUES THROUGH THE ENUM */

    /* UPON WHICH, IT IS A CASE BY WHICH WE DETERMINE THE TYPE */
    /* BASED ON HOW THE DIRECTIVE APPEARS IN THE ASSEMBLY */

    switch (FILE_MODE)
    {
        case MODE_NORMAL: /* EITHER AN IF, ELSEIF, IFDEF, ENDIF, ELSE, OR ENDC DIRECTIVE */

        /* THE STRING COMPARISON ASSUMES THE LENGTH OF THE DIRECTIVE */
        /* BOOLEAN EXPRESSIONS SUCH AS ALL OF THE ABOVE COMMENTED ABOUT THE CASE */
        /* HAVE A LENGTH OF 0 */

        /* THE LENGTH WILL BE EVALUATED AS AN ENUMERATION OF SUCH */

        if(FILE_STATE->FALSE_IF != 0 || DIRECTIVE_LENGTH != 0)
        {
            if(strncmp(LINE_POINTER, "if", *DIRECTIVE_LENGTH) == 0)
            {
                /* CHECK THE SEE IF THERE IS ANY CONCURRENT CODE AFTER THE IF STATEMENT */
                /* ALSO CHECK FOR SUBSEQUENT WHITESPACE */

                if((*FILE_STATE->AFTER_IF) != '\0')
                {
                    fprintf(stderr, "Unexpected If Level after execution\n");
                    exit(EXIT_FAILURE);
                }

                /* THIS IS UNDER THE GUISE OF ASSUMINNG THAT IF STATEMENTS ASDRE NESTED */
                /* WITHIN AN INVALID IF STATEMENT. THIS WILL ITERATE THROUGH EACH RESPECTIVE IF LEVEL TO DETERMINE */
                /* THE VALIDITY */

                FILE_STATE->CURRENT_IF++;
            }

            else if(strncmp(LINE_POINTER, "elseif", *DIRECTIVE_LENGTH) == 0
                || strncmp(LINE_POINTER,  "else",   *DIRECTIVE_LENGTH) == 0
                || strncmp(LINE_POINTER,  "endc",   *DIRECTIVE_LENGTH) == 0
                || strncmp(LINE_POINTER,  "endif",  *DIRECTIVE_LENGTH) == 0)

                {
                    PARSE_LINE(FILE_STATE, FILE_STATE->SOURCE_LINE, LABEL, LINE_POINTER);
                }
        }

        else
        {
            #ifdef USE_DICTIONARY

            struct DICTIONARY_ENTRY* ENTRY = DICTIONARY_LOOKUP(LOOKUP, DIRECTIVE_LENGTH, 0);


            /* IF THERE IS NO RELEVANT ENTRY IN RELATION TO THE SYMBOL TYPE */
            /* CREATE A NEW ENTRY AND PARSE THOSE CONTENTS */

            /* OTHERWISE, CASTING A WARNING PERTAINING TOWARDS THE OFFSET */
            /* AS IT WILL ASSUME THAT THERE IS NOTHING THERE */

            if(ENTRY == NULL || ENTRY->TYPE != SYMBOL_MACRO)
            {
                PARSE_LINE(FILE_STATE, SOURCE, LABEL, LINE_POINTER);
            }
            else
            {
                char* PARAMS = 0;
                unsigned TOTAL_PARAMS = 0;

                /* EVALUATE THE NEW LENGTH OF THE CURRENT LINE POINTER */
                /* AS WELL AS THE DESIGNATED PARAMETERS FOUND IN THE CURRENT LOCATION */

                LINE_POINTER += strspn(LINE_POINTER, DIRECTIVE_CHARS);
                PARAMS += *(char*)malloc(sizeof(FILE_STATE)); 
                TOTAL_PARAMS += 1;

                EXTRACT_MACRO_SPEC();
            }
        }

        break;

        case MODE_REPEAT: /* ASSUMES THAT THERE IS AN ENDR DIRECTIVE - OTHERWISE, CONTINUE TO READ THE LINE */

            if(DIRECTIVE_LENGTH != 0 || strncmp(LINE_POINTER, "endr", DIRECTIVE_LENGTH == 0))
            {
                printf("Directive: '%s' found\n", DIRECTIVES_BASE->KEY);
                PARSE_LINE(FILE_STATE, SOURCE, LABEL, LINE_POINTER);
            }

         break;

         /* THIS ONE IS SELF-EXPLANATORY */
         /* IN ASSEMBLY, YOU DECLARE A MACRO BY DOING THE FOLLOWING: */
         /*================================*/
         /* MACRO_NAME               MACRO */
         /*     (your args go here)        */
         /*                          ENDM */
         /*================================*/
         /* SUCH IS THE CASE WITH A MACRO, ANY ARGUMENTS CAN BE PRESENT */
         /* THE BEST CASE IN ORDER TO COVER ALL BASIS IS TO DO A RUDIMENTARY */
         /* DIRECTIVE LENGTH CHECK TO DETERMINE HOW MUCH WHITESPACE GOVERNS */
         /* THE DECLARATIVE SPACE */ 

        case MODE_MACRO: 
            if(DIRECTIVE_LENGTH != 0 || strncmp(LINE_POINTER, "endm", DIRECTIVE_LENGTH == 0))
            {
                printf("Directive: '%s' found\n", DIRECTIVES_BASE->KEY);
                PARSE_LINE(FILE_STATE, SOURCE, LABEL, LINE_POINTER);
            }

            if(LABEL != NULL)
            {
                fprintf(stderr, "Short Macros shouldn't assert labels");
            }

         break;

    default:
        fprintf(stderr, "No directives have been parsed\n");
        break;
    }


    free(DIRECTIVES_BASE);
    free(LOOKUP);
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
        PRINT_SEMANTIC(FILE_STATE, "Directive cannot be found for this line", 0);
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
            PRINT_SEMANTIC(FILE_STATE, "File Pragmas are out of memory scope\n", "%0x");
            break;
    }

    free(ASM);
    free(LOCATION);
}

/* EXTRACT CORRESPONDING INFORMATION PERTAINING TOWARDS MACRO SIZE AND IT'S RELEVANT SPECIFIER */
/* THIS CAN BE DONE BY ELIMINATING THE WHITESPACE BETWEEN CHAR'S TO EXTRACT AND STORE PARAMS */

void EXTRACT_MACRO_SPEC()
{
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

            case ';':
            case '\0':
                break;
        
            default:
                fprintf(stderr, "No valid Character could be evaluated for this space: %s\n", CHARACTER_SPACE);
                exit(1);
                break;
        }
    }

}


#endif
