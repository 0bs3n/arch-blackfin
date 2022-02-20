#include "Instructions.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool 
BlackfinInstruction::DisassembleJumpCallLong(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel)
{
	/* CALLa
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 1 | 1 | 0 | 0 | 0 | 1 |.S.|.msw...........................|
	|.lsw...........................................................|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int S   = ((instructionWordHigh >> (CALLa_S_bits - 16)) & CALLa_S_mask);
	int lsw = ((instructionWordLow >> 0) & 0xffff);
	int msw = ((instructionWordHigh >> 0) & 0xff);

	if (parallel) return false;

    instr.operand_count = 2;
    instr.operands[1] = { .cls = IMM, .imm = pcrel24((msw << 16) | lsw) };
    instr.operands[1].flags.pcrelative = true;

    if (S) {
        instr.operation = OP_CALLREL;
        instr.operands[0] = { .cls = MNEMOMIC, .mnemonic = OL_CALL };
    }
    else {
        instr.operation = OP_JMPREL;
        instr.operands[0] = { .cls = MNEMOMIC, .mnemonic = OL_JUMPL };
    }

	return true;
}
