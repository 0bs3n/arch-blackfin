#include "Instructions.h"
#include "arch_blackfin.h"

enum ArgMode {
    DREG_DREG,
    PREG_PREG,
    DREG_IMM,
    PREG_IMM
};

bool BlackfinInstruction::DisassembleCCFlag(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel) {
	int x = ((instructionWord >> CCflag_x_bits) & CCflag_x_mask);
	int y = ((instructionWord >> CCflag_y_bits) & CCflag_y_mask);
	int I = ((instructionWord >> CCflag_I_bits) & CCflag_I_mask);
	int G = ((instructionWord >> CCflag_G_bits) & CCflag_G_mask);
	int opc = ((instructionWord >> CCflag_opc_bits) & CCflag_opc_mask);

    // FIXME: The internet says this is unsafe, but I and G are at most each 1, or'd max 3 so we fit
    enum ArgMode arg_mode = static_cast<enum ArgMode>((I << 1) | G);

    if (parallel) return false;

    // CC = reg ==/</<=/u</u<= reg
    instr.operands[0].cls = blackfin::SREG;
    instr.operands[0].reg = REG_CC;
    instr.operands[0].flags.cc_inverted = false;
    instr.operands[1].cls = blackfin::OPERATOR;
    instr.operands[1].operat = blackfin::OPER_EQ;
    instr.operands[3].cls = blackfin::OPERATOR;
    instr.operation = blackfin::OP_CCFLAG;
    instr.operand_count = 5;

    if (opc == 5 || opc == 6 || opc == 7) {
        instr.operands[2].cls = blackfin::REG;
        instr.operands[2].reg = REG_A0;
        instr.operands[4].cls = blackfin::REG;
        instr.operands[4].reg = REG_A1;
        switch (opc) {
        case 5:
            instr.operands[3].operat = blackfin::OPER_EQEQ;
            return true;
        case 6:
            instr.operands[3].operat = blackfin::OPER_LT;
            return true;
        case 7:
            instr.operands[3].operat = blackfin::OPER_LTE;
            return true;
        }
    }
    
    switch (arg_mode) {
    case DREG_DREG:
        instr.operands[2].cls = blackfin::REG;
        instr.operands[2].reg = dregs(x);
        instr.operands[4].cls = blackfin::REG;
        instr.operands[4].reg = dregs(y);
        break;
    case PREG_PREG:
        instr.operands[2].cls = blackfin::REG;
        instr.operands[2].reg = pregs(x);
        instr.operands[4].cls = blackfin::REG;
        instr.operands[4].reg = pregs(y);
        break;
    case DREG_IMM:
        instr.operands[2].cls = blackfin::REG;
        instr.operands[2].reg = dregs(x);
        switch (opc) {
        case 0:
        case 1:
        case 2:
            instr.operands[4].cls = blackfin::IMM;
            instr.operands[4].imm = imm3(y);
            break;
        case 3:
        case 4:
            instr.operands[4].cls = blackfin::IMM;
            instr.operands[4].imm = uimm3(y);
            break;
        }
        break;
    case PREG_IMM:
        instr.operands[2].cls = blackfin::REG;
        instr.operands[2].reg = pregs(x);
        switch (opc) {
        case 0:
        case 1:
        case 2:
            instr.operands[4].cls = blackfin::IMM;
            instr.operands[4].imm = imm3(y);
            break;
        case 3:
        case 4:
            instr.operands[4].cls = blackfin::IMM;
            instr.operands[4].imm = uimm3(y);
            break;
        }
        break;
        break;
    }

    switch (opc) {
    case 0:
        instr.operands[3].operat = blackfin::OPER_EQEQ;
        break;
    case 1:
        instr.operands[3].operat = blackfin::OPER_LT;
        break;
    case 2:
        instr.operands[3].operat = blackfin::OPER_LTE;
        break;
    case 3:
        instr.operands[3].operat = blackfin::OPER_LTU;
        break;
    case 4:
        instr.operands[3].operat = blackfin::OPER_LTEU;
        break;
    }
    return true;
}
