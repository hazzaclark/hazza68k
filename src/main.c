/* COPYRIGHT (C) HARRY CLARK 2023 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "common.h"
#include "disasm.h"

#ifdef USE_DISASM

int main(int argc, char** argv)
{
    bool PRINT_USAGE;

    /* WE CHECK TO SEE IF THE CODE CONDITION OF THE PROGRAM */
    /* IS MET BEFORE DISPLAYING CONTENTS */

    /* AFTER WHICH, WE PRINT CONTENTS TO THE SCREEN */

    if(argc < 2 || PRINT_USAGE)
    {
        fputs
        (
            "-----------------------------------------\n"
            "Motorola 68000 Assembler - by Harry Clark\n"
            "-----------------------------------------\n"
        , stdout);
    }
}

#endif
