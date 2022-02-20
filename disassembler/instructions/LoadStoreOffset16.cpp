#include "Instructions.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool 
BlackfinInstruction::DisassembleLoadStoreOffset16(uint16_t instructionWordHigh, uint16_t instructionWordLow, struct blackfin::Instruction &instr, bool parallel) 
{
	/* LDSTidxI
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 1 | 1 | 0 | 0 | 1 |.W.|.Z.|.sz....|.ptr.......|.reg.......|
	|.offset........................................................|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int Z = ((instructionWordHigh >> (LDSTidxI_Z_bits - 16)) & LDSTidxI_Z_mask);
	int W = ((instructionWordHigh >> (LDSTidxI_W_bits - 16)) & LDSTidxI_W_mask);
	int sz = ((instructionWordHigh >> (LDSTidxI_sz_bits - 16)) & LDSTidxI_sz_mask);
	int reg = ((instructionWordHigh >> (LDSTidxI_reg_bits - 16)) & LDSTidxI_reg_mask);
	int ptr = ((instructionWordHigh >> (LDSTidxI_ptr_bits - 16)) & LDSTidxI_ptr_mask);
	int offset = ((instructionWordLow >> LDSTidxI_offset_bits) & LDSTidxI_offset_mask);

    if (!W) instr.operation = OP_LD;
    else instr.operation = OP_ST;
    instr.operand_count = 3;
    instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };

	if (W == 0 && sz == 0 && Z == 0)
	{
        // dreg = [preg + offset]
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = imm16s4(offset),
            .oper = OPER_PLUS,
            .width = 4
        }};
	}
	else if (W == 0 && sz == 0 && Z == 1)
	{
        // preg = [preg + offset]
        instr.operands[0] = { .cls = REG, .reg = pregs(reg) };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = imm16s4(offset),
            .oper = OPER_PLUS,
            .width = 4
        }};
	}
	else if (W == 0 && sz == 1 && Z == 0)
	{
        // dreg = W[preg + offset] (Z)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = imm16s2(offset),
            .oper = OPER_PLUS,
            .width = 2
        }};
        instr.operands[2].flags.zero_extended = true;
	}
	else if (W == 0 && sz == 1 && Z == 1)
	{
        // dreg = W[preg + offset] (X)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = imm16s2(offset),
            .oper = OPER_PLUS,
            .width = 2
        }};
        instr.operands[2].flags.sign_extended = true;
	}
	else if (W == 0 && sz == 2 && Z == 0)
	{
        // dreg = B[preg + offset] (Z)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = imm16(offset),
            .oper = OPER_PLUS,
            .width = 1
        }};
        instr.operands[2].flags.zero_extended = true;
	}
	else if (W == 0 && sz == 2 && Z == 1)
	{
        // dreg = B[preg + offset] (X)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = imm16(offset),
            .oper = OPER_PLUS,
            .width = 1
        }};
        instr.operands[2].flags.sign_extended = true;
	}
	else if (W == 1 && sz == 0 && Z == 0)
	{
        // [preg + offset] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = imm16s4(offset),
            .oper = OPER_PLUS,
            .width = 4
        }};
        instr.operands[2] = { .cls = REG, .reg = dregs(reg) };
	}
	else if (W == 1 && sz == 0 && Z == 1)
	{
        // [preg + offset] = preg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = imm16s4(offset),
            .oper = OPER_PLUS,
            .width = 4
        }};
        instr.operands[2] = { .cls = REG, .reg = pregs(reg) };
	}
	else if (W == 1 && sz == 1 && Z == 0)
	{
        // W[preg + offset] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = imm16s2(offset),
            .oper = OPER_PLUS,
            .width = 2
        }};
        instr.operands[2] = { .cls = REG, .reg = dregs(reg) };
	}
	else if (W == 1 && sz == 2 && Z == 0)
	{
        // B[preg + offset] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGOFF,
            .ptr_reg = pregs(ptr),
            .offset = imm16(offset),
            .oper = OPER_PLUS,
            .width = 1
        }};
        instr.operands[2] = { .cls = REG, .reg = dregs(reg) };
	}
	else
		return false;

	return true;
}
