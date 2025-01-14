/* COPYRIGHT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 ASSEMBLER */

/* THIS FILE HOUSES THE NECESSITIES FOR THE ASSEMBLY PROCESS */
/* FOCUSSING ON THE BACKEND OF THE ASSEMBLER */

#ifndef DISASSEMBLY
#define DISASSEMBLY

/* NESTED INCLUDES */

#include "assemble.h"
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

#define     SIZE_BYTE       0x01
#define     SIZE_SHORT      0x02
#define     SIZE_WORD       0x03
#define     SIZE_LONG       0x04
#define     SIZE_UNDEF      0x00

#define SIZE_WL   (SIZE_WORD | SIZE_LONG)
#define SIZE_BWL  (SIZE_BYTE | SIZE_WORD | SIZE_LONG)

#define		MAX_CHAR		32
#define		MAX_BIT_ARGS	256

#define NO_ARGUMENT             0x000000
#define EA_DREG                 0x000001
#define EA_AREG                 0x000002
#define EA_AREG_IND            0x000004
#define EA_AREG_IND_INC        0x000008
#define EA_AREG_DEC_IND        0x000010
#define EA_AREG_IND_DISP       0x000020
#define EA_AREG_IND_DISP_REG   0x000040
#define EA_ABS_SHORT_IND       0x000080
#define EA_ABS_LONG_IND        0x000100
#define EA_PC_IND_DISP         0x000200
#define EA_PC_IND_DISP_REG     0x000400
#define EA_IMMEDIATE           0x000800
#define EA_STATUS_REG          0x001000
#define EA_FLAGS_REG           0x002000
#define REGISTER_LIST          0x004000
#define ABS_ADDRESS            0x008000
#define UNUSED                 0x010000
#define NUM_DREG               0x020000
#define NUM_AREG               0x040000
#define NUM_AREG_DEC_IND       0x080000
#define NUM_AREG_IND_INC       0x100000
#define NUM_IMMEDIATE_3BIT     0x200000
#define NUM_IMMEDIATE_QUICK    0x400000
#define NUM_IMMEDIATE_5BIT     0x800000
#define NUM_IMMEDIATE_4        0x1000000
#define NUM_IMMEDIATE_8        0x2000000
#define NUM_IMMEDIATE_16       0x4000000
#define CONTROL_REG            0x8000000

#define EA_SOURCE_ARG	(EA_DREG|EA_AREG|EA_AREG_IND|EA_AREG_IND_INC|EA_AREG_DEC_IND|EA_AREG_IND_DISP|EA_AREG_IND_DISP_REG|EA_ABS_SHORT_IND|EA_ABS_LONG_IND|EA_PC_IND_DISP|EA_PC_IND_DISP_REG|EA_IMMEDIATE)
#define EA_DEST_ARG		(EA_DREG|EA_AREG_IND|EA_AREG_IND_INC|EA_AREG_DEC_IND|EA_AREG_IND_DISP|EA_AREG_IND_DISP_REG|EA_ABS_SHORT_IND|EA_ABS_LONG_IND)
#define EA_MEMORY_ARG	(EA_AREG_IND|EA_AREG_IND_INC|EA_AREG_DEC_IND|EA_AREG_IND_DISP|EA_AREG_IND_DISP_REG|EA_ABS_SHORT_IND|EA_ABS_LONG_IND)
#define EA_ADDRESS_ARG	(EA_AREG_IND|EA_AREG_IND_DISP|EA_AREG_IND_DISP_REG|EA_ABS_SHORT_IND|EA_ABS_LONG_IND|EA_PC_IND_DISP|EA_PC_IND_DISP_REG)

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
	EA_MODE_DREG,
	EA_MODE_AREG,
	EA_MODE_AREG_IND,
	EA_MODE_AREG_IND_INC,
	EA_MODE_AREG_IND_DEC,
	EA_MODE_AREG_IND_DISP,
	EA_MODE_AREG_IND_DISP_REG,
	EA_MODE_ABS_SHORT_IND,
	EA_MODE_ABS_LONG_IND,
	EA_MODE_PC_IND_DISP,
	EA_MODE_PC_IND_DISP_REG,
	EA_IMM,
	EA_STAT_REG,
	EA_FLAGS,

	EA_NUM_DREG,
	EA_NUM_AREG,
	EA_NUM_AREG_IND_INC
	
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

//===============================================================================
//						68K OPCODE AND MNENOMIC DEFS.
//
//		THE FOLLOWING IS A MASSIVE LIST PERTAINING TOWARDS EACH INDIVIDUAL
// 					OPCODE AND MNENOMIC DEF AND EXPRESSION - 
//					INSPIRED HEAVILY FROM ASM68K AND SNASM68K
//===============================================================================

static OPCODE OPCODE_ABCD = { SIZE_BYTE, "OPCODE_ABCD", NUM_AREG_DEC_IND | NUM_AREG_DEC_IND, 0xC108, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_ADD = { SIZE_BYTE, "OPCODE_ADD", NUM_DREG | NUM_DREG, 0xC100, 0x0000, 0, 0, &OPCODE_ABCD };
static OPCODE OPCODE_ADDI = { SIZE_BWL, "OPCODE_ADDI", EA_SOURCE_ARG | NUM_DREG, 0xD000, 0x0000, 6, 0, NULL };
static OPCODE OPCODE_ADDA = { SIZE_BWL, "OPCODE_ADDA", NUM_DREG | EA_MEMORY_ARG, 0xD100, 0x0000, 6, 9, &OPCODE_ADDI };
static OPCODE OPCODE_ADDX = { SIZE_BWL, "OPCODE_ADDX", EA_IMMEDIATE | EA_DEST_ARG, 0x0600, 0x0000, 6, 0, NULL };
static OPCODE OPCODE_ANDI = { SIZE_WL, "OPCODE_ANDI", EA_SOURCE_ARG | NUM_AREG, 0xD0C0, 0x0000, 8, 0, NULL };
static OPCODE OPCODE_AND = { SIZE_BWL, "OPCODE_AND", NUM_AREG_DEC_IND | NUM_AREG_DEC_IND, 0xD108, 0x0000, 6, 9, NULL };
static OPCODE OPCODE_ADDQ = { SIZE_BWL, "OPCODE_ADDQ", NUM_IMMEDIATE_QUICK | EA_DEST_ARG | EA_AREG, 0x5000, 0x0000, 6, 9, NULL };
static OPCODE OPCODE_ASR = { SIZE_BWL, "OPCODE_ASR", NUM_IMMEDIATE_QUICK | NUM_DREG, 0xE000, 0x0000, 6, 9, &OPCODE_AND };
static OPCODE OPCODE_BCHG = { SIZE_LONG, "OPCODE_BCHG", NUM_DREG | EA_DREG, 0x0140, 0x0000, 0, 9, NULL };
static OPCODE OPCODE_BCLR = { SIZE_BYTE, "OPCODE_BCLR", NUM_IMMEDIATE_3BIT | EA_MEMORY_ARG, 0x0840, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_BSET = { SIZE_LONG, "OPCODE_BSET", NUM_IMMEDIATE_5BIT | EA_DREG, 0x0840, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_BRA = { SIZE_UNDEF, "OPCODE_BRA", ABS_ADDRESS | NO_ARGUMENT, 0x6000, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_BSR = { SIZE_UNDEF, "OPCODE_BSR", ABS_ADDRESS | NO_ARGUMENT, 0x6100, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_BNE = { SIZE_UNDEF, "OPCODE_BNE", ABS_ADDRESS | NO_ARGUMENT, 0x6600, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_CLR = { SIZE_BWL, "OPCODE_CLR", EA_DEST_ARG | NO_ARGUMENT, 0x4200, 0x0000, 6, 0, NULL };
static OPCODE OPCODE_CMPI = { SIZE_BWL, "OPCODE_CMPI", EA_IMMEDIATE | EA_DEST_ARG, 0x0C00, 0x0000, 6, 0, NULL };
static OPCODE OPCODE_DIVS = { SIZE_WORD, "OPCODE_DIVS", EA_SOURCE_ARG | NUM_DREG, 0x81C0, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_DIVU = { SIZE_WORD, "OPCODE_DIVU", EA_SOURCE_ARG | NUM_DREG, 0x80C0, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_EXG = { SIZE_LONG, "OPCODE_EXG", NUM_DREG | NUM_DREG, 0xC140, 0x0000, 0, 9, NULL };
static OPCODE OPCODE_ILLEGAL = { SIZE_UNDEF, "OPCODE_ILLEGAL", NO_ARGUMENT | NO_ARGUMENT, 0x4AFC, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_JSR = { SIZE_UNDEF, "OPCODE_JSR", EA_ADDRESS_ARG | NO_ARGUMENT, 0x4E80, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_LEA = { SIZE_UNDEF, "OPCODE_LEA", EA_ADDRESS_ARG | NUM_AREG, 0x41C0, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_LINK = { SIZE_UNDEF, "OPCODE_LINK", NUM_AREG | NUM_IMMEDIATE_16, 0x4E50, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_MOVE = { SIZE_BWL, "OPCODE_MOVE", EA_SOURCE_ARG | EA_DEST_ARG, 0x0000, 0x0000, 12, 0, NULL };
static OPCODE OPCODE_MOVEC = { SIZE_LONG, "OPCODE_MOVEC", CONTROL_REG | NUM_DREG, 0x4E7A, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_MOVES = { SIZE_BWL, "OPCODE_MOVES", EA_MEMORY_ARG | NUM_DREG, 0x0E00, 0x0000, 6, 0, NULL };
static OPCODE OPCODE_MOVEM = { SIZE_WL, "OPCODE_MOVEM", REGISTER_LIST | EA_AREG_DEC_IND, 0x4880, 0x0000, 6, 0, NULL };
static OPCODE OPCODE_MOVEA = { SIZE_WL, "OPCODE_MOVEA", EA_SOURCE_ARG | NUM_AREG, 0x2040, 0x0000, 12, 0, NULL };
static OPCODE OPCODE_MOVEQ = { SIZE_LONG, "OPCODE_MOVEQ", NUM_IMMEDIATE_8 | NUM_DREG, 0x7000, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_MULU = { SIZE_WORD, "OPCODE_MULU", EA_SOURCE_ARG | NUM_DREG, 0xC0C0, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_NBC = { SIZE_BYTE, "OPCODE_NBC", EA_DEST_ARG | NO_ARGUMENT, 0x4800, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_NEG = { SIZE_BWL, "OPCODE_NEG", EA_DEST_ARG | NO_ARGUMENT, 0x4400, 0x0000, 6, 0, NULL };
static OPCODE OPCODE_NOP = { SIZE_UNDEF, "OPCODE_NOP", NO_ARGUMENT | NO_ARGUMENT, 0x4E71, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_OR = { SIZE_BWL, "OPCODE_OR", EA_SOURCE_ARG | NUM_DREG, 0x8000, 0x0000, 6, 0, NULL };
static OPCODE OPCODE_ROL = { SIZE_BWL, "OPCODE_ROL", NUM_DREG | NUM_DREG, 0xE138, 0x0000, 6, 9, NULL };
static OPCODE OPCODE_RTS = { SIZE_UNDEF, "OPCODE_RTS", NO_ARGUMENT | NO_ARGUMENT, 0x4E75, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_STOP = { SIZE_UNDEF, "OPCODE_STOP", NO_ARGUMENT | NO_ARGUMENT, 0x4E72, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_SUB = { SIZE_BWL, "OPCODE_SUB", NUM_DREG | EA_MEMORY_ARG, 0x9100, 0x0000, 6, 9, NULL };
static OPCODE OPCODE_TAS = { SIZE_BYTE, "OPCODE_TAS", EA_DEST_ARG | NO_ARGUMENT, 0x4AC0, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_TRAP = { SIZE_UNDEF, "OPCODE_TRAP", NUM_IMMEDIATE_4 | NO_ARGUMENT, 0x4E40, 0x0000, 0, 0, NULL };
static OPCODE OPCODE_TST = { SIZE_BWL, "OPCODE_TST", EA_DEST_ARG | NO_ARGUMENT, 0x4A00, 0x0000, 6, 0, NULL };
static OPCODE OPCODE_UNLK = { SIZE_UNDEF, "OPCODE_UNLK", NUM_AREG | NO_ARGUMENT, 0x4E58, 0x0000, 0, 0, NULL };
#define ROOT_NODE OPCODE_MOVE


#endif
#endif
#endif
