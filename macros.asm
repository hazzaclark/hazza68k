;--------------------------------------------------------
;           COPYRIGHT (C) HARRY CLARK 2024
;--------------------------------------------------------

;--------------------------------------------------------
;			MOTOROLA 68K ASSEMBLER SAMPLE FILE
;--------------------------------------------------------

;-------------------------------------------------------------
;	THIS FILE SERVES TO PROVIDE A BASE BY WHICH YOU ARE
;	ABLE TO USE MY HOMEMADE 68K ASSEMBLER TO CREATE BIN FILES
;
;	EXAMPLE OF WHICH IS THIS HELLO WORLD EXAMPLE FOR THE 
;					MEGA DRIVE/GENESIS 
;-------------------------------------------------------------


VDP_CTRL                EQU         0x00C00004
VDP_DATA                EQU         0x00C00000

VDP_CRAM_WRITE          EQU         0xC0000000
VDP_VRAM_WRITE          EQU         0x40000000

VRAM_ADDR_TILES         EQU         0x0000
VRAM_ADDR_PLANE_A       EQU         0xC000
VRAM_ADDR_PLANE_B       EQU         0xE000
VDP_PLANE_WIDTH         EQU         0x40
VDP_PLANE_HEIGHT        EQU         0x20

SEGA_TMSS_VER           EQU         0x00A10001
SEGA_TMSS_ADDR          EQU         0x0A14000
SEGA_TMSS_SIG           EQU         'SEGA'        

TEXT_POS_X              EQU         0x08
TEXT_POS_Y              EQU         0x04

SET_VRAM_WRITE:         MACRO       ADDR    
    MOVE.L              #(VDP_VRAM_WRITE)|((\ADDR)&$3FFF)<<16|(\ADDR)>>14, VDP_CTRL
    ENDM

SET_CRAM_WRITE:         MACRO       ADDR
    MOVE.L              #(VDP_CRAM_WRITE)|((\ADDR)&$3FFF)<<16|(\ADDR)>>14, VDP_CTRL
    ENDM

VDP_REGISTERS:
	DC.B 0x14
	DC.B 0x74
	DC.B 0x30
	DC.B 0x00 
	DC.B 0x07
	DC.B 0x78
	DC.B 0x00
	DC.B 0x00
	DC.B 0x00
	DC.B 0x00
	DC.B 0x08
	DC.B 0x00
	DC.B 0x81
	DC.B 0x3F 
	DC.B 0x00
	DC.B 0x02
	DC.B 0x01
	DC.B 0x00
	DC.B 0x00
	DC.B 0xFF
	DC.B 0xFF
	DC.B 0x00
	DC.B 0x00
	DC.B 0x80
	
	even

