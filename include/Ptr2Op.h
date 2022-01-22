#include "arch_blackfin.h"

class Ptr2OpInstruction {
/* PTR2op
+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
| 0 | 1 | 0 | 0 | 0 | 1 | 0 |.opc.......|.src.......|.dst.......|
+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
public:
    static bool Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
};
