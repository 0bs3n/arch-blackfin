#include "PushPop.h"
#include "arch_blackfin.h"

bool PushPopInstruction::Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel) {
	int W   = ((instructionWord >> PushPopReg_W_bits) & PushPopReg_W_mask);
	int grp = ((instructionWord >> PushPopReg_grp_bits) & PushPopReg_grp_mask);
	int reg = ((instructionWord >> PushPopReg_reg_bits) & PushPopReg_reg_mask);
    
    if (parallel) return false;

    instr.operands[1].cls = blackfin::OPERATOR;
    instr.operands[1].operat = blackfin::OPER_EQ;
    instr.operand_count = 3;

    // POP
    if (!W && mostreg(reg, grp)) {
        instr.operands[0] = { .cls = blackfin::REG, .reg = allregs(reg, grp) };
        instr.operands[2] = { .cls = blackfin::MEM_ACCESS, .mem_access = {
            .mode = blackfin::MEM_REGMOD,
            .ptr_reg = REG_SP,
            .post_inc = true
        }};
        instr.operation = blackfin::OP_LD;

    // PUSH
    } else if (W && allreg(reg, grp) && !(grp == 1 && reg == 6)) {
        instr.operands[0] = { .cls = blackfin::MEM_ACCESS, .mem_access = {
            .mode = blackfin::MEM_REGMOD,
            .ptr_reg = REG_SP,
            .pre_dec = true
        }};
        instr.operands[2] = { .cls = blackfin::REG, .reg = allregs(reg, grp) };
        instr.operation = blackfin::OP_ST;
    } else {
        return false;
    }
    return true;
}
