#include "Instructions.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool 
BlackfinInstruction::DisassembleLoadStoreFPOffset(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel) 
{
	/* LDSTiiFP
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 0 | 1 | 1 | 1 | 0 |.W.|.offset............|.reg...........|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int reg = ((instructionWord >> LDSTiiFP_reg_bits) & LDSTiiFP_reg_mask);
	int offset = ((instructionWord >> LDSTiiFP_offset_bits) & LDSTiiFP_offset_mask);
	int W = ((instructionWord >> LDSTiiFP_W_bits) & LDSTiiFP_W_mask);

    instr.operand_count = 3;
    instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
	if (W == 0)
	{
        // reg = [FP - offset]
        instr.operation = OP_LD;
        instr.operands[0] = { .cls = REG, .reg = dpregs(reg) };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGOFF,
            .ptr_reg = REG_FP,
            .offset = negimm5s4(offset),
            .oper = OPER_PLUS,
            .width = 4
        }};
	}
	else if (W == 1)
	{
        // [FP - offset] = reg
        instr.operation = OP_ST;
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGOFF,
            .ptr_reg = REG_FP,
            .offset = negimm5s4(offset),
            .oper = OPER_PLUS,
            .width = 4
        }};
        instr.operands[2] = { .cls = REG, .reg = dpregs(reg) };
	}
	else
		return 0;

	return 2; 
}
