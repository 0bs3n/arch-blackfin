#include "LoadStoreOffset.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool 
LoadStoreOffsetInstruction::Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel) 
{
	/* LDSTii
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 0 | 1 |.W.|.op....|.offset........|.ptr.......|.reg.......|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int reg = ((instructionWord >> LDSTii_reg_bit) & LDSTii_reg_mask);
	int ptr = ((instructionWord >> LDSTii_ptr_bit) & LDSTii_ptr_mask);
	int offset = ((instructionWord >> LDSTii_offset_bit) & LDSTii_offset_mask);
	int op = ((instructionWord >> LDSTii_op_bit) & LDSTii_op_mask);
	int W = ((instructionWord >> LDSTii_W_bit) & LDSTii_W_mask);

    instr.operand_count = 3;
    instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
    if (!W) instr.operation = OP_LD;
    else instr.operation = OP_ST;

	if (W == 0 && op == 0)
	{
        // dreg = [preg + offset]
        instr.operands[0] = { .cls = REG, .reg = dregs(reg)};
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = uimm4s4(offset),
            .oper = OPER_PLUS,
            .width = 4
        }};
	}
	else if (W == 0 && op == 1)
	{
        // dreg = W[preg + offset] (Z)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg)};
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = uimm4s2(offset),
            .oper = OPER_PLUS,
            .width = 2
        }};
        instr.operands[2].flags.zero_extended = true;
	}
	else if (W == 0 && op == 2)
	{
        // dreg = W[preg + offset] (X)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg)};
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = uimm4s2(offset),
            .oper = OPER_PLUS,
            .width = 2
        }};
        instr.operands[2].flags.sign_extended = true;
	}
	else if (W == 0 && op == 3)
	{
        // preg = [preg + offset]
        instr.operands[0] = { .cls = REG, .reg = pregs(reg)};
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = uimm4s4(offset),
            .oper = OPER_PLUS,
            .width = 4
        }};
	}
	else if (W == 1 && op == 0)
	{
        // [preg + offset] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = uimm4s4(offset),
            .oper = OPER_PLUS,
            .width = 4
        }};
        instr.operands[2] = { .cls = REG, .reg = dregs(reg)};
	} 
	else if (W == 1 && op == 1)
	{
        // W[preg + offset] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = uimm4s2(offset),
            .oper = OPER_PLUS,
            .width = 2
        }};
        instr.operands[2] = { .cls = REG, .reg = dregs(reg)};
	}
	else if (W == 1 && op == 3)
	{
        // [preg + offset] = preg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = uimm4s4(offset),
            .oper = OPER_PLUS,
            .width = 4
        }};
        instr.operands[2] = { .cls = REG, .reg = pregs(reg)};
	} else 
        return false;

    return true;
}
