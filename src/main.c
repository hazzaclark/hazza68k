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
    int FILE;
    char* MESSAGE;

    if(argc < 2)
    {
        DISPLAY_HELP(argv[0]);
    }

    if((FILE = PARSE_ARGS(argc, argv)) <= 0) return(-FILE);

    if((SOURCE = fopen(argv[FILE], "r")) == NULL)
    {
        fprintf(stderr, "Unable to open file '%s'\n", argv[FILE]);
        return EXIT_FAILURE;
    }

    if((MESSAGE = INIT_OUTPUT(argv[FILE])))
    {
        fprintf(stderr, "Initialisation of Output failed: %s\n", MESSAGE);
        fclose(SOURCE);
        return -2;
    }

    rewind(SOURCE);
    fclose(SOURCE);
    END_OUTPUT();

    return 0;
}
