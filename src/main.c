/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "68KSTD.h"
#include "assemble.h"

/* SYSTEM INCLUDES */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    FILE* SOURCE;
    int FILE, ERROR;
    char* MESSAGE;

    if(argc < 2)
    {
        DISPLAY_HELP(argv[0]);
    }

    if((FILE = PARSE_ARGS(argc, argv)) <= 0) return -FILE;

    if((SOURCE = fopen(argv[FILE], "r")) == NULL)
    {
        fprintf(stderr, "Unable to open file '%s'\n", argv[FILE]);
        return EXIT_FAILURE;
    }

    fprintf(stdout, "-------PROCESSING FILE -------\n");

    if((ERROR = PASS_FILE(SOURCE)))
    {
        fprintf(stderr, "Processing failed: %d\n", ERROR);
    }

    if(!ERROR)
    {
        if((MESSAGE = INIT_OUTPUT(argv[FILE])))
        {
            fprintf(stderr, "Initialisation of output format failed: %s\n", MESSAGE);
            ERROR = 2;
        }
        
        else
        {
            rewind(SOURCE);
            if((ERROR = PASS_FILE(SOURCE)))
            {
                fprintf(stderr, "Output generation failed with %d errors\n", ERROR);
            }

            END_OUTPUT();
        }
    }

    fclose(SOURCE);
    return ERROR;
}
