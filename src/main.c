/* COPYRIGHT (C) HARRY CLARK 2023 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "common.h"
#include "disasm.h"

#ifdef USE_DISASM

static FILE* INPUT_FILE;
static FILE* OUTPUT_FILE;
static FILE* SYMBOL_FILE; 
static FILE* LISTING_FILE;

static char* INPUT_FILE_PATH;
static char* OUTPUT_FILE_PATH;

/* THIS FUNCTION SERVES TO PROVIDE A SURROGATE MEANS OF DECLUTTERING */
/* THE MAIN FUNCTION WITH VERBOSE STATEMENTS */

void CHECK_VALID_ARGS(void)
{
    if(OUTPUT_FILE_PATH == NULL)
    {
        fprintf(stderr, "Output file path must be specified using '-o");
    }

    else
    {
        INPUT_FILE = (INPUT_FILE_PATH == NULL) ? stdin : fopen(INPUT_FILE_PATH, "r");
    }
}

int main(int argc, char** argv)
{
    bool PRINT_USAGE = false;
    int INDEX;         

    /* WE CHECK TO SEE IF THE CODE CONDITION OF THE PROGRAM */
    /* IS MET BEFORE DISPLAYING CONTENTS */

    /* AFTER WHICH, WE PRINT CONTENTS TO THE SCREEN */

    for(INDEX = 1; INDEX < argc; INDEX++)
    {
        if(argv[INDEX][0] == '-') 
        {
            switch (argv[INDEX][1])
            {
                case 'i':
                    if (INDEX + 1 < argc) 
                    {
                        INPUT_FILE_PATH = argv[INDEX + 1];
                        INDEX++;
                    } 
                    else 
                    {
                        fprintf(stderr, "No input file path specified after '-i'.\n");
                        return EXIT_FAILURE;
                    }

                    continue;
                case 'o':
                    if (INDEX + 1 < argc) 
                    {
                        OUTPUT_FILE_PATH = argv[INDEX + 1];
                        INDEX++;
                    } 
                    else 
                    {
                        fprintf(stderr, "No output file path specified after '-o'.\n");
                        return EXIT_FAILURE;
                    }
                    continue;

                case 'u':
                    PRINT_USAGE = true;
                    continue;

                default:
                    fprintf(stderr, "Unknown option '-%c'.\n", argv[INDEX][1]);
                    return EXIT_FAILURE;
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
       CHECK_VALID_ARGS();
    }

    return 0;
}

#endif
