#include "Ptr2Op.h"
#include "arch_blackfin.h"

bool Ptr2OpInstruction::Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel)
{
    /* PTR2op
    +---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
    | 0 | 1 | 0 | 0 | 0 | 1 | 0 |.opc.......|.src.......|.dst.......|
    +---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
    int src = ((instructionWord >> PTR2op_src_bits) & PTR2op_dst_mask);
	int opc = ((instructionWord >> PTR2op_opc_bits) & PTR2op_opc_mask);
	int dst = ((instructionWord >> PTR2op_dst_bits) & PTR2op_dst_mask);

    // Register assignments
    switch (opc) {
    case 1:
    case 3:
    case 4:
        instr.operand_count = 5;
        instr.operation = blackfin::OP_MVSHIFTED;
        instr.operands[0] = { .cls = blackfin::REG, .reg = pregs(dst) };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2] = { .cls = blackfin::REG, .reg = pregs(src) };

        if (opc == 1) instr.operands[3] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_LSHIFTL };
        else instr.operands[3] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_LSHIFTR };

        if (opc == 4) instr.operands[4] = { .cls = blackfin::IMM, .imm = 1 };
        else instr.operands[4] = { .cls = blackfin::IMM, .imm = 2 };
        break;
    case 0:
    case 5:
        instr.operand_count = 3;
        instr.operands[0] = { .cls = blackfin::REG, .reg = pregs(dst) };
        instr.operands[1].cls = blackfin::OPERATOR;
        instr.operands[2] = { .cls = blackfin::REG, .reg = pregs(src) };
        if (opc == 0) {
            instr.operation = blackfin::OP_MINUSEQ;
            instr.operands[1].operat = blackfin::OPER_MINUSEQ;
        }
        else {
            instr.operation = blackfin::OP_RCADD;
            instr.operands[1].operat = blackfin::OPER_PLUSEQ;
        }
        break; 
    case 6:
    case 7:
        instr.operation = blackfin::OP_MVADDTHENSHIFT;
        instr.operand_count = 7;
        instr.operands[0] = { .cls = blackfin::REG, .reg = pregs(dst) };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2] = { .cls = blackfin::REG, .reg = pregs(dst) };
        instr.operands[3] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_PLUS };
        instr.operands[4] = { .cls = blackfin::REG, .reg = pregs(src) };
        instr.operands[5] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_LSHIFTL };
        instr.operands[6].cls = blackfin::IMM;

        if (opc == 6) instr.operands[6].imm = 1;
        if (opc == 7) instr.operands[6].imm = 2;

        break;
    default:
        return false;
    }
    return true;
}
