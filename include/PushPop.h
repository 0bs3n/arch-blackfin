#include "arch_blackfin.h"

class PushPopInstruction {
/* PushPopReg
+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |.W.|.grp.......|.reg.......|
+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
public:
    static bool Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
};
