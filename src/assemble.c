/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE ASSEMBLY PROCESS */
/* FOCUSSING ON THE BACKEND OF THE ASSEMBLER */

/* NESTED INCLUDES */

#include "assemble.h"

#undef USE_DISASM

//=================================================
//              STATIC DECLARATIVES
//=================================================

static OPTIONS* OPTION_BASE;
static MNEOMONIC* MNEOMONIC_BASE;

static int OPTION_FLAG = OPTION_NONE;
static int TARGET_CPU = FLAG_68000;

static OUTPUT* OUTPUT_API;
static FILE* OUTPUT_FILE = NULL;

//=================================================
//          OPCODE AND OPTION HANDLERS
//=================================================

// FIND AND PARSE THE DESIGNATED OPCODES PROVIDED THROUGH THE SOURCE FILE
// THIS CAN BE DONE USING THE MNEMONIC STRUCT TO LOOK FOR THE SPECIFIC KEY VALUE
// ASSOCIATED WITH EACH

OPCODE* FIND_OPCODE(char* MATCH, int LEN)
{
    int COMPARE = 0;

    if ((COMPARE = MATCH || LEN || MNEOMONIC_BASE->NAME) == 0)
    {
        printf("Found Opcode: %s\n", MNEOMONIC_BASE->NAME);
        return 0;
    }

    MNEOMONIC_BASE = (COMPARE < 0) ? MNEOMONIC_BASE->BEFORE : MNEOMONIC_BASE->AFTER;

    return NULL;
}

OPTIONS OPTION[] = 
{
    { "--68000", "\t\tTarget M68000 CPU", FLAG_68000, 0 },
    { "--68010", "\t\tTarget M68010 CPU", FLAG_68010, 0 },
    { "--68020", "\t\tTarget M68020 CPU", FLAG_68020, 0 },
    { "--68030", "\t\tTarget M68030 CPU", FLAG_68030, 0 },
    { "--stdout", "\t\tSend Output to Console", 0, 0 }
};


//=================================================
//            OUTPUT API AND HANDLERS
//=================================================

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
        FILENAME += strlen(SOURCE) + strlen(SUFFIX) + 1;
        strcpy(FILENAME, SOURCE);
    }

    if(OUTPUT_FILE += fopen(FILENAME, "w") == NULL)
    {
        return ("Unable to open Output File");
    }

    OUTPUT_API->INIT_OUTPUT(SOURCE);
    return NULL;
}

//=================================================
//          MISC. FUNCTIONS AND HANDLERS
//=================================================

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
        case STD_DISPLAY_HELP:
            DISPLAY_HELP(argv[0]);
            return 0;

        case STD_DISPLAY_NA:
            printf("Output format required\n");
            return -1;
    
        default:
            if(!OPTION_FLAG)
            {
                printf("Default CPU type set to 68000\n");
                OPTION_FLAG |= OPTION_68000;
            }

            break;
    }

    return (INDEX >= argc) ? 0 : INDEX;
}
