#include "LoadStorePmod.h"
#include "arch_blackfin.h"

bool 
LoadStorePmodInstruction::Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel) 
{
	/* LDSTpmod
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 0 | 0 | 0 |.W.|.aop...|.reg.......|.idx.......|.ptr.......|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int W   = ((instructionWord >> LDSTpmod_W_bits) & LDSTpmod_W_mask);
	int aop = ((instructionWord >> LDSTpmod_aop_bits) & LDSTpmod_aop_mask);
	int idx = ((instructionWord >> LDSTpmod_idx_bits) & LDSTpmod_idx_mask);
	int ptr = ((instructionWord >> LDSTpmod_ptr_bits) & LDSTpmod_ptr_mask);
	int reg = ((instructionWord >> LDSTpmod_reg_bits) & LDSTpmod_reg_mask);


	if (aop == 1 && W == 0 && idx == ptr)
	{
        // reg_lo = W[ptr];
        instr.operand_count = 3;
        instr.operation = blackfin::OP_LD;
        instr.operands[0] = { .cls = blackfin::REG, .reg = dregs_lo(reg) };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2].cls = blackfin::MEM_ACCESS;
        instr.operands[2].mem_access = { .mode = blackfin::MEM_REG, .ptr_reg = pregs(ptr), .width = 2};
	}
	else if (aop == 2 && W == 0 && idx == ptr)
	{
        // reg_hi = W[ptr]
        instr.operand_count = 3;
        instr.operation = blackfin::OP_LD;
        instr.operands[0] = { .cls = blackfin::REG, .reg = dregs_hi(reg) };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2].cls = blackfin::MEM_ACCESS;
        instr.operands[2].mem_access = { .mode = blackfin::MEM_REG, .ptr_reg = pregs(ptr), .width = 2};
	}
	else if (aop == 1 && W == 1 && idx == ptr)
	{
        // W[ptr] = reg_lo
        instr.operand_count = 3;
        instr.operation = blackfin::OP_ST;
        instr.operands[0].cls = blackfin::MEM_ACCESS;
        instr.operands[0].mem_access = { .mode = blackfin::MEM_REG, .ptr_reg = pregs(ptr), .width = 2};
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2] = { .cls = blackfin::REG, .reg = dregs_lo(reg) };

	}
	else if (aop == 2 && W == 1 && idx == ptr)
	{
        // W[ptr] = reg_hi
        instr.operand_count = 3;
        instr.operation = blackfin::OP_ST;
        instr.operands[0].cls = blackfin::MEM_ACCESS;
        instr.operands[0].mem_access = { .mode = blackfin::MEM_REG, .ptr_reg = pregs(ptr), .width = 2};
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2] = { .cls = blackfin::REG, .reg = dregs_hi(reg) };
	}
	else if (aop == 0 && W == 0)
	{
        // reg = [ptr ++ idx]
        instr.operand_count = 5;
        instr.operation = blackfin::OP_LD;
        instr.operands[0] = { .cls = blackfin::REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2].cls = blackfin::MEM_ACCESS;
        instr.operands[2].mem_access = { 
            .mode = blackfin::MEM_REGREG, 
            .ptr_reg = pregs(ptr), 
            .idx_reg = pregs(idx),
            .oper = blackfin::OPER_PLUSPLUS,
            .width = 4
        };
	}
	else if (aop == 1 && W == 0)
	{
        // reg_lo = W[ptr ++ idx]
        instr.operand_count = 5;
        instr.operation = blackfin::OP_LD;
        instr.operands[0] = { .cls = blackfin::REG, .reg = dregs_lo(reg) };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2].cls = blackfin::MEM_ACCESS;
        instr.operands[2].mem_access = { 
            .mode = blackfin::MEM_REGREG, 
            .ptr_reg = pregs(ptr), 
            .idx_reg = pregs(idx),
            .oper = blackfin::OPER_PLUSPLUS,
            .width = 2
        };
	}
	else if (aop == 2 && W == 0)
	{
        // reg_hi = W[ptr ++ idx]
        instr.operand_count = 5;
        instr.operation = blackfin::OP_LD;
        instr.operands[0] = { .cls = blackfin::REG, .reg = dregs_hi(reg) };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2].cls = blackfin::MEM_ACCESS;
        instr.operands[2].mem_access = { 
            .mode = blackfin::MEM_REGREG, 
            .ptr_reg = pregs(ptr), 
            .idx_reg = pregs(idx),
            .oper = blackfin::OPER_PLUSPLUS,
            .width = 2
        };
	}
	else if (aop == 3 && W == 0)
	{
        // reg = W[ptr ++ idx] (Z)
        instr.operand_count = 5;
        instr.operation = blackfin::OP_LD;
        instr.operands[0] = { .cls = blackfin::REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2].cls = blackfin::MEM_ACCESS;
        instr.operands[2].flags.zero_extended = true;
        instr.operands[2].mem_access = { 
            .mode = blackfin::MEM_REGREG, 
            .ptr_reg = pregs(ptr), 
            .idx_reg = pregs(idx),
            .oper = blackfin::OPER_PLUSPLUS,
            .width = 2
        };
	}
	else if (aop == 3 && W == 1)
	{
        // reg = W[ptr ++ idx] (X)
        instr.operand_count = 5;
        instr.operation = blackfin::OP_LD;
        instr.operands[0] = { .cls = blackfin::REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2].cls = blackfin::MEM_ACCESS;
        instr.operands[2].flags.sign_extended = true;
        instr.operands[2].mem_access = { 
            .mode = blackfin::MEM_REGREG, 
            .ptr_reg = pregs(ptr), 
            .idx_reg = pregs(idx),
            .oper = blackfin::OPER_PLUSPLUS,
            .width = 2
        };
	}
	else if (aop == 0 && W == 1)
	{
        // [ptr ++ idx] = reg
        instr.operand_count = 3;
        instr.operation = blackfin::OP_ST;
        instr.operands[0].cls = blackfin::MEM_ACCESS;
        instr.operands[0].mem_access = { 
            .mode = blackfin::MEM_REGREG, 
            .ptr_reg = pregs(ptr), 
            .idx_reg = pregs(idx),
            .oper = blackfin::OPER_PLUSPLUS,
            .width = 4
        };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2] = { .cls = blackfin::REG, .reg = dregs(reg) };
	}
	else if (aop == 1 && W == 1)
	{
        // W[ptr ++ idx] = reg_lo
        instr.operand_count = 3;
        instr.operation = blackfin::OP_ST;
        instr.operands[0].cls = blackfin::MEM_ACCESS;
        instr.operands[0].mem_access = { 
            .mode = blackfin::MEM_REGREG, 
            .ptr_reg = pregs(ptr), 
            .idx_reg = pregs(idx),
            .oper = blackfin::OPER_PLUSPLUS,
            .width = 2
        };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2] = { .cls = blackfin::REG, .reg = dregs_lo(reg) };
	}
	else if (aop == 2 && W == 1)
	{
        // W[ptr ++ idx] = reg_hi
        instr.operand_count = 3;
        instr.operation = blackfin::OP_ST;
        instr.operands[0].cls = blackfin::MEM_ACCESS;
        instr.operands[0].mem_access = { 
            .mode = blackfin::MEM_REGREG, 
            .ptr_reg = pregs(ptr), 
            .idx_reg = pregs(idx),
            .oper = blackfin::OPER_PLUSPLUS,
            .width = 2
        };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2] = { .cls = blackfin::REG, .reg = dregs_hi(reg) };
	}
	else
		return false;

    return true;

}
