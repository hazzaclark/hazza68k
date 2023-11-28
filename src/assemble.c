/* COPYRIGHT (C) HARRY CLARK 2023 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE ASSEMBLY PROCESS */
/* FOCUSSING ON THE BACKEND OF THE ASSEMBLER */

/* NESTED INCLUDES */

#include "assemble.h"

#ifdef USE_ASM

#undef USE_DISASM

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

/* ASSUMING THE ARBIT
ARY LENGTH OF THE LINE BEING PARSED */
/* ASSEMBLE THE LINE BY ASSUMING THE LENGTH OF THE DIRECTIVES */

STATIC
void ASSEMBLE_LINE(FILE_SEMANTIC* FILE_STATE, const char* SOURCE)
{

}

#endif
