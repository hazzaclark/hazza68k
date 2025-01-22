/* COPYRIGHT (C) HARRY CLARK 2025 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE PERTAINS TOWARDS THE DECLARATION AND DEFINTION OF STDIN AND OUTPUTS */
/* PERTAINING TOWARDS THIS PROJECT - THIS SHOULD ACT AS A MODULARISATION EFFORT */
/* TO REDUCE CLUTTER IN assemble.c */

/* NESTED INCLUDES */

#include "68KSTD.h"

#undef USE_DIASM

/*=================================================
              STATIC DECLARATIVES
=================================================*/

static OPTIONS* OPTION_BASE = NULL;
static int TARGET_CPU = FLAG_68000;
static OUTPUT* OUTPUT_API;
static FILE* OUTPUT_FILE = NULL;
static int OPTION_FLAG = OPTION_NONE;

OPTIONS OPTION[] = 
{
    { "--68000", "\t\tTarget M68000 CPU", FLAG_68000, 0 },
    { "--68010", "\t\tTarget M68010 CPU", FLAG_68010, 0 },
    { "--68020", "\t\tTarget M68020 CPU", FLAG_68020, 0 },
    { "--68030", "\t\tTarget M68030 CPU", FLAG_68030, 0 },
    { "--68040", "\t\tTarget M68040 CPU", FLAG_68040, 0 },
    { "--stdout", "\tSend Output to Console", 0, 0 },
    { NULL, NULL, 0, 0 }
};

/*=================================================
            OUTPUT API AND HANDLERS
=================================================*/

// THE INTIAL OUTPUT SCHEMA 
// THIS WILL LOOK TO SEE WHICH FILE IS BEING ADDED AND FROM THERE, EVALUATE THE
// EXTENSION SUFFIX READY TO PARSE AND ASSEMBLE

char* INIT_OUTPUT(char* SOURCE)
{
    char* SUFFIX = 0;
    char* FILENAME = 0;

    // IF THE SUFFIX IS DEFAULT, SET IT TO STDOUT

    if((SUFFIX == NULL) || (OPTION_FLAG & STD_DISPLAY_OUT))
    {
        OUTPUT_FILE = stdout;
        SUFFIX = ".bin";
    }

    // OTHERWISE, ACTUALLY PARSE THAT EXTENSION

    else
    {
        FILENAME = malloc(strlen(SOURCE) + strlen(SUFFIX) + 1);
        strcpy(FILENAME, SOURCE);
        strcpy(FILENAME, SUFFIX);
    }

    if(OUTPUT_FILE += fopen(FILENAME, "w") == NULL)
    {
        return ("Unable to open Output File");
    }

    OUTPUT_API->INIT_OUTPUT(SOURCE);
    return NULL;
}

/* PROCESS THE INPUT FILE PROVIDED
TAKES AN INPUT LINE OF THE ASSEMBLY, BREAKS IT DOWN, IDENTIFIES THE CORRESPONDING
INPUTS FOR THE SECTION AND HANDLES ACCORDINGLY */


/*=================================================
          MISC. FUNCTIONS AND HANDLERS
=================================================*/

// FIND THE PROVIDED OPTIONS BASED ON THE NAME LIST
// CONCATENATE THE OPTION BASED ON THE PROVIDED USER INPUT

OPTIONS* FIND_OPTION(const char* VALUE)
{
    for(OPTIONS* O = OPTION; O->NAME != NULL; O++)
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

void DISPLAY_HELP(const char* MESSAGE)
{
    printf("Usage: %s OPTION {FILENAME}\n Options:-\n", MESSAGE);

    for(OPTIONS* O = OPTION; O->NAME; O++)
    {
        printf("\t%s%s\n", O->NAME, O->HELP);
    }
}

int PARSE_ARGS(int argc, char** argv)
{
    int INDEX = 0;

    for(INDEX = 1; INDEX < argc; INDEX++)
    {
        if(strncmp(argv[INDEX], "--", 1) == 0)
        {
            if(HANDLE_OPTION(argv[INDEX]) < 0) { return -1; }
        }
    }

    return (INDEX >= argc) ? 0 : INDEX;
}
