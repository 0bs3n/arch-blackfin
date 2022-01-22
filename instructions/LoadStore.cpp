#include "LoadStore.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool 
LoadStoreInstruction::Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel) 
{
	/* LDST
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 0 | 0 | 1 |.sz....|.W.|.aop...|.Z.|.ptr.......|.reg.......|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int Z   = ((instructionWord >> LDST_Z_bits) & LDST_Z_mask);
	int W   = ((instructionWord >> LDST_W_bits) & LDST_W_mask);
	int sz  = ((instructionWord >> LDST_sz_bits) & LDST_sz_mask);
	int aop = ((instructionWord >> LDST_aop_bits) & LDST_aop_mask);
	int reg = ((instructionWord >> LDST_reg_bits) & LDST_reg_mask);
	int ptr = ((instructionWord >> LDST_ptr_bits) & LDST_ptr_mask);

    // FIXME: terrible repeated code here, should be refactored to
    // more nicely parse the various opcode parameters

    instr.operand_count = 3;
    if (!W) instr.operation = OP_LD;
    else instr.operation = OP_ST;

	if (aop == 0 && sz == 0 && Z == 0 && W == 0)
	{
        // dreg = [preg ++ ]
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_PLUSPLUS,
            .width = 4
        }};
	}
	else if (aop == 0 && sz == 0 && Z == 1 && W == 0 && reg != ptr)
	{
        // preg = [preg ++ ]
        instr.operands[0] = { .cls = REG, .reg = pregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_PLUSPLUS,
            .width = 4
        }};
	}
	else if (aop == 0 && sz == 1 && Z == 0 && W == 0)
	{
        // dreg = W[preg ++ ] (Z)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_PLUSPLUS,
            .width = 2
        }};
        instr.operands[2].flags.zero_extended = true;
	}
	else if (aop == 0 && sz == 1 && Z == 1 && W == 0)
	{
        // dreg = W[preg ++ ] (X)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_PLUSPLUS,
            .width = 2
        }};
        instr.operands[2].flags.sign_extended = true;
	}
	else if (aop == 0 && sz == 2 && Z == 0 && W == 0)
	{
        // dreg = B[preg ++ ] (Z)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_PLUSPLUS,
            .width = 1
        }};
        instr.operands[2].flags.zero_extended = true;
	}
	else if (aop == 0 && sz == 2 && Z == 1 && W == 0)
	{
        // dreg = B[preg ++ ] (X)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_PLUSPLUS,
            .width = 1
        }};
        instr.operands[2].flags.sign_extended = true;
	}
	else if (aop == 1 && sz == 0 && Z == 0 && W == 0)
	{
        // dreg = [preg -- ]
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_MINUSMINUS,
            .width = 4
        }};
	}
	else if (aop == 1 && sz == 0 && Z == 1 && W == 0 && reg != ptr)
	{
        // preg = [preg -- ]
        instr.operands[0] = { .cls = REG, .reg = pregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_MINUSMINUS,
            .width = 4
        }};
	}
	else if (aop == 1 && sz == 1 && Z == 0 && W == 0)
	{
        // dreg = W[preg -- ] (Z)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_MINUSMINUS,
            .width = 2
        }};
        instr.operands[2].flags.zero_extended = true;
	}
	else if (aop == 1 && sz == 1 && Z == 1 && W == 0)
	{
        // dreg = W[preg -- ] (X)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_MINUSMINUS,
            .width = 2
        }};
        instr.operands[2].flags.sign_extended = true;
	}
	else if (aop == 1 && sz == 2 && Z == 0 && W == 0)
	{
        // dreg = B[preg -- ] (Z)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_MINUSMINUS,
            .width = 1
        }};
        instr.operands[2].flags.zero_extended = true;
	}
	else if (aop == 1 && sz == 2 && Z == 1 && W == 0)
	{
        // dreg = B[preg -- ] (X)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_MINUSMINUS,
            .width = 1
        }};
        instr.operands[2].flags.sign_extended = true;
	}
	else if (aop == 2 && sz == 0 && Z == 0 && W == 0)
	{
        // dreg = [preg]
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REG,
            .ptr_reg = pregs(ptr),
            .width = 4
        }};
	}
	else if (aop == 2 && sz == 0 && Z == 1 && W == 0)
	{
        // preg = [preg]
        instr.operands[0] = { .cls = REG, .reg = pregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REG,
            .ptr_reg = pregs(ptr),
            .width = 4
        }};
	}
	else if (aop == 2 && sz == 1 && Z == 0 && W == 0)
	{
        //dreg = W[preg] (Z)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REG,
            .ptr_reg = pregs(ptr),
            .width = 2
        }};
        instr.operands[2].flags.zero_extended = true;
	}
	else if (aop == 2 && sz == 1 && Z == 1 && W == 0)
	{
        // dreg = W[preg] (X)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REG,
            .ptr_reg = pregs(ptr),
            .width = 2
        }};
        instr.operands[2].flags.sign_extended = true;
	}
	else if (aop == 2 && sz == 2 && Z == 0 && W == 0)
	{
        // dreg = B[preg] (Z)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REG,
            .ptr_reg = pregs(ptr),
            .width = 1
        }};
        instr.operands[2].flags.zero_extended = true;
	}
	else if (aop == 2 && sz == 2 && Z == 1 && W == 0)
	{
        // dreg = B[preg] (X)
        instr.operands[0] = { .cls = REG, .reg = dregs(reg) };
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REG,
            .ptr_reg = pregs(ptr),
            .width = 1
        }};
        instr.operands[2].flags.sign_extended = true;
	}
	else if (aop == 0 && sz == 0 && Z == 0 && W == 1)
	{
        // [preg ++] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_PLUSPLUS,
            .width = 4
        }};
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = REG, .reg = dregs(reg) };
	}
	else if (aop == 0 && sz == 0 && Z == 1 && W == 1)
	{
        // [preg ++] = preg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_PLUSPLUS,
            .width = 4
        }};
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = REG, .reg = pregs(reg) };
	}
	else if (aop == 0 && sz == 1 && Z == 0 && W == 1)
	{
        // W[preg ++] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_PLUSPLUS,
            .width = 2
        }};
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = REG, .reg = dregs(reg) };
	}
	else if (aop == 0 && sz == 2 && Z == 0 && W == 1)
	{
        // B[preg ++] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_PLUSPLUS,
            .width = 1
        }};
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = REG, .reg = dregs(reg) };
	}
	else if (aop == 1 && sz == 0 && Z == 0 && W == 1)
	{
        // [preg --] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_MINUSMINUS,
            .width = 4
        }};
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = REG, .reg = dregs(reg) };
	}
	else if (aop == 1 && sz == 0 && Z == 1 && W == 1)
	{
        // [preg --] = preg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_MINUSMINUS,
            .width = 4
        }};
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = REG, .reg = pregs(reg) };
	}
	else if (aop == 1 && sz == 1 && Z == 0 && W == 1)
	{
        // W[preg --] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_MINUSMINUS,
            .width = 2
        }};
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = REG, .reg = dregs(reg) };
	}
	else if (aop == 1 && sz == 2 && Z == 0 && W == 1)
	{
        // B[preg --] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGIMM,
            .ptr_reg = pregs(ptr),
            .idx_imm = 1,
            .oper = OPER_MINUSMINUS,
            .width = 1
        }};
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = REG, .reg = dregs(reg) };
	}
	else if (aop == 2 && sz == 0 && Z == 0 && W == 1)
	{
        // [preg] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REG,
            .ptr_reg = pregs(ptr),
            .width = 4
        }};
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = REG, .reg = dregs(reg) };
	}
	else if (aop == 2 && sz == 0 && Z == 1 && W == 1)
	{
        // [preg] = preg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REG,
            .ptr_reg = pregs(ptr),
            .width = 4
        }};
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = REG, .reg = pregs(reg) };
	}
	else if (aop == 2 && sz == 1 && Z == 0 && W == 1)
	{
        // W[preg] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REG,
            .ptr_reg = pregs(ptr),
            .width = 2
        }};
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = REG, .reg = dregs(reg) };
	}
	else if (aop == 2 && sz == 2 && Z == 0 && W == 1)
	{
        // B[preg] = dreg
        instr.operands[0] = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REG,
            .ptr_reg = pregs(ptr),
            .width = 1
        }};
        instr.operands[1] = { .cls = OPERATOR, .operat = OPER_EQ };
        instr.operands[2] = { .cls = REG, .reg = dregs(reg) };
	}
	else
		return false;

	return true;
}
