#include "Instructions.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool 
BlackfinInstruction::DisassembleLinkage(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel) 
{
	/* linkage
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 1 | 1 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |.R.|
	|.framesize.....................................................|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int R = ((instructionWordHigh >> (Linkage_R_bits - 16)) & Linkage_R_mask);
	int framesize = ((instructionWordLow >> Linkage_framesize_bits) & Linkage_framesize_mask);

	if (parallel) return false;

	if (R == 0) {
        instr.operation = OP_LINK;
        instr.operand_count = 2;
        instr.operands[0] = { .cls = MNEMOMIC, .mnemonic = OL_LINK };
        instr.operands[1] = { .cls = IMM, .imm = uimm16s4(framesize) };
	} else if (R == 1) {
        instr.operation = OP_UNLINK;
        instr.operand_count = 1;
        instr.operands[0] = { .cls = MNEMOMIC, .mnemonic = OL_UNLINK };
    } else
		return false;

	return true;
}
