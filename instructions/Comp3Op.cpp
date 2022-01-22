#include "Comp3Op.h"
#include "arch_blackfin.h"

bool Comp3OpInstruction::Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel)
{
	/* COMP3op
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 0 | 1 | 0 | 1 |.opc.......|.dst.......|.src1......|.src0......|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
    // 0101 1110 1001 0100
    // 0101 111 010 010 100
    // op = 7, dst = 2, src1 = 2, src0 = 4
	int opc  = ((instructionWord >> COMP3op_opc_bits) & COMP3op_opc_mask);
	int dst  = ((instructionWord >> COMP3op_dst_bits) & COMP3op_dst_mask);
	int src0 = ((instructionWord >> COMP3op_src0_bits) & COMP3op_src0_mask);
	int src1 = ((instructionWord >> COMP3op_src1_bits) & COMP3op_src1_mask);
    
    instr.operation = blackfin::OP_LDREG3;
    switch (opc) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            instr.operand_count = 5;
            instr.operands[0] = { .cls = blackfin::REG, .reg = dregs(dst) };
            instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
            instr.operands[2] = { .cls = blackfin::REG, .reg = dregs(src0) };
            instr.operands[3].cls = blackfin::OPERATOR;
            instr.operands[4] = { .cls = blackfin::REG, .reg = dregs(src1) };
            break;
        case 5:
            instr.operand_count = 5;
            instr.operands[0] = { .cls = blackfin::REG, .reg = pregs(dst) };
            instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
            instr.operands[2] = { .cls = blackfin::REG, .reg = pregs(src0) };
            instr.operands[3].cls = blackfin::OPERATOR;
            instr.operands[4] = { .cls = blackfin::REG, .reg = pregs(src1) };
            break;
        case 6:
        case 7:
            instr.operand_count = 7;
            instr.operands[0] = { .cls = blackfin::REG, .reg = pregs(dst) };
            instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
            instr.operands[2] = { .cls = blackfin::REG, .reg = pregs(src0) };
            instr.operands[3].cls = blackfin::OPERATOR;
            instr.operands[4] = { .cls = blackfin::REG, .reg = pregs(src1) };
            instr.operands[5] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_LSHIFTL };
            instr.operands[6].cls = blackfin::IMM;

            if (opc == 6) instr.operands[6].imm = 1;
            if (opc == 7) instr.operands[6].imm = 2;
            break;
        default:
            return false;

    }
    if (opc == 0 || opc == 5 || opc == 6 || opc == 7) instr.operands[3].operat = blackfin::OPER_PLUS;
    if (opc == 1) instr.operands[3].operat = blackfin::OPER_MINUS;
    if (opc == 2) instr.operands[3].operat = blackfin::OPER_AND;
    if (opc == 3) instr.operands[3].operat = blackfin::OPER_OR;
    if (opc == 4) instr.operands[3].operat = blackfin::OPER_XOR;
    return true;
}
