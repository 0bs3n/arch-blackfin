#include "arch_blackfin.h"

class Comp3OpInstruction {
	/* COMP3op
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 0 | 1 | 0 | 1 |.opc.......|.dst.......|.src1......|.src0......|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
    public:
    static bool Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
};
