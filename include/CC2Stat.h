#include "arch_blackfin.h"

class CC2StatInstruction {
/* CC2stat
+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 1 | 1 |.D.|.op....|.cbit..............|
+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
public:
    static bool Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
};
