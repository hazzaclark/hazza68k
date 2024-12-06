/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "assemble.h"

/* SYSTEM INCLUDES */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    FILE* SOURCE;
    int FILE;
    char* MESSAGE;

    if((FILE = PARSE_ARGS(argc, argv)) <= 0) return(-FILE);

    if((SOURCE = fopen(argv[FILE], "r")) == NULL)
    {
        fprintf(stderr, "Unable to open file '%s'\n", argv[FILE]);
        return EXIT_FAILURE;
    }

    if((MESSAGE = INIT_OUTPUT(argv[FILE])))
    {
        fprintf(stderr, "Initialisation of Output failed: %s\n", MESSAGE);
        return -2;
    }

    else
    {
        rewind(SOURCE);
        END_OUTPUT();
    }

}
