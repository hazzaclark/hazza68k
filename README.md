# 68K_ASM
My memory efficient Motorola 68000 Assembler 

# Motive:

This project serves to prove an ease-of-use, memory efficient Assembler designed specifically for Motorola 68000 Assembly.

The ease-of-use will take affect through the surrogate means of using the Command Line to build the Assembler as well
as Assemble your Code - as it will automate the process governed by Makefiles and the such

# Usage:

``git clone https://github.com/hazzaaclark/68K_ASM.git``

``cd 68K_ASM``

``sudo make``

# Notes:

Dynamic Memory Allocation is at the fore-front of this entire project, hence why the dominating language is C

I wanted to be able to create such a tool whereby it takes into account the realtime logistics of compiling 68K Assembly code
and the best way I knew how to take that into considering was to achieve a pseudo "cycle-accurate" representation.

This will ensure that I am able to cater towards the memory-related operations that govern the compilation time.

Furthermore, what with the aforementioned ease-of-use, I wanted to ensure that runtime performance is of the upmost importance
hence the prior mention of prioritising cycle-accurate timings were of the upmost essence

# Sources and Citations:

68K USER MANUAL: https://www.nxp.com/docs/en/reference-manual/MC68000UM.pdf

GENERAL 68K ASSEMBLY PROGRAMMING: https://www.inf.pucrs.br/~calazans/undergrad/orgcomp_EC/mat_microproc/MC6800-AssemblyLProg.pdf
