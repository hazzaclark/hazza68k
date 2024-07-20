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

			INCLUDE	"macros.asm"

ROM_START:

	DC.B				"SEGA MEGA DRIVE "
	DC.B				"HARRY CLARK "
	DC.B				"HELLO WORLD SAMPLE "
	DC.B				"HELLO WORLD SAMPLE "
	DC.B				"GM XXXXXX-XX "
	DC.W				0x0000
	DC.B				"J					"
	DC.L				ROM_START
	DC.L				ROM_END-1
	DC.L				0x00FF0000
	DC.L				0x00FF0000+0x0000FFF
	DC.L				0x00000000
	DC.L				0x00000000
	DC.L				0x00000000
	DC.L				0x00000000
	DC.L				0x00000000
	DC.L				0x00000000
	DC.B				"JUE				"


CPU_INIT:
	JSR			WRITE_TMSS
	JSR			WRITE_VDP

	SET_VRAM_WRITE 		0x0000

	MOVE.W		#(0x00010000/2)-1, D0
	
	@CLEAR_VRAM:
	MOVE.W		#0x0, VDP_DATA
	DBRA		D0, @CLEAR_VRAM

	MOVE.W		#0x2300, SR

	SET_CRAM_WRITE 		0x0000

WRITE_TMSS:

	MOVE.B		SEGA_TMSS_VER, D0
	ANDI.B		#0x0F, D0
	BEQ			@SKIP_TMSS
	MOVE.L		#SEGA_TMSS_SIG, SEGA_TMSS_ADDR

	@SKIP_TMSS:
	MOVE.W		VDP_CTRL, D0
	RTS 		 

WRITE_VDP:

	LEA			VDP_REGISTERS, A0
	MOVE.W 		#0x18-1, D0
	MOVE.W		#0x8000, D1

	RTS


ROM_END:
