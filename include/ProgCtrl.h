#include "arch_blackfin.h"

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

class ProgCtrlInstruction {
/* ProgCtrl
+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |.prgfunc.......|.poprnd........|
+---+---+---+---|---+---+---+---|---+---+---+---|---+---+---+---+  */
public:
    static bool Disassemble(uint16_t instructionWord, struct blackfin::Instruction &instr, bool parallel);
};
