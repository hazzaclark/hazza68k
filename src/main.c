/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "68KSTD.h"
#include "assemble.h"
#include "dictionary.h"

/* SYSTEM INCLUDES */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    FILE* SOURCE = NULL;
    int FILE, ERROR;

    if((FILE = PARSE_ARGS(argc, argv)) <= 0) return(-FILE);

    printf("-------PROCESSING-------\n");

    if((ERROR = PASS_FILE(SOURCE)))
    {
        fprintf(stderr, "Unable to Pass File %d", ERROR);
    }

    INIT_OUTPUT(argv[FILE]);
    END_OUTPUT();

    return 0;
}
