#include "CCMove.h"
#include "arch_blackfin.h"

using namespace blackfin;

bool CCMoveInstruction::Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel) {
	int s  = ((instructionWord >> CCmv_s_bits) & CCmv_s_mask);
	int d  = ((instructionWord >> CCmv_d_bits) & CCmv_d_mask);
	int T  = ((instructionWord >> CCmv_T_bits) & CCmv_T_mask);
	int src = ((instructionWord >> CCmv_src_bits) & CCmv_src_mask);
	int dst = ((instructionWord >> CCmv_dst_bits) & CCmv_dst_mask);

    if (parallel) return false;

    instr.operation = blackfin::OP_CCMOVE;
    instr.operands[0] = { .cls = MNEMOMIC, .mnemonic = OL_IF };
    instr.operands[1].cls = blackfin::SREG;
    instr.operands[1].reg = REG_CC;

    // CC direction, "CC" if 1, "!CC" if 0
    if (T) instr.operands[1].flags.cc_inverted = false;
    else instr.operands[1].flags.cc_inverted = true;

    instr.operands[2].cls = blackfin::REG;
    instr.operands[2].reg = gregs(dst, d);
    instr.operands[3].cls = blackfin::OPERATOR;
    instr.operands[3].operat = blackfin::OPER_EQ;
    instr.operands[4].cls = blackfin::REG;
    instr.operands[4].reg = gregs(src, s);
    instr.operand_count = 5;
    return true;
}
