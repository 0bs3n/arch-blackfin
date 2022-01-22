#include "Instructions.h"
#include <stdint.h>

bfd_vma bfd_getl16 (const void *p);
bfd_vma bfd_geta32(bfd_vma p);
uint16_t get_instruction_word(uint32_t instructionCandidate, bool canonical_high);
uint32_t get_uint32_be(uint8_t *data, int addr);
static int _disasm_insn_bfin(uint32_t instructionCandidate, uint32_t addr, blackfin::Instruction &result, bool parallel);
int disasm_insn_bfin (uint8_t *data, uint32_t addr, blackfin::Instruction &instr);
