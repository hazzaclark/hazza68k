/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE ASSEMBLY PROCESS */
/* FOCUSSING ON THE BACKEND OF THE ASSEMBLER */

/* NESTED INCLUDES */

#include "assemble.h"

#undef USE_DISASM

static OPTIONS* OPTION_BASE;

static int OPTION_FLAG = OPTION_NONE;
static int TARGET_CPU = FLAG_68000;

OPTIONS OPTION[] = 
{
    { "--68000", "\t\tTarget M68000 CPU", FLAG_68000, 0 },
    { "--68010", "\t\tTarget M68010 CPU", FLAG_68010, 0 },
    { "--68020", "\t\tTarget M68020 CPU", FLAG_68020, 0 },
    { "--68030", "\t\tTarget M68030 CPU", FLAG_68030, 0 },
    { "--stdout", "\t\tSend Output to Console", 0, 0 }
};


// FIND THE PROVIDED OPTIONS BASED ON THE NAME LIST
// CONCATENATE THE OPTION BASED ON THE PROVIDED USER INPUT

OPTIONS* FIND_OPTION(const char* VALUE)
{
    for(OPTIONS* O = OPTION; OPTION_BASE->NAME; O++)
    {
        if(strcmp(VALUE, OPTION_BASE->NAME) == 0) return O;
    }

    return NULL;
}

// A PRE-REQUISITE TO THE FUNCTION ABOVE IN ORDER TO BE ABLE TO
// PARSE THE PROVIDED OPTION

int HANDLE_OPTION(const char* ARG)
{
    OPTIONS* OPTION_TYPE = FIND_OPTION(ARG);

    if(!OPTION_TYPE)
    {
        fprintf(stderr, "Unrecognised Option '%s\n", ARG);
        return EXIT_FAILURE;
    }

    OPTION_FLAG = (OPTION_FLAG & OPTION_BASE->RESET) | OPTION_BASE->SET;
    TARGET_CPU |= M68K_OPTION_TARGET;

    return 0;
    
}

void DISPLAY_HELP(const char* NAME)
{
    printf("Usage: %s {options} {filename}\n Options:-\n", NAME);

    for(OPTIONS* O = OPTION; OPTION_BASE->NAME; O++)
    {
        printf("\t%s%s\n", O->NAME, O->HELP);
    }
}

int PARSE_ARGS(int argc, char** argv)
{
    int INDEX = 0;
    int OPTION_FLAG = 0;

    for(INDEX = 1; INDEX < argc; INDEX++)
    {
        if(strncmp(argv[INDEX], "--", 1) == 0)
        {
            if(HANDLE_OPTION(argv[INDEX]) < 0) { return -1; }
        }
    }

    switch (OPTION_FLAG)
    {
        case STD_DISPLAY_TEXT:
            DISPLAY_HELP(argv[0]);
            return 0;
    
    default:
        break;
    }

    return 0;
}
