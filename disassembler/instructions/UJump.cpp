#include "Instructions.h"
#include "arch_blackfin.h"

bool BlackfinInstruction::DisassembleUJump(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel)
{
    int32_t offset = ((instructionWord >> UJump_offset_bits) & UJump_offset_mask);

    if (parallel) return false;

    instr.operation = blackfin::OP_JMPREL;
    instr.operand_count = 2;
    instr.operands[0] = { .cls = blackfin::MNEMOMIC, .mnemonic = blackfin::OL_JUMPS };
    instr.operands[1] = { .cls = blackfin::IMM, .imm = pcrel12(offset) };
    instr.operands[1].flags.pcrelative = true;

    return true;
}
