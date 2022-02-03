#include "ALU2Op.h"
#include "arch_blackfin.h"

bool ALU2opInstruction::Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel)
{
    /* ALU2op
    +---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
    | 0 | 1 | 0 | 0 | 0 | 0 |.opc...........|.src.......|.dst.......|
    +---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int src = ((instructionWord >> ALU2op_src_bits) & ALU2op_src_mask);
	int opc = ((instructionWord >> ALU2op_opc_bits) & ALU2op_opc_mask);
	int dst = ((instructionWord >> ALU2op_dst_bits) & ALU2op_dst_mask);

    // Register assignment
    switch (opc) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 14:
    case 15:
        instr.operand_count = 3;
        instr.operation = blackfin::OP_ALU2OP;
        instr.operands[0] = { .cls = blackfin::REG, .reg = dregs(dst) };
        instr.operands[2] = { .cls = blackfin::REG, .reg = dregs(src) };
        instr.operands[1].cls = blackfin::OPERATOR;
        if (opc == 0)  instr.operands[1].operat = blackfin::OPER_ASHIFTREQ;
        if (opc == 1)  instr.operands[1].operat = blackfin::OPER_LSHIFTREQ;
        if (opc == 2)  instr.operands[1].operat = blackfin::OPER_LSHIFTLEQ;
        if (opc == 3)  instr.operands[1].operat = blackfin::OPER_MULEQ;
        if (opc == 14) instr.operands[1].operat = blackfin::OPER_EQNEG;
        if (opc == 15) instr.operands[1].operat = blackfin::OPER_EQNOT;
        break;
    case 4:
    case 5:
        instr.operation = blackfin::OP_ALUADDTHENSHIFT;
        instr.operand_count = 7;
        instr.operands[0] = { .cls = blackfin::REG, .reg = dregs(dst) };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2] = { .cls = blackfin::REG, .reg = dregs(dst) };
        instr.operands[3] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_PLUS };
        instr.operands[4] = { .cls = blackfin::REG, .reg = dregs(src) };
        instr.operands[5] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_LSHIFTR };
        instr.operands[6].cls = blackfin::IMM;
        if (opc == 4) instr.operands[6].imm = 1;
        if (opc == 5) instr.operands[6].imm = 2;
        break;
    case 8:
    case 9:
        instr.operation = blackfin::OP_ALUDIVPRIM;
        instr.operand_count = 4;
        instr.operands[0].cls = blackfin::MNEMOMIC;
        instr.operands[1] = { .cls = blackfin::REG, .reg = dregs(dst)};
        instr.operands[2] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_COMMA };
        instr.operands[3] = { .cls = blackfin::REG, .reg = dregs(src)};
        if (opc == 8) instr.operands[0].mnemonic = blackfin::OL_DIVQ;
        if (opc == 9) instr.operands[0].mnemonic = blackfin::OL_DIVS;
        break;
    case 10:
    case 11:
    case 12:
    case 13:
        instr.operation = blackfin::OP_ALU2OP;
        instr.operand_count = 3;
        instr.operands[0].cls = blackfin::REG;
        instr.operands[0].reg = dregs(dst);
        instr.operands[1].cls = blackfin::OPERATOR;
        instr.operands[1].operat = blackfin::OPER_EQ;
        instr.operands[2].cls = blackfin::REG;

        if (opc == 10 || opc == 11) instr.operands[2].reg = dregs_lo(src);
        if (opc == 12 || opc == 13) instr.operands[2].reg = dregs_byte(src);

        if (opc == 10) instr.operands[2].flags.sign_extended = true;
        if (opc == 11) instr.operands[2].flags.zero_extended = true;
        if (opc == 12) instr.operands[2].flags.sign_extended = true;
        if (opc == 13) instr.operands[2].flags.zero_extended = true;
        break;
    }
    return true;
}
