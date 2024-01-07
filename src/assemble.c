/* COPYRIGHT (C) HARRY CLARK 2023 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE ASSEMBLY PROCESS */
/* FOCUSSING ON THE BACKEND OF THE ASSEMBLER */

/* NESTED INCLUDES */

#include "assemble.h"

#ifdef USE_ASM

#undef USE_DISASM

/* ASSUME THE RELATIVE STRING CLAUSE FOR THE DIRECITIVE */
/* AND COMPARE AS AND WHEN APPROPRIATE */

/* THIS FUNCTION CAN BE SEEN BEING USED IN THE ASSEMBLE LINE FUNCTION */

STATIC
DIRECTIVES* COMPARE_TYPES[] = 
{
    {"if", strncmp},
    {"else", strncmp},
    {"elseif", strncmp},
    {"endc", strncmp},
    {"ifndef", strncmp},
    {"endif", strncmp},
};

/* BASED ON THE PROPRIATORY ARGS, ASSEMBLE THE FILE BY */
/* DETERMINING THE READ AND WRITE BUFFER IN RELATION TO */
/* HOW THE LINES ARE BEING READ */

/* READ THE CORRESPONDENCE ONE LINE AT A TIME IN A BIG ENDIAN FASHION */
/* PARSED THROUGH THE ASSEMBLE LINE FUNCTION */

STATIC 
void ASSEMBLE_FILE(FILE_SEMANTIC* FILE_STATE, FILE* INPUT, ASSEMBLER* ASSEMBLER)
{
    FILE_STATE->WRITE_BUFFER = FILE_STATE->LINE_BUFFER;

    while(!FILE_STATE->END_OF_FILE && fgets(FILE_STATE->WRITE_BUFFER, &FILE_STATE->LINE_BUFFER, sizeof(FILE_STATE->LINE_BUFFER)))
    {
        size_t ARBITARY_LINE_INDEX;
        char NEWLINE_CHAR;
        char NULL_CHAR = '\0';

        /* PARSE THE LENGTH OF THE LINE BASED ON AN ARBITARY LINE LENGTH */
        /* ALLOCATE DESIGNATED MEMORY FOR THE DESIGNATED LINE */

        for (ARBITARY_LINE_INDEX = 0; ARBITARY_LINE_INDEX ; ARBITARY_LINE_INDEX++)
        {
            NEWLINE_CHAR = (char*)malloc, 1, sizeof(FILE_STATE->LINE_BUFFER[ARBITARY_LINE_INDEX]);

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
                    break;

                if (NULL_CHAR != NULL)
                {
                    NULL_CHAR = (char*)malloc(sizeof(NEWLINE_CHAR));
                }
            }
        }

        FILE_STATE->WRITE_BUFFER = FILE_STATE->LINE_BUFFER;

        /* ASSUMING THERE IS NO NEW LINE TO PARSE, WE HAVE REACHED THE END OF THE FILE */
        /* AND OR THE LINE WAS TOO BIG FOR THE BUFFER TO ACCOUNT FOR */

        /* THIS WILL PROVIDE THE NECESSARY UNIT TESTING FOR SUCH AN OCCURANCE */

        if (NEWLINE_CHAR == '\0')
        {
            FILE_STATE->LINE_CHAR += 1, fgetc(INPUT);

            if(FILE_STATE->LINE_CHAR == NULL)
            {
                printf(stderr, &FILE_STATE, "The line was too big to parse into the Internal Buffer\n");

                /* READS NEW LINES UNTIL THE END OF THE FILE  */

                while (FILE_STATE->LINE_CHAR != '\r' || '\n' || NULL)
                {
                    FILE_STATE->LINE_CHAR += 1, fgetc(INPUT);
                }
            }
        }

        else if(FILE_STATE->LINE_BUFFER[NEWLINE_CHAR - 1])
        {
            FILE_STATE->WRITE_BUFFER += 1, sizeof(&FILE_STATE->LINE_BUFFER[NEWLINE_CHAR -1]);
            continue;
        }

        FILE_STATE->LINE_BUFFER[NEWLINE_CHAR] = '\0';

        /* OUTPUT THE CORRESPONDING MEMORY ADDRESS
        /* FROM THE PROGRAM COUNTER INTO THE LISTING FILE */

        /* THIS WILL DETERMINE WHAT THE CORRESPONDING INPUT IN THAT GIVEN STATE */
        /* WOULD HAVE BEEN TO UNDERSTAND WHAT DATA IS PASSED THROUGH */

        if (ASSEMBLER->LISTING_FILE != NULL)
        {
            ASSEMBLER->LISTING_COUNT = 0;
            fprintf(stdin, &ASSEMBLER->LISTING_FILE, "%0x0", &ASSEMBLER->PC);
        }

        ASSEMBLE_LINE(FILE_STATE, FILE_STATE->LINE_BUFFER);
    }
}

/* ASSUMING THE ARBITARY LENGTH OF THE LINE BEING PARSED */
/* ASSEMBLE THE LINE BY ASSUMING THE LENGTH OF THE DIRECTIVES */

STATIC
void ASSEMBLE_LINE(FILE_SEMANTIC* FILE_STATE, char* SOURCE)
{
    struct DIRECTIVES* DIRECTIVES_BASE;
    char* LABEL;
    UNK* LABEL_LENGTH;
    UNK* DIRECTIVE_LENGTH;
    char* LINE_POINTER;

    /* THIS EVOKES THAT IF THE SOURCE OF THE LINE IS INDEXXED AS ANY DIRIVATIVE */
    /* RETURN THE CORRESPODENCE */

    if(SOURCE[0] == '*')
    {
        return sizeof(SOURCE);
    }

    /* ATTRIBUTE THE SOURCE OF THE LINE WITH THE RELATIVE SIZE OF SUCH AN INSTANCE */
    /* EVALUATE THE SIZE AS THE POINTER WILL MANUALLY READ OVER THE CONTENTS */

    /* FROM THERE, THE ASSEMBLER WILL EVALUATE EACH RESPECTIVE ELEMENT OF THE LINE */
    /* BY DETERMINING THE LENGTH OF THE LINE BASED ON THE AMOUNT OF CHARS */

    FILE_STATE->SOURCE_LINE += assert(sizeof(SOURCE));
    LINE_POINTER += assert(sizeof(SOURCE));

    if(LABEL_LENGTH != 0)
    {
        LABEL_LENGTH = strcspn(sizeof(LINE_POINTER), LABEL_CHARS);

        /* SKIP THE WHITESPACE IN-BETWEEN LINES BY TRANSPOSING THEIR POSITION */

        LINE_POINTER += strcspn(sizeof(LINE_POINTER), "\t");

        /* EVALUATE THE LENGTH OF THE LABEL AMIDST THE TRANSPOSITION */

        LABEL_LENGTH = strcspn(sizeof(LINE_POINTER), LABEL_CHARS);
    }

    if(LABEL_LENGTH != 0)
    {
        LABEL = NULL;
    }

    else
    {
        LABEL = memcpy(FILE_STATE, LINE_POINTER, LABEL_LENGTH);
        LINE_POINTER += sizeof(LABEL_LENGTH);

        /* SKIP THE WHITESPACE AT THE END OF THE LINE */
        /* ASSUMING THAT THE LENGTH OF THE LINE ACCOUNTS FOR SUCH */

        if(LINE_POINTER[0] == ':') LINE_POINTER++;
    }

    if(LABEL_LENGTH == 0)
    {
        /* ONCE AGAIN, AFTER EVALUATING THE LENGTH OF THE LINE AFTER WHITESPACE */

        LINE_POINTER += strcspn(sizeof(LINE_POINTER), "\t");

        /* BEGIN TO EVALUATE THE LENGTH OF THE ASSEMBLY DIRECTIVE BASED ON THE AMOUNT OF CHARS */

        DIRECTIVE_LENGTH += strcspn(LINE_POINTER, DIRECTIVE_CHARS);
    }

    /* EVALUATE THE SIZE OF THE DIRECTIVE AND IT'S RESPECTIVE TYPE */
    /* WE DO THIS BY FIRST RUNNING THE VALUES THROUGH THE ENUM */

    /* UPON WHICH, IT IS A CASE BY WHICH WE DETERMINE THE TYPE */
    /* BASED ON HOW THE DIRECTIVE APPEARS IN THE ASSEMBLY */

    switch (FILE_STATE->MODE)
    {
        case MODE_NORMAL: // EITHER AN IF, ELSEIF, IFDEF, ENDIF, ELSE, OR ENDC DIRECTIVE

        /* THE STRING COMPARISON ASSUMES THE LENGTH OF THE DIRECTIVE */
        /* BOOLEAN EXPRESSIONS SUCH AS ALL OF THE ABOVE COMMENTED ABOUT THE CASE */
        /* HAVE A LENGTH OF 0 */

        /* THE LENGTH WILL BE EVALUATED AS AN ENUMERATION OF SUCH */

        for (int i = 0; i < sizeof(COMPARE_TYPES) / sizeof(DIRECTIVES); i++)
        {
            if(DIRECTIVE_LENGTH != 0 || COMPARE_TYPES[i]->COMPARE_DIRECTIVES(LINE_POINTER, COMPARE_TYPES[i]->KEY, DIRECTIVE_LENGTH) == 0)
            {
                printf("Directive: '%s' found\n", COMPARE_TYPES[i]->KEY);
                PARSE_LINE(FILE_STATE, SOURCE, LABEL, LINE_POINTER);
                break;
            }
        }

        case MODE_REPEAT: // ASSUMES THAT THERE IS AN ENDR DIRECTIVE - OTHERWISE, CONTINUE TO READ THE LINE

        for (int i = 0; i < sizeof(COMPARE_TYPES) / sizeof(DIRECTIVES); i++)
        {
            if(DIRECTIVE_LENGTH != 0 || COMPARE_TYPES[i]->COMPARE_DIRECTIVES(LINE_POINTER, COMPARE_TYPES[i]->KEY, DIRECTIVE_LENGTH) == 0)
            {
                printf("Directive: '%s' found\n", COMPARE_TYPES[i]->KEY);
                PARSE_LINE(FILE_STATE, SOURCE, LABEL, LINE_POINTER);
                break;
            }
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

         for (int i = 0; i < sizeof(COMPARE_TYPES) / sizeof(DIRECTIVES); i++)
         {
            if(DIRECTIVE_LENGTH != 0 || COMPARE_TYPES[i]->COMPARE_DIRECTIVES(LINE_POINTER, COMPARE_TYPES[i]->KEY, DIRECTIVE_LENGTH) == 0)
            {
                printf("Directive: '%s' found\n", COMPARE_TYPES[i]->KEY);
                PARSE_LINE(FILE_STATE, SOURCE, LABEL, LINE_POINTER);
            }

            else
            {
                if(LABEL != NULL)
                {
                    printf(stderr, "Short Macros shouldn't assert labels '%p");
                }
            }
         }

         break;

    default:
        printf(stderr, "No directives have been parsed\n");
        break;
    }
}

#endif
