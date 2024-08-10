/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE PERTAINS TOWARDS THE FUNCTIONALITY SURROUNDING THE PARSING */
/* AND LEXICAL DEFINITIONS OF M68K INSTRUCTIONS AND OPCODES */

/* NESTED INCLUDES */

#include "common.h"
#include "assemble.h"
#include "disasm.h"

#ifdef USE_DISASM

/* RESOLVE THE EXPRESSION TYPE BASED ON LOWER LEVEL LEXICAL ARGS AND TYPES */
/* THIS WILL USE SWITCH CASE TO DISCERN THE VARIOUS TYPES FROM THE ENUMS */

bool RESOLVE_EXP(struct FILE_SEMANTIC* FILE_STATE, EXPRESSION_TYPE* EXP_TYPE, long* VALUE)
{
    bool SUCCESS = true;
    char EXP_EVAL = *(char*)EXP_TYPE;

    switch (EXP_EVAL)
    {
        case EXPRESSION_NUMBER:
        case EXPRESSION_ADD:
        case EXPRESSION_MULTIPLY:
        case EXPRESSION_DIVIDE:
        case EXPRESSION_MODULO:
        case EXPRESSION_LOGICAL_OR:
        case EXPRESSION_LOGICAL_AND:
        case EXPRESSION_BITWISE_OR:
        case EXPRESSION_BITWISE_AND:
        case EXPRESSION_BITWISE_XOR:
        {
            if(!RESOLVE_EXP(FILE_STATE, &EXP_TYPE[0], (long*)FILE_STATE->LEFT_NODE) 
            || !RESOLVE_EXP(FILE_STATE, &EXP_TYPE[1], (long*)FILE_STATE->RIGHT_NODE)) 
            
            SUCCESS = false;
        }

        if(SUCCESS)
        {
            switch (EXP_EVAL)
            {
                case EXPRESSION_SUBTRACT:
                    *VALUE = FILE_STATE->LEFT_NODE - FILE_STATE->RIGHT_NODE;
                    break;
            
                default:
                    break;
            }
        }
    }

    return SUCCESS;
}

#endif