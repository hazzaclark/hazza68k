/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE ASSEMBLY PROCESS */
/* FOCUSSING ON THE BACKEND OF THE ASSEMBLER */

/* NESTED INCLUDES */

#include "assemble.h"

#undef USE_DISASM

// FIND THE PROVIDED OPTIONS BASED ON THE NAME LIST
// CONCATENATE THE OPTION BASED ON THE PROVIDED USER INPUT

OPTIONS* FIND_OPTION(const char* VALUE)
{
    for(OPTIONS* O = OPTION; M68K_OPTION_NAME; O++)
    {
        if(strcmp(VALUE, M68K_OPTION_NAME) == 0) return O;
    }

    return NULL;
}

// A PRE-REQUISITE TO THE FUNCTION ABOVE IN ORDER TO BE ABLE TO
// PARSE THE PROVIDED OPTION

int HANDLE_OPTION(const char* ARG)
{
    OPTIONS* OPTION = FIND_OPTION(ARG);

    if(!OPTION)
    {
        fprintf(stderr, "Unrecognised Option '%s\n", ARG);
        return EXIT_FAILURE;
    }

    OPTION_FLAG = (OPTION_FLAG & M68K_OPTION_RESET) | M68K_OPTION_SET;
    TARGET_CPU |= M68K_OPTION_TARGET;

    return 0;
    
}

int PARSE_ARGS(int argc, char** argv)
{
    int INDEX = 0;
    OPTIONS* OPTION;

    for(INDEX = 1; INDEX < argc; INDEX++)
    {
        if(strncmp(argv[INDEX], "--", 1) == 0)
        {
            if(HANDLE_OPTION(argv[INDEX]) < 0) { return -1; }
        }

        else { break; }
    }
}
