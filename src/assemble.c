/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE ASSEMBLY PROCESS */
/* FOCUSSING ON THE BACKEND OF THE ASSEMBLER */

/* NESTED INCLUDES */

#include "assemble.h"

#undef USE_DISASM

static MNEOMONIC* MNEOMONIC_BASE = NULL;

/*=================================================
          OPCODE AND OPTION HANDLERS
=================================================*/

// FIND AND PARSE THE DESIGNATED OPCODES PROVIDED THROUGH THE SOURCE FILE
// THIS CAN BE DONE USING THE MNEMONIC STRUCT TO LOOK FOR THE SPECIFIC KEY VALUE
// ASSOCIATED WITH EACH

OPCODE* FIND_OPCODE(char* MATCH, int LEN)
{
    int COMPARE = 0;

    COMPARE = strncmp(MATCH, MNEOMONIC_BASE->NAME, LEN);
    
    if (COMPARE == 0) 
    {
        printf("Found Opcode: %s\n", MNEOMONIC_BASE->NAME);
        return MNEOMONIC_BASE->OPCODES; 
    }

    MNEOMONIC_BASE = (COMPARE < 0) ? MNEOMONIC_BASE->BEFORE : MNEOMONIC_BASE->AFTER;
    return NULL;
}

char* PROC_INPUT(int LINE, char* BUFFER)
{

}

int PASS_FILE(FILE* SOURCE)
{
    char BUFFER[MAX_BIT_ARGS];
    char* MESSAGE;

    int LINE, ERROR = 0;

    /* ASSUME THERE IS A CURRENT FILE BEING PASSED THROUGH */
    /* EVALUATE LENGTH FROM THE START TO THE END */

    while(fgets(BUFFER, MAX_BIT_ARGS, SOURCE))
    {
        BUFFER[strlen(BUFFER) - 1] = EOF;
        LINE++;

        /* CHECK THROUGH EACH CORRESPONDING LINE */

        NEXT_LINE(LINE, BUFFER);
    }
}
