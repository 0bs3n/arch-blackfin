#include "Logic2Op.h"
#include "arch_blackfin.h"

bool Logic2OpInstruction::Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel)
{
	int src = ((instructionWord >> LOGI2op_src_bits) & LOGI2op_src_mask);
	int opc = ((instructionWord >> LOGI2op_opc_bits) & LOGI2op_opc_mask);
	int dst = ((instructionWord >> LOGI2op_dst_bits) & LOGI2op_dst_mask);

    if (parallel) return false;

	if (opc == 0 || opc == 1)
	{
        instr.operation = blackfin::OP_CCBITTST;
        instr.operand_count = 6;
        instr.operands[0] = { .cls = blackfin::REG, .reg = REG_CC };
        instr.operands[1] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_EQ };
        instr.operands[2] = { .cls = blackfin::MNEMOMIC, .mnemonic = blackfin::OL_BITTST };
        instr.operands[3] = { .cls = blackfin::REG, .reg = dregs(dst) };
        instr.operands[4] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_COMMA };
        instr.operands[5] = { .cls = blackfin::IMM, .imm = uimm5(src) };
        if (opc == 0) instr.operands[2].flags.cc_inverted = true;
        return true;
	} else if (opc == 2 || opc == 3 || opc == 4) {
        instr.operand_count = 4;
        instr.operands[0].cls = blackfin::MNEMOMIC;
        instr.operands[1] = { .cls = blackfin::REG, .reg = dregs(dst) };
        instr.operands[2] = { .cls = blackfin::OPERATOR, .operat = blackfin::OPER_COMMA };
        instr.operands[3] = { .cls = blackfin::IMM, .imm = uimm5(src) };

        if (opc == 2) {
            instr.operands[0].mnemonic = blackfin::OL_BITSET;
            instr.operation = blackfin::OP_BITSET;
        }
        if (opc == 3) {
            instr.operands[0].mnemonic = blackfin::OL_BITTGL;
            instr.operation = blackfin::OP_BITTGL;
        }
        if (opc == 4) {
            instr.operands[0].mnemonic = blackfin::OL_BITCLR;
            instr.operation = blackfin::OP_BITCLR;
        }
    } else if (opc == 5 || opc == 6 || opc == 7) {
        instr.operand_count = 3;
        instr.operands[0] = { .cls = blackfin::REG, .reg = dregs(dst) };
        instr.operands[1].cls = blackfin::OPERATOR;
        instr.operands[2] = { .cls = blackfin::IMM, .imm = uimm5(src) };

        if (opc == 5) {
            instr.operation = blackfin::OP_MVSHIFTED;
            instr.operands[1].operat = blackfin::OPER_ASHIFTREQ;
        } else if (opc == 6) {
            instr.operation = blackfin::OP_MVSHIFTED;
            instr.operands[1].operat = blackfin::OPER_LSHIFTREQ;
        } else if (opc == 6) {
            instr.operation = blackfin::OP_MVSHIFTED;
            instr.operands[1].operat = blackfin::OPER_LSHIFTLEQ;
        }
    }
    return true;
}
