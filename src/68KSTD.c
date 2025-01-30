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

void NULL_INIT_OUTPUT(char* SOURCE) {}
void NULL_NEXT_LINE(int LINE, char* CODE) {}
void NULL_SET_ADDRESS(U32 ADDRESS) {}
void NULL_SET_START(U32 ADDRESS) {}
void NULL_ADD_BYTE(U8 BYTE) {}
void NULL_END_OUTPUT(void) {}

static OUTPUT NULL_OUTPUT_API = 
{
    NULL_INIT_OUTPUT,
    NULL_NEXT_LINE,
    NULL_SET_ADDRESS,
    NULL_SET_START,
    NULL_ADD_BYTE,
    NULL_END_OUTPUT
};

/*=================================================
            OUTPUT API AND HANDLERS
=================================================*/

// THE INTIAL OUTPUT SCHEMA 
// THIS WILL LOOK TO SEE WHICH FILE IS BEING ADDED AND FROM THERE, EVALUATE THE
// EXTENSION SUFFIX READY TO PARSE AND ASSEMBLE

static OUTPUT *OUTPUT_ROUTINE = &NULL_OUTPUT_API;

char* INIT_OUTPUT(char* SOURCE)
{
    char* SUFFIX = NULL, *DOT = NULL;
    char* FILENAME = NULL;

    switch (OPTION_FLAG)
    {
        case STD_DISPLAY_NA:
            OUTPUT_ROUTINE = &NULL_OUTPUT_API;
            break;
    }

    if((SUFFIX == NULL) || (OPTION_FLAG & STD_DISPLAY_OUT))
    {
        OUTPUT_FILE = stdout;
    }

    else
    {
        FILENAME = malloc(strlen(SOURCE) + strlen(SUFFIX) + 1);
        strcpy(FILENAME, SOURCE);

        if((DOT = strrchr(FILENAME, PERIOD)) != NULL) *DOT = PARAM_EOS;

        strcat(FILENAME, SUFFIX);

        if((OUTPUT_FILE = fopen(FILENAME, "w")) == NULL)
        {
            return ("Unable to open Output File");
        }
    }

    if (OUTPUT_API && OUTPUT_API->INIT_OUTPUT) 
    {
        OUTPUT_API->INIT_OUTPUT(SOURCE);
    
    }
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
    int FOUND_FILE = 0;
    
    for(INDEX = 1; INDEX < argc; INDEX++)
    {
        if(strncmp(argv[INDEX], "--", 2) == 0)
        {
            OPTIONS* FOUND = NULL;
            for(OPTIONS* O = OPTION; O->NAME != NULL; O++)
            {
                if(strcmp(argv[INDEX], O->NAME) == 0)
                {
                    FOUND = O;  
                    break;
                }
            }
           
            if(FOUND != NULL)
            {
                OPTION_FLAG = (OPTION_FLAG & FOUND->RESET) | FOUND->SET;
                TARGET_CPU |= M68K_OPTION_TARGET;
            }
            else
            {
                fprintf(stderr, "Unrecognised Option '%s'\n", argv[INDEX]);
            }
        }
        else 
        {
            FOUND_FILE = 1;
            break;
        }
    }
   
    return (FOUND_FILE ? INDEX : 0);
}
