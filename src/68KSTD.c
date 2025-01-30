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
static OUTPUT* OUTPUT_API = NULL;
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
    char* SUFFIX = ".bin";
    char* FILENAME = NULL;

    if (OPTION_FLAG & STD_DISPLAY_OUT) 
    {
        OUTPUT_FILE = stdout;
    } 
    else 
    {
        FILENAME = malloc(strlen(SOURCE) + strlen(SUFFIX) + 1); 
        if (!FILENAME) 
        {
            return "Memory allocation failed for output filename.";
        }

        strcpy(FILENAME, SOURCE);
        strcat(FILENAME, SUFFIX);

        OUTPUT_FILE = fopen(FILENAME, "w");
        if (!OUTPUT_FILE) 
        {
            free(FILENAME); 
            return "Unable to open output file.";
        }

        free(FILENAME); 
    }

    if (!OUTPUT_API) 
    {
        return "Output API is not initialized.";
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

void DISPLAY_HELP(const char* MESSAGE)
{
    printf("Usage: %s OPTION FILENAME\n Options:-\n", MESSAGE);

    for(OPTIONS* O = OPTION; O->NAME; O++)
    {
        printf("\t%s%s\n", O->NAME, O->HELP);
    }
}

int PARSE_ARGS(int argc, char** argv)
{
    int INDEX = 0;

    if(argc < 2)
    {
        DISPLAY_HELP(argv[0]);
    }

    for(INDEX = 1; INDEX < argc; INDEX++)
    {
        if(strncmp(argv[INDEX], "--", 2) == 0)
        {
            for(OPTIONS* O = OPTION; O->NAME != NULL; O++)
            {
                if(strcmp(argv[INDEX], O->NAME) == 0) break;
            }

            if(OPTION_BASE->NAME != NULL)
            {
                OPTION_FLAG = (OPTION_FLAG & OPTION_BASE->RESET) | OPTION_BASE->SET;
                TARGET_CPU |= M68K_OPTION_TARGET;
            }

            else
            {
                fprintf(stderr, "Unrecognised Option '%s'\n", argv[INDEX]);
            }
        }
    }

    return (INDEX >= argc) ? 0 : INDEX;
}
