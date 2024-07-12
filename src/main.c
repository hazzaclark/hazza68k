/* COPYRIGHT (C) HARRY CLARK 2023 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "assemble.h"
#include "common.h"
#include "disasm.h"

#ifdef USE_DISASM


int main(int argc, char** argv)
{
    bool PRINT_USAGE = false;
    int INDEX;         

    char* INPUT_FILE_PATH;
    char* OUTPUT_FILE_PATH;


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
            "=========================================\n"
            "  HARRY CLARK - MOTOROLA 68K ASSEMBLER  \n"
            "=========================================\n"
            "\n"
            "Options:                                 \n"
            " -i [path] - Input File.                 \n"
            " -o [path] - Output File.                \n"
            " -l [path] - Listing File.               \n"
            "=========================================\n"
        , stdout);
    }
    else
    {

        FILE* INPUT_FILE;
        FILE* OUTPUT_FILE;

       if(OUTPUT_FILE_PATH == NULL)
       {
            fprintf(stderr, "Output file path must be specified with '-o'");
       }

       else
       {

            if(INPUT_FILE_PATH == NULL) INPUT_FILE = stdin;
            else INPUT_FILE = fopen(INPUT_FILE_PATH, "r");

            OUTPUT_FILE = fopen(OUTPUT_FILE_PATH, "wb");
                
            if(INPUT_FILE && OUTPUT_FILE && INPUT_FILE_PATH == NULL)
            {
                fprintf(stderr, "Could not Assemble");
            }

            fclose(OUTPUT_FILE);
        }

        fclose(INPUT_FILE);
    }

    return 0;
}

#endif
