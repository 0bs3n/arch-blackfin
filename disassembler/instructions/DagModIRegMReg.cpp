#include "Instructions.h"
#include "arch_blackfin.h"

bool 
BlackfinInstruction::DisassembleDagModIRegMReg(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel) 
{
	/* dagMODim
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
	| 1 | 0 | 0 | 1 | 1 | 1 | 1 | 0 |.br| 1 | 1 |.op|.m.....|.i.....|
	+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
	int i  = ((instructionWord >> DagMODim_i_bits) & DagMODim_i_mask);
	int m  = ((instructionWord >> DagMODim_m_bits) & DagMODim_m_mask);
	int br = ((instructionWord >> DagMODim_br_bits) & DagMODim_br_mask);
	int op = ((instructionWord >> DagMODim_op_bits) & DagMODim_op_mask);
    
    instr.operation = blackfin::OP_MV;
    instr.operand_count = 3;
    instr.operands[0] = { .cls = blackfin::REG, .reg = iregs(i) };
    instr.operands[1].cls = blackfin::OPERATOR;
    instr.operands[2] = { .cls = blackfin::REG, .reg = mregs(m) };

    switch (op) {
    case 0:
        if (br) { 
            instr.operation = blackfin::OP_RCPLUSEQ; 
            instr.operand_count = 4;
            instr.operands[3] = { .cls = blackfin::MNEMOMIC, .mnemonic = blackfin::OL_BREV };
        }
        else instr.operation = blackfin::OP_MV;
        instr.operands[1].operat = blackfin::OPER_PLUSEQ;
        break;
    case 1:
        instr.operation = blackfin::OP_MINUSEQ;
        instr.operands[1].operat = blackfin::OPER_MINUSEQ;
        break;
    defaut:
        return false;
    }
    return true;
}
