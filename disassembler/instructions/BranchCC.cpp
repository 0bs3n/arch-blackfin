#include "Instructions.h"
#include "arch_blackfin.h"

bool BlackfinInstruction::DisassembleBranchCC(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel)
{
	int B = ((instructionWord >> BRCC_B_bits) & BRCC_B_mask);
	int T = ((instructionWord >> BRCC_T_bits) & BRCC_T_mask);
	int offset = ((instructionWord >> BRCC_offset_bits) & BRCC_offset_mask);
    
    if (parallel) return false;

    // The disassembler doesn't care about BP I don't think, but whether or not a given
    // branch is predicted as taken is encoded in the B bit. 
    // i.e. B == 1, predict as taken, B == 0 predict as not taken.
    // I'm adding it as a flag and whatever is using this decomp
    // result can decide if they care or not.
    
    instr.operand_count = 4;
    instr.operation = blackfin::OP_BRCC;
    instr.operands[0].cls = blackfin::MNEMOMIC;
    instr.operands[0].mnemonic = blackfin::OL_IF;
    instr.operands[1].cls = blackfin::REG;
    instr.operands[1].reg = REG_CC;
    instr.operands[2].cls = blackfin::MNEMOMIC;
    instr.operands[2].mnemonic = blackfin::OL_JUMP;
    instr.operands[3].cls = blackfin::IMM;
    instr.operands[3].imm = pcrel10(offset);
    instr.operands[3].flags.pcrelative = true;
    
    if (B) instr.operands[3].flags.branch_predicted = true;
    if (!T) instr.operands[1].flags.cc_inverted = true;
    return true;
}
