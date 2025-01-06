/* COPYRIGHT (C) HARRY CLARK 2025 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE PERTAINS TOWARDS THE DECLARATION AND DEFINTION OF STDIN AND OUTPUTS */
/* PERTAINING TOWARDS THIS PROJECT - THIS SHOULD ACT AS A MODULARISATION EFFORT */
/* TO REDUCE CLUTTER IN assemble.c */

#ifndef M68K_STD
#define M68K_STD

/* NESTED INCLUDES */

#include "assemble.h"

OPTIONS* FIND_OPTION(const char* VALUE);
int PARSE_ARGS(int argc, char** argv);
void DISPLAY_HELP(const char* MESSAGE);
int HANDLE_OPTION(const char* ARG);
char* INIT_OUTPUT(char* SOURCE);


#endif