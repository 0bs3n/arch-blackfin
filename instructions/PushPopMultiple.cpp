#include "PushPopMultiple.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool 
PushPopMultipleInstruction::Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel) 
{
	int p  = ((instructionWord >> PushPopMultiple_p_bits) & PushPopMultiple_p_mask);
	int d  = ((instructionWord >> PushPopMultiple_d_bits) & PushPopMultiple_d_mask);
	int W  = ((instructionWord >> PushPopMultiple_W_bits) & PushPopMultiple_W_mask);
	int dr = ((instructionWord >> PushPopMultiple_dr_bits) & PushPopMultiple_dr_mask);
	int pr = ((instructionWord >> PushPopMultiple_pr_bits) & PushPopMultiple_pr_mask);

    if (parallel) return false;
    if (pr > 5) return false;

    blackfin::InstructionOperand stack_op;
    blackfin::InstructionOperand range;
    
    instr.operands[1].cls = blackfin::OPERATOR;
    instr.operands[1].operat = blackfin::OPER_EQ;
    instr.operand_count = 3;

    // FIXME: why are these flags being set?
    range.flags.sign_extended = false;
    range.flags.zero_extended = false;

    if (d && !p) {
        if (pr != 0) return false;
        range.cls = blackfin::REG_RANGE;
        range.r_r.top = REG_R7;
        range.r_r.bottom = dregs(imm5d(dr));
    } else if (p && !d) {
        if (dr != 0) return false;
        range.cls = blackfin::REG_RANGE;
        range.r_r.top = REG_P5;
        range.r_r.bottom = pregs(imm5d(pr));
    } else if (p && d) {
        range.cls = blackfin::REG_RANGE_PAIR;
        range.r_r.top = REG_R7;
        range.r_p.top = REG_P5;
        range.r_r.bottom = dregs(imm5d(dr));
        range.r_p.bottom = pregs(imm5d(pr));
    }

    if (W) { // PUSH
        stack_op = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGMOD,
            .ptr_reg = REG_SP,
            .pre_dec = true
        }};
        instr.operands[0] = stack_op;
        instr.operands[2] = range;
        instr.operation = blackfin::OP_ST;
        return true;
    } else { // POP
        stack_op = { .cls = MEM_ACCESS, .mem_access = {
            .mode = MEM_REGMOD,
            .ptr_reg = REG_SP,
            .post_inc = true
        }};
        instr.operands[0] = range;
        instr.operands[2] = stack_op;
        instr.operation = blackfin::OP_LD;
        return true;
    }
}
