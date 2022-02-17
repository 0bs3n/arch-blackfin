enum elf_bfin_reloc_type {
    R_BFIN_UNUSED0 = 0x00,          /* relocation type 0 is not defined */
    R_BFIN_PCREL5M2 = 0x01,         /* LSETUP part a */
    R_BFIN_UNUSED1 = 0x02,          /* relocation type 2 is not defined */
    R_BFIN_PCREL10 = 0x03,          /* type 3, 0x00) if cc jump <target> */
    R_BFIN_PCREL12_JUMP = 0x04,     /* type 4, 0x00) jump <target> */
    R_BFIN_RIMM16 = 0x05,           /* type 0x5, 0x00) rN = <target> */
    R_BFIN_LUIMM16 = 0x06,          /* # 0x6, 0x00) preg.l=<target> Load imm 16 to lower half */
    R_BFIN_HUIMM16 = 0x07,          /* # 0x7, 0x00) preg.h=<target> Load imm 16 to upper half */
    R_BFIN_PCREL12_JUMP_S = 0x08,   /* # 0x8 jump.s <target> */
    R_BFIN_PCREL24_JUMP_X = 0x09,   /* # 0x9 jump.x <target> */
    R_BFIN_PCREL24 = 0x0a,          /* # 0xa call <target> , 0x00) not expandable */
    R_BFIN_UNUSEDB = 0x0b,          /* # 0xb not generated */
    R_BFIN_UNUSEDC = 0x0c,          /* # 0xc  not used */
    R_BFIN_PCREL24_JUMP_L = 0x0d,   /* 0xd jump.l <target> */
    R_BFIN_PCREL24_CALL_X = 0x0e,   /* 0xE, 0x00) call.x <target> if <target> is above 24 bit limit call through P1 */
    R_BFIN_VAR_EQ_SYMB = 0x0f,      /* 0xf, 0x00) linker should treat it same as 0x12 */
    R_BFIN_BYTE_DATA = 0x10,        /* 0x10, 0x00) .byte var = symbol */
    R_BFIN_BYTE2_DATA = 0x11,       /* 0x11, 0x00) .byte2 var = symbol */
    R_BFIN_BYTE4_DATA = 0x12,       /* 0x12, 0x00) .byte4 var = symbol and .var var=symbol */
    R_BFIN_PCREL11 = 0x13,          /* 0x13, 0x00) lsetup part b */
    R_BFIN_GOT17M4 = 0x14,
    R_BFIN_GOTHI = 0x15,
    R_BFIN_GOTLO = 0x16,
    R_BFIN_FUNCDESC = 0x17,
    R_BFIN_FUNCDESC_GOT17M4 = 0x18,
    R_BFIN_FUNCDESC_GOTHI = 0x19,
    R_BFIN_FUNCDESC_GOTLO = 0x1a,
    R_BFIN_FUNCDESC_VALUE = 0x1b,
    R_BFIN_FUNCDESC_GOTOFF17M4 = 0x1c,
    R_BFIN_FUNCDESC_GOTOFFHI = 0x1d,
    R_BFIN_FUNCDESC_GOTOFFLO = 0x1e,
    R_BFIN_GOTOFF17M4 = 0x1f,
    R_BFIN_GOTOFFHI = 0x20,
    R_BFIN_GOTOFFLO = 0x21,
    R_BFIN_PUSH = 0xE0,
    R_BFIN_CONST = 0xE1,
    R_BFIN_ADD = 0xE2,
    R_BFIN_SUB = 0xE3,
    R_BFIN_MULT = 0xE4,
    R_BFIN_DIV = 0xE5,
    R_BFIN_MOD = 0xE6,
    R_BFIN_LSHIFT = 0xE7,
    R_BFIN_RSHIFT = 0xE8,
    R_BFIN_AND = 0xE9,
    R_BFIN_OR = 0xEA,
    R_BFIN_XOR = 0xEB,
    R_BFIN_LAND = 0xEC,
    R_BFIN_LOR = 0xED,
    R_BFIN_LEN = 0xEE,
    R_BFIN_NEG = 0xEF,
    R_BFIN_COMP = 0xF0,
    R_BFIN_PAGE = 0xF1,
    R_BFIN_HWPAGE = 0xF2,
    R_BFIN_ADDR = 0xF3,
    R_BFIN_PLTPC = 0x40,         /* PLT gnu only relocation */
    R_BFIN_GOT = 0x41,           /* GOT gnu only relocation */
    R_BFIN_GNU_VTINHERIT = 0x42, /* C++, gnu only */
    R_BFIN_GNU_VTENTRY = 0x43, /* C++, gnu only */
    R_BFIN_max
};

static const char* reloc_strings[] = {
    "R_BFIN_UNUSED0",
    "R_BFIN_PCREL5M2",
    "R_BFIN_UNUSED1",
    "R_BFIN_PCREL10",
    "R_BFIN_PCREL12_JUMP",
    "R_BFIN_RIMM16",
    "R_BFIN_LUIMM16",
    "R_BFIN_HUIMM16",
    "R_BFIN_PCREL12_JUMP_S",
    "R_BFIN_PCREL24_JUMP_X",
    "R_BFIN_PCREL24",
    "R_BFIN_UNUSEDB",
    "R_BFIN_UNUSEDC",
    "R_BFIN_PCREL24_JUMP_L",
    "R_BFIN_PCREL24_CALL_X",
    "R_BFIN_VAR_EQ_SYMB",
    "R_BFIN_BYTE_DATA",
    "R_BFIN_BYTE2_DATA",
    "R_BFIN_BYTE4_DATA",
    "R_BFIN_PCREL11",
    "R_BFIN_GOT17M4",
    "R_BFIN_GOTHI",
    "R_BFIN_GOTLO",
    "R_BFIN_FUNCDESC",
    "R_BFIN_FUNCDESC_GOT17M4",
    "R_BFIN_FUNCDESC_GOTHI",
    "R_BFIN_FUNCDESC_GOTLO",
    "R_BFIN_FUNCDESC_VALUE",
    "R_BFIN_FUNCDESC_GOTOFF17M4",
    "R_BFIN_FUNCDESC_GOTOFFHI",
    "R_BFIN_FUNCDESC_GOTOFFLO",
    "R_BFIN_GOTOFF17M4",
    "R_BFIN_GOTOFFHI",
    "R_BFIN_GOTOFFLO",
    "R_BFIN_PUSH",
    "R_BFIN_CONST",
    "R_BFIN_ADD",
    "R_BFIN_SUB",
    "R_BFIN_MULT",
    "R_BFIN_DIV",
    "R_BFIN_MOD",
    "R_BFIN_LSHIFT",
    "R_BFIN_RSHIFT",
    "R_BFIN_AND",
    "R_BFIN_OR",
    "R_BFIN_XOR",
    "R_BFIN_LAND",
    "R_BFIN_LOR",
    "R_BFIN_LEN",
    "R_BFIN_NEG",
    "R_BFIN_COMP",
    "R_BFIN_PAGE",
    "R_BFIN_HWPAGE",
    "R_BFIN_ADDR",
    "R_BFIN_PLTPC",
    "R_BFIN_GOT",
    "R_BFIN_GNU_VTINHERIT",
    "R_BFIN_GNU_VTENTRY",
    "R_BFIN_max"
};
