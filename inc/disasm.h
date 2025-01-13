/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE ASSEMBLY PROCESS */
/* FOCUSSING ON THE BACKEND OF THE ASSEMBLER */

#ifndef DISASSEMBLY
#define DISASSEMBLY

/* NESTED INCLUDES */

#include "common.h"

/* SYSTEM INCLUDES */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>

#if defined(USE_DISASM)
#define USE_DISASM
#else
#define USE_DISASM

#define     SIZE_BYTE       1 << 0
#define     SIZE_SHORT      1 << 1
#define     SIZE_WORD       1 << 2
#define     SIZE_QUAD       1 << 3
#define     SIZE_UNDEF      1 << 4

#define		MAX_CHAR		32
#define		MAX_BIT_ARGS	256

#if defined(TYPE_ARGS)
#define TYPE_ARGS
#else
#define TYPE_ARGS

extern UNK* TYPE;

#define     CREATE_TYPE(VALUE) \
static TYPE* ARBITARY_TYPE ## VALUE(TYPE* NODE); \


#define 		M68K_BOOL_FORMAT		"%d"
#define			M68K_BYTE_FORMAT		"%02x"	
#define			M68K_WORD_FORMAT		"%04x"
#define			M68K_LONG_FORMAT		"%08x"


#define 		PERIOD        '.'
#define 		PLUS        '+'
#define 		MINUS        '-'
#define 		OPAREN        '('
#define 		CPAREN        ')'
#define 		COMMA        ','
#define 		DOLLAR        '$'
#define 		PERCENT        '%'
#define 		HASH        '#'
#define 		COLON        ':'
#define 		SEMICOLON    ';'
#define 		ASTERIX        '*'
#define 		SLASH        '/'
#define 		UNDERSCORE    '_'
#define 		QUOTE        '\''
#define 		QUOTES        '"'
#define 		ESCAPE        '\\'
#define 		AMPERSAND    '&'
#define 		PIPE        '|'
#define 		HAT        '^'
#define 		BANG        '!'

typedef enum CONDITION
{
    CONDITION_TRUE,
	CONDITION_FALSE,
	CONDITION_HIGHER,
	CONDITION_LOWER_OR_SAME,
	CONDITION_CARRY_CLEAR,
	CONDITION_CARRY_SET,
	CONDITION_NOT_EQUAL,
	CONDITION_EQUAL,
	CONDITION_OVERFLOW_CLEAR,
	CONDITION_OVERFLOW_SET,
	CONDITION_PLUS,
	CONDITION_MINUS,
	CONDITION_GREATER_OR_EQUAL,
	CONDITION_LESS_THAN,
	CONDITION_GREATER_THAN,
	CONDITION_LESS_OR_EQUAL

} CONDITION;

typedef enum OPCODE_TYPE
{
    OPCODE_ORI_TO_CCR,
	OPCODE_ORI_TO_SR,
	OPCODE_ORI,
	OPCODE_ANDI_TO_CCR,
	OPCODE_ANDI_TO_SR,
	OPCODE_ANDI,
	OPCODE_SUBI,
	OPCODE_ADDI,
	OPCODE_EORI_TO_CCR,
	OPCODE_EORI_TO_SR,
	OPCODE_EORI,
	OPCODE_CMPI,
	OPCODE_BTST_STATIC,
	OPCODE_BCHG_STATIC,
	OPCODE_BCLR_STATIC,
	OPCODE_BSET_STATIC,
	OPCODE_BTST_DYNAMIC,
	OPCODE_BCHG_DYNAMIC,
	OPCODE_BCLR_DYNAMIC,
	OPCODE_BSET_DYNAMIC,
	OPCODE_MOVEP_TO_REG,
	OPCODE_MOVEP_FROM_REG,
	OPCODE_MOVEA,
	OPCODE_MOVE,
	OPCODE_MOVE_FROM_SR,
	OPCODE_MOVE_TO_CCR,
	OPCODE_MOVE_TO_SR,
	OPCODE_NEGX,
	OPCODE_CLR,
	OPCODE_NEG,
	OPCODE_NOT,
	OPCODE_EXT,
	OPCODE_NBCD,
	OPCODE_SWAP,
	OPCODE_PEA,
	OPCODE_ILLEGAL,
	OPCODE_TAS,
	OPCODE_TST,
	OPCODE_TRAP,
	OPCODE_LINK,
	OPCODE_UNLK,
	OPCODE_MOVE_TO_USP,
	OPCODE_MOVE_FROM_USP,
	OPCODE_RESET,
	OPCODE_NOP,
	OPCODE_STOP,
	OPCODE_RTE,
	OPCODE_RTS,
	OPCODE_TRAPV,
	OPCODE_RTR,
	OPCODE_JSR,
	OPCODE_JMP,
	OPCODE_MOVEM_TO_REGS,
	OPCODE_MOVEM_FROM_REGS,
	OPCODE_LEA,
	OPCODE_CHK,
	OPCODE_ADDQ,
	OPCODE_SUBQ,
	OPCODE_Scc,
	OPCODE_DBcc,
	OPCODE_BRA,
	OPCODE_BSR,
	OPCODE_Bcc,
	OPCODE_MOVEQ,
	OPCODE_DIVU,
	OPCODE_DIVS,
	OPCODE_SBCD_DATA_REGS,
	OPCODE_SBCD_ADDRESS_REGS,
	OPCODE_OR_TO_REG,
	OPCODE_OR_FROM_REG,
	OPCODE_SUB_TO_REG,
	OPCODE_SUB_FROM_REG,
	OPCODE_SUBX_DATA_REGS,
	OPCODE_SUBX_ADDRESS_REGS,
	OPCODE_SUBA,
	OPCODE_EOR,
	OPCODE_CMPM,
	OPCODE_CMP,
	OPCODE_CMPA,
	OPCODE_MULU,
	OPCODE_MULS,
	OPCODE_ABCD_DATA_REGS,
	OPCODE_ABCD_ADDRESS_REGS,
	OPCODE_EXG,
	OPCODE_AND_TO_REG,
	OPCODE_AND_FROM_REG,
	OPCODE_ADD_TO_REG,
	OPCODE_ADD_FROM_REG,
	OPCODE_ADDX_DATA_REGS,
	OPCODE_ADDX_ADDRESS_REGS,
	OPCODE_ADDA,
	OPCODE_ASL_STATIC,
	OPCODE_ASR_STATIC,
	OPCODE_LSL_STATIC,
	OPCODE_LSR_STATIC,
	OPCODE_ROXL_STATIC,
	OPCODE_ROXR_STATIC,
	OPCODE_ROL_STATIC,
	OPCODE_ROR_STATIC,
	OPCODE_ASL_DYNAMIC,
	OPCODE_ASR_DYNAMIC,
	OPCODE_LSL_DYNAMIC,
	OPCODE_LSR_DYNAMIC,
	OPCODE_ROXL_DYNAMIC,
	OPCODE_ROXR_DYNAMIC,
	OPCODE_ROL_DYNAMIC,
	OPCODE_ROR_DYNAMIC,
	OPCODE_ASL_SINGLE,
	OPCODE_ASR_SINGLE,
	OPCODE_LSL_SINGLE,
	OPCODE_LSR_SINGLE,
	OPCODE_ROXL_SINGLE,
	OPCODE_ROXR_SINGLE,
	OPCODE_ROL_SINGLE,
	OPCODE_ROR_SINGLE

} OPCODE_TYPE;

#define			OPERAND_NONE																	 0
#define			OPERAND_DATA_REGISTER														1 << 0
#define			OPERAND_ADDRESS_REGISTER													1 << 1
#define			OPERAND_ADDRESS_REGISTER_IND												1 << 2
#define			OPERAND_ADDRESS_REGISTER_IND_POSTINCREMENT									1 << 3
#define			OPERAND_ADDRESS_REGISTER_IND_PREDECREMENT									1 << 4
#define			OPERAND_ADDRESS_REGISTER_IND_W_DISP											1 << 5
#define			OPERAND_ADDRESS_REGISTER_IND_W_DISP_INDEX_REG								1 << 6
#define			OPERAND_ADDRESS_BASE														1 << 7
#define			OPERAND_ADDRESS_ABSOLUTE													1 << 8
#define			OPERAND_LITERAL																1 << 9
#define			OPERAND_PC_W_DISP															1 << 10
#define			OPERAND_PC_W_DISP_INDEX_REG													1 << 11
#define			OPERAND_STATUS_REGISTER_BASE												1 << 12
#define			OPERAND_CONDITION_CODE_REG_BASE												1 << 13
#define			OPERAND_USER_STACK_POINTER_REG_BASE											1 << 14
#define			OPERAND_REGISTER_LIST														1 << 15	

typedef enum EXPRESSION_TYPE
{
    EXPRESSION_SUBTRACT,
	EXPRESSION_ADD,
	EXPRESSION_MULTIPLY,
	EXPRESSION_DIVIDE,
	EXPRESSION_MODULO,
	EXPRESSION_NEGATE,
	EXPRESSION_LOGICAL_NOT,
	EXPRESSION_LOGICAL_OR,
	EXPRESSION_LOGICAL_AND,
	EXPRESSION_BITWISE_NOT,
	EXPRESSION_BITWISE_OR,
	EXPRESSION_BITWISE_XOR,
	EXPRESSION_BITWISE_AND,
	EXPRESSION_EQUALITY,
	EXPRESSION_INEQUALITY,
	EXPRESSION_LESS_THAN,
	EXPRESSION_LESS_OR_EQUAL,
	EXPRESSION_MORE_THAN,
	EXPRESSION_MORE_OR_EQUAL,
	EXPRESSION_LEFT_SHIFT,
	EXPRESSION_RIGHT_SHIFT,
	EXPRESSION_NUMBER,
	EXPRESSION_IDENTIFIER,
	EXPRESSION_STRING,
	EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT,
	EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION,
	EXPRESSION_STRLEN,
	EXPRESSION_DEF

} EXPRESSION_TYPE;


typedef enum EA_MODE
{
	EA_DREG,
	EA_AREG,
	EA_AREG_IND,
	EA_AREG_IND_INC,
	EA_AREG_IND_DEC,
	EA_AREG_IND_DISP,
	EA_AREG_IND_DISP_REG,
	EA_ABS_SHORT_IND,
	EA_ABS_LONG_IND,
	EA_PC_IND_DISP,
	EA_PC_IND_DISP_REG,
	EA_IMM,
	EA_STAT_REG,
	EA_FLAGS,

	NUM_DREG,
	NUM_AREG,
	NUM_AREG_IND_INC
	
} EA_MODE;

typedef enum DIRECTIVES
{
	NONE = 0,
	ERROR,
	IDENTIFIER,
	SYM_OPCODE,
	ADDRESS_REG,
	DATA_REG, 
	FPU_REG,
	REG_LIST,
	NUMBER,
	REAL_FPNUM,
	CHAR,
	STRING,
	EXP,

	PC,
	SR,
	CCR,
	USP,
	VBR,
	SFC,
	DFC,

	BYTE,
	WORD,
	LONG,
	TEXT,
	DATA,
	BSS,
	ORG,
	START,
	ALIGN,
	EQU,
	END,
	DC, DS,

	MUL,
	DIV,
	MOD,
	AND,
	OR,
	EOR,
	NOT,
	ADDRESS

} DIRECTIVES;

#endif
#endif
#endif
