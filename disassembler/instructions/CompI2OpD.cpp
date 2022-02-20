#include "Instructions.h"
#include "arch_blackfin.h"

bool 
BlackfinInstruction::DisassembleCompI2OpD(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel) 
{

	int op  = ((instructionWord >> COMPI2opP_op_bits) & COMPI2opP_op_mask);
	int src = ((instructionWord >> COMPI2opP_src_bits) & COMPI2opP_src_mask);
	int dst = ((instructionWord >> COMPI2opP_dst_bits) & COMPI2opP_dst_mask);

    if (parallel) return false;
    
    instr.operation = blackfin::OP_LDIMM; 
    instr.operand_count = 3;
    instr.operands[0].cls = blackfin::PREG;
    instr.operands[0].reg = dregs(dst);

    instr.operands[1].cls = blackfin::OPERATOR;

    instr.operands[2].cls = blackfin::IMM;
    instr.operands[2].imm = imm7(src);

    switch (op) {
    case 0:
        instr.operands[1].operat = blackfin::OPER_EQ;
        instr.operands[2].flags.sign_extended = true;
        return true;
    case 1:
        instr.operands[1].operat = blackfin::OPER_PLUSEQ;
        return true;
    default:
        return false;
    }

    return false;
}
