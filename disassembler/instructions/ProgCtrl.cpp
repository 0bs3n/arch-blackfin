#include "Instructions.h"

enum ProgCtrlOps {
    PCNOP,
    PCRET,
    PCEEVENT,
    PCINTCTRL,
    PCJMP,
    PCCALL,
    PCEXCPTCTRL,
    PCTESTSET,
    PCILLEGAL
};

bool BlackfinInstruction::DisassembleProgCtrl(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel) {
	/* ProgCtrl
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |.prgfunc.......|.poprnd........|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
    
    int poprnd  = ((instructionWord >> ProgCtrl_poprnd_bits) & ProgCtrl_poprnd_mask);
    int prgfunc = ((instructionWord >> ProgCtrl_prgfunc_bits) & ProgCtrl_prgfunc_mask);
    ProgCtrlOps operation;

    if (prgfunc == PCNOP && parallel) return PCILLEGAL;

    switch (prgfunc) {
    case 0:
         operation = PCNOP;
         break;
    case 1:
        operation = PCRET;
        break;
    case 2:
        operation = PCEEVENT;
        break;
    case 3:
    case 4:
        operation = PCINTCTRL;
        break;
    case 5:
    case 6:
        operation = PCJMP;
        break;
    case 7:
    case 8:
        operation = PCCALL;
        break;
    case 9:
    case 10:
        operation = PCEXCPTCTRL;
        break;
    case 11:
        operation = PCTESTSET;
        break;
    default:
        operation = PCILLEGAL;
        break;
    }

    if (operation == PCILLEGAL) return false;
    switch (operation) {
    case PCNOP:
        instr.operands[0].cls = blackfin::MNEMOMIC;
        instr.operands[0].mnemonic = blackfin::OL_NOP;
        instr.operation = blackfin::OP_NOP;
        instr.operand_count = 1;
        return true;
    case PCRET:
        instr.operands[0].cls = blackfin::MNEMOMIC;
        instr.operands[1].cls = blackfin::SREG;
        instr.operation = blackfin::OP_RET;
        instr.operand_count = 2;
        switch (poprnd) {
        case 0:
            instr.operands[0].mnemonic = blackfin::OL_RTS;
            instr.operands[1].reg = REG_RETS;
            return true;
        case 1:
            instr.operands[0].mnemonic = blackfin::OL_RTI;
            instr.operands[1].reg = REG_RETI;
            return true;
        case 2:
            instr.operands[0].mnemonic = blackfin::OL_RTX;
            instr.operands[1].reg = REG_RETX;
            return true;
        case 3:
            instr.operands[0].mnemonic = blackfin::OL_RTN;
            instr.operands[1].reg = REG_RETN;
            return true;
        case 4:
            instr.operands[0].mnemonic = blackfin::OL_RTE;
            instr.operands[1].reg = REG_RETE;
            return true;
        default:
            return false;
        }
    case PCEEVENT:
        instr.operands[0].cls = blackfin::MNEMOMIC;
        instr.operation = blackfin::OP_EEVENT;
        instr.operand_count = 1;
        switch (poprnd) {
        case 0:
            instr.operands[0].mnemonic = blackfin::OL_IDLE;
            return true;
        case 2:
            instr.operands[0].mnemonic = blackfin::OL_CSYNC;
            return true;
        case 3:
            instr.operands[0].mnemonic = blackfin::OL_SSYNC;
            return true;
        case 4:
            instr.operands[0].mnemonic = blackfin::OL_EMUEXCPT;
            return true;
        default:
            return false;
        }
    case PCINTCTRL:
        if (!IS_DREG(0, poprnd)) return false;
        instr.operands[0].cls = blackfin::MNEMOMIC;
        instr.operands[1].cls = blackfin::DREG;
        instr.operands[1].reg = dregs(poprnd);
        instr.operand_count = 2;
        switch (prgfunc) {
        case 3:
            instr.operands[0].mnemonic = blackfin::OL_CLI;
            return true;
        case 4:
            instr.operands[0].mnemonic = blackfin::OL_STI;
            return true;
        }
    case PCJMP:
    case PCCALL:
        if (!IS_PREG(1, poprnd)) return false;
        instr.operands[0].cls = blackfin::MNEMOMIC;
        instr.operands[1].cls = blackfin::PREG;
        instr.operands[1].reg = pregs(poprnd);
        instr.operand_count = 2;
        switch (prgfunc) {
        case 5:
        case 8:
            instr.operands[0].mnemonic = blackfin::OL_JUMP;
            break;
        case 6:
        case 7:
            instr.operands[0].mnemonic = blackfin::OL_CALL;
            break;
        }
        switch (prgfunc) {
        case 5:
            instr.operation = blackfin::OP_JMPREGABS;
            return true;
        case 6:
            instr.operation = blackfin::OP_CALLREGABS;
            return true;
        case 7:
            instr.operation = blackfin::OP_CALLREGREL;
            return true;
        case 8:
            instr.operation = blackfin::OP_JMPREGREL;
            return true;
        default:
            return false;
        }
    case PCEXCPTCTRL:
        instr.operands[0].cls = blackfin::MNEMOMIC;
        instr.operands[1].cls = blackfin::IMM;
        instr.operands[1].imm = uimm4(poprnd);
        instr.operand_count = 2;
        switch (prgfunc) {
        case 10:
            instr.operands[0].mnemonic = blackfin::OL_RAISE;
            instr.operation = blackfin::OP_RAISE;
            return true;
        case 11:
            instr.operands[0].mnemonic = blackfin::OL_EXCPT;
            instr.operation = blackfin::OP_EXCPT;
            return true;
        default:
            return false;
        }
    case PCTESTSET:
        if (!IS_PREG(1, poprnd) || poprnd > 5) return false;
        instr.operands[0].cls = blackfin::MNEMOMIC;
        instr.operands[0].mnemonic = blackfin::OL_TESTSET;
        instr.operands[1].cls = blackfin::PREG;
        instr.operands[1].reg = pregs(poprnd);
        instr.operand_count = 2;
        return true;
    default:
        return false;
    }
    return false;
}
