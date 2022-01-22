#include "CC2Dreg.h"

bool CC2DregInstruction::Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel)
{
	int op  = ((instructionWord >> CC2dreg_op_bits) & CC2dreg_op_mask);
	int reg = ((instructionWord >> CC2dreg_reg_bits) & CC2dreg_reg_mask);

    if (parallel) return false;

    instr.operands[0].cls = blackfin::REG;
    instr.operands[2].cls = blackfin::REG;

    instr.operands[1].cls = blackfin::OPERATOR;
    instr.operands[1].operat = blackfin::OPER_EQ;
    instr.operand_count = 3;
    instr.operation = blackfin::OP_CCDREG;

    switch (op) {
    case 0:
        instr.operands[0].reg = dregs(reg);
        instr.operands[2].reg = REG_CC;
        return true;
    case 1:
        instr.operands[0].reg = REG_CC;
        instr.operands[2].reg = dregs(reg);
        return true;
    case 3:
        instr.operands[0].reg = REG_CC;
        instr.operands[2].reg = REG_CC;
        instr.operands[2].flags.cc_inverted = true;
        return true;
    }
    return false;
}
