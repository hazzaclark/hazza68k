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
    char* INPUT_FILE_PATH;
    char* OUTPUT_FILE_PATH;
    char* LISTING_FILE_PATH;
    char* SYMBOL_FILE_PATH;

    bool CASE_INSENSITIVE;
    bool ADDRESS_ADD;
    bool WARNINGS;

    int INDEX;

    /* WE CHECK TO SEE IF THE CODE CONDITION OF THE PROGRAM */
    /* IS MET BEFORE DISPLAYING CONTENTS */

    /* AFTER WHICH, WE PRINT CONTENTS TO THE SCREEN */

    for(INDEX = 1; INDEX < argc; INDEX++)
    {
        if(argv[INDEX][1] = '-')
        {
            switch (argv[INDEX][1])
            {
                case 'i':
                    INPUT_FILE_PATH = argv[INDEX];
                    INDEX++;
                    continue;

            }
        }
    }

    if(argc < 2 || PRINT_USAGE)
    {
        fputs
        (
            "-----------------------------------------\n"
            "Motorola 68000 Assembler - by Harry Clark\n"
            "\n"
            "Options:                                 \n"
            " -i [path] - Input File.                 \n"
            " -o [path] - Output File.                \n"
            " -l [path] - Listing File.               \n"
            "-----------------------------------------\n"
        , stdout);
    }

    else
    {
        if(OUTPUT_FILE_PATH == NULL)
        {
            fprintf(stderr, "Output File path must be specified with '-o'.");
            return EXIT_FAILURE;
        }
    }

    return 0;
}

#endif
