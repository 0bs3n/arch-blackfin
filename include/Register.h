enum Register
{
  REG_RL0, REG_RL1, REG_RL2, REG_RL3, REG_RL4, REG_RL5, REG_RL6, REG_RL7,
  REG_RH0, REG_RH1, REG_RH2, REG_RH3, REG_RH4, REG_RH5, REG_RH6, REG_RH7,
  REG_R0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,
  REG_R1_0, REG_R3_2, REG_R5_4, REG_R7_6, REG_P0, REG_P1, REG_P2, REG_P3,
  REG_P4, REG_P5, REG_SP, REG_FP, REG_A0x, REG_A1x, REG_A0w, REG_A1w,
  REG_A0, REG_A1, REG_I0, REG_I1, REG_I2, REG_I3, REG_M0, REG_M1,
  REG_M2, REG_M3, REG_B0, REG_B1, REG_B2, REG_B3, REG_L0, REG_L1,
  REG_L2, REG_L3,
  REG_AZ, REG_AN, REG_AC0, REG_AC1, REG_AV0, REG_AV1, REG_AV0S, REG_AV1S,
  REG_AQ, REG_V, REG_VS,
  REG_sftreset, REG_omode, REG_excause, REG_emucause, REG_idle_req, REG_hwerrcause, REG_CC, REG_LC0,
  REG_LC1, REG_ASTAT, REG_RETS, REG_LT0, REG_LB0, REG_LT1, REG_LB1,
  REG_CYCLES, REG_CYCLES2, REG_USP, REG_SEQSTAT, REG_SYSCFG, REG_RETI, REG_RETX, REG_RETN,
  REG_RETE, REG_EMUDAT, REG_BR0, REG_BR1, REG_BR2, REG_BR3, REG_BR4, REG_BR5, REG_BR6,
  REG_BR7, REG_PL0, REG_PL1, REG_PL2, REG_PL3, REG_PL4, REG_PL5, REG_SLP, REG_FLP,
  REG_PH0, REG_PH1, REG_PH2, REG_PH3, REG_PH4, REG_PH5, REG_SHP, REG_FHP,
  REG_IL0, REG_IL1, REG_IL2, REG_IL3, REG_ML0, REG_ML1, REG_ML2, REG_ML3,
  REG_BL0, REG_BL1, REG_BL2, REG_BL3, REG_LL0, REG_LL1, REG_LL2, REG_LL3,
  REG_IH0, REG_IH1, REG_IH2, REG_IH3, REG_MH0, REG_MH1, REG_MH2, REG_MH3,
  REG_BH0, REG_BH1, REG_BH2, REG_BH3, REG_LH0, REG_LH1, REG_LH2, REG_LH3,
  REG_AC0_COPY, REG_V_COPY, REG_RND_MOD, REG_A0H, REG_A0L, REG_A1H, REG_A1L,
  REG_LASTREG,
};

enum reg_class
{
  rc_dregs_lo, rc_dregs_hi, rc_dregs, rc_dregs_pair, rc_dregs_b, rc_pregs, rc_spfp, rc_dregs_hilo, rc_accum_ext,
  rc_accum_word, rc_accum, rc_iregs, rc_mregs, rc_bregs, rc_lregs, rc_dpregs, rc_gregs,
  rc_regs, rc_statbits, rc_ignore_bits, rc_ccstat, rc_counters, rc_dregs2_sysregs1, rc_open, rc_sysregs2,
  rc_sysregs3, rc_allregs,
  LIM_REG_CLASSES
};

static const char * const reg_names[] =
{
  "R0.L", "R1.L", "R2.L", "R3.L", "R4.L", "R5.L", "R6.L", "R7.L",
  "R0.H", "R1.H", "R2.H", "R3.H", "R4.H", "R5.H", "R6.H", "R7.H",
  "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
  "R1:0", "R3:2", "R5:4", "R7:6", "P0", "P1", "P2", "P3",
  "P4", "P5", "SP", "FP", "A0.X", "A1.X", "A0.W", "A1.W",
  "A0", "A1", "I0", "I1", "I2", "I3", "M0", "M1",
  "M2", "M3", "B0", "B1", "B2", "B3", "L0", "L1",
  "L2", "L3",
  "AZ", "AN", "AC0", "AC1", "AV0", "AV1", "AV0S", "AV1S",
  "AQ", "V", "VS",
  "sftreset", "omode", "excause", "emucause", "idle_req", "hwerrcause", "CC", "LC0",
  "LC1", "ASTAT", "RETS", "LT0", "LB0", "LT1", "LB1",
  "CYCLES", "CYCLES2", "USP", "SEQSTAT", "SYSCFG", "RETI", "RETX", "RETN",
  "RETE", "EMUDAT",
  "R0.B", "R1.B", "R2.B", "R3.B", "R4.B", "R5.B", "R6.B", "R7.B",
  "P0.L", "P1.L", "P2.L", "P3.L", "P4.L", "P5.L", "SP.L", "FP.L",
  "P0.H", "P1.H", "P2.H", "P3.H", "P4.H", "P5.H", "SP.H", "FP.H",
  "I0.L", "I1.L", "I2.L", "I3.L", "M0.L", "M1.L", "M2.L", "M3.L",
  "B0.L", "B1.L", "B2.L", "B3.L", "L0.L", "L1.L", "L2.L", "L3.L",
  "I0.H", "I1.H", "I2.H", "I3.H", "M0.H", "M1.H", "M2.H", "M3.H",
  "B0.H", "B1.H", "B2.H", "B3.H", "L0.H", "L1.H", "L2.H", "L3.H",
  "AC0_COPY", "V_COPY", "RND_MOD", "A0.H", "A0.L", "A1.H", "A1.L",
  "LASTREG",
  0
};


static const enum Register decode_dregs_lo[] =
{
  REG_RL0, REG_RL1, REG_RL2, REG_RL3, REG_RL4, REG_RL5, REG_RL6, REG_RL7,
};


/* RH(0..7).  */
static const enum Register decode_dregs_hi[] =
{
  REG_RH0, REG_RH1, REG_RH2, REG_RH3, REG_RH4, REG_RH5, REG_RH6, REG_RH7,
};


/* R(0..7).  */
static const enum Register decode_dregs[] =
{
  REG_R0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,
};

/* R BYTE(0..7).  */
static const enum Register decode_dregs_byte[] =
{
  REG_BR0, REG_BR1, REG_BR2, REG_BR3, REG_BR4, REG_BR5, REG_BR6, REG_BR7,
};


/* P(0..5) SP FP.  */
static const enum Register decode_pregs[] =
{
  REG_P0, REG_P1, REG_P2, REG_P3, REG_P4, REG_P5, REG_SP, REG_FP,
};


/* I(0..3).  */
static const enum Register decode_iregs[] =
{
  REG_I0, REG_I1, REG_I2, REG_I3,
};


/* M(0..3).  */
static const enum Register decode_mregs[] =
{
  REG_M0, REG_M1, REG_M2, REG_M3,
};


/* dregs pregs.  */
static const enum Register decode_dpregs[] =
{
  REG_R0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,
  REG_P0, REG_P1, REG_P2, REG_P3, REG_P4, REG_P5, REG_SP, REG_FP,
};


/* [dregs pregs].  */
static const enum Register decode_gregs[] =
{
  REG_R0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,
  REG_P0, REG_P1, REG_P2, REG_P3, REG_P4, REG_P5, REG_SP, REG_FP,
};


/* [dregs pregs (iregs mregs) (bregs lregs)].  */
static const enum Register decode_regs[] =
{
  REG_R0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,
  REG_P0, REG_P1, REG_P2, REG_P3, REG_P4, REG_P5, REG_SP, REG_FP,
  REG_I0, REG_I1, REG_I2, REG_I3, REG_M0, REG_M1, REG_M2, REG_M3,
  REG_B0, REG_B1, REG_B2, REG_B3, REG_L0, REG_L1, REG_L2, REG_L3,
};


/* [dregs pregs (iregs mregs) (bregs lregs) Low Half].  */
static const enum Register decode_regs_lo[] =
{
  REG_RL0, REG_RL1, REG_RL2, REG_RL3, REG_RL4, REG_RL5, REG_RL6, REG_RL7,
  REG_PL0, REG_PL1, REG_PL2, REG_PL3, REG_PL4, REG_PL5, REG_SLP, REG_FLP,
  REG_IL0, REG_IL1, REG_IL2, REG_IL3, REG_ML0, REG_ML1, REG_ML2, REG_ML3,
  REG_BL0, REG_BL1, REG_BL2, REG_BL3, REG_LL0, REG_LL1, REG_LL2, REG_LL3,
};


/* [dregs pregs (iregs mregs) (bregs lregs) High Half].  */
static const enum Register decode_regs_hi[] =
{
  REG_RH0, REG_RH1, REG_RH2, REG_RH3, REG_RH4, REG_RH5, REG_RH6, REG_RH7,
  REG_PH0, REG_PH1, REG_PH2, REG_PH3, REG_PH4, REG_PH5, REG_SHP, REG_FHP,
  REG_IH0, REG_IH1, REG_IH2, REG_IH3, REG_MH0, REG_MH1, REG_MH2, REG_MH3,
  REG_BH0, REG_BH1, REG_BH2, REG_BH3, REG_LH0, REG_LH1, REG_LH2, REG_LH3,
};


static const enum Register decode_statbits[] =
{
  REG_AZ,        REG_AN,        REG_AC0_COPY,    REG_V_COPY,
  REG_LASTREG,   REG_LASTREG,   REG_AQ,          REG_LASTREG,
  REG_RND_MOD,   REG_LASTREG,   REG_LASTREG,     REG_LASTREG,
  REG_AC0,       REG_AC1,       REG_LASTREG,     REG_LASTREG,
  REG_AV0,       REG_AV0S,      REG_AV1,         REG_AV1S,
  REG_LASTREG,   REG_LASTREG,   REG_LASTREG,     REG_LASTREG,
  REG_V,         REG_VS,        REG_LASTREG,     REG_LASTREG,
  REG_LASTREG,   REG_LASTREG,   REG_LASTREG,     REG_LASTREG,
};


/* LC0 LC1.  */
static const enum Register decode_counters[] =
{
  REG_LC0, REG_LC1,
};


/* [dregs pregs (iregs mregs) (bregs lregs)
   dregs2_sysregs1 open sysregs2 sysregs3].  */
static const enum Register decode_allregs[] =
{
  REG_R0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,
  REG_P0, REG_P1, REG_P2, REG_P3, REG_P4, REG_P5, REG_SP, REG_FP,
  REG_I0, REG_I1, REG_I2, REG_I3, REG_M0, REG_M1, REG_M2, REG_M3,
  REG_B0, REG_B1, REG_B2, REG_B3, REG_L0, REG_L1, REG_L2, REG_L3,
  REG_A0x, REG_A0w, REG_A1x, REG_A1w, REG_LASTREG, REG_LASTREG, REG_ASTAT, REG_RETS,
  REG_LASTREG, REG_LASTREG, REG_LASTREG, REG_LASTREG, REG_LASTREG, REG_LASTREG, REG_LASTREG, REG_LASTREG,
  REG_LC0, REG_LT0, REG_LB0, REG_LC1, REG_LT1, REG_LB1, REG_CYCLES, REG_CYCLES2,
  REG_USP, REG_SEQSTAT, REG_SYSCFG, REG_RETI, REG_RETX, REG_RETN, REG_RETE, REG_EMUDAT,
  REG_LASTREG,
};

typedef enum
{
  c_0, c_1, c_4, c_2, c_uimm2, c_uimm3, c_imm3, c_pcrel4,
  c_imm4, c_uimm4s4, c_uimm4s4d, c_uimm4, c_uimm4s2, c_negimm5s4, c_imm5, c_imm5d, c_uimm5, c_imm6,
  c_imm7, c_imm7d, c_imm8, c_uimm8, c_pcrel8, c_uimm8s4, c_pcrel8s4, c_lppcrel10, c_pcrel10,
  c_pcrel12, c_imm16s4, c_luimm16, c_imm16, c_imm16d, c_huimm16, c_rimm16, c_imm16s2, c_uimm16s4,
  c_uimm16s4d, c_uimm16, c_pcrel24, c_uimm32, c_imm32, c_huimm32, c_huimm32e,
} const_forms_t;

struct formats
{
  char *name;
  int nbits;
  char reloc;
  char issigned;
  char pcrel;
  char scale;
  char offset;
  char negative;
  char positive;
  char decimal;
  char leading;
  char exact;
};

static struct formats constant_formats[] =
{
  { "0",          0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  { "1",          0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  { "4",          0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  { "2",          0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  { "uimm2",      2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { "uimm3",      3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { "imm3",       3, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  { "pcrel4",     4, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  { "imm4",       4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  { "uimm4s4",    4, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0},
  { "uimm4s4d",   4, 0, 0, 0, 2, 0, 0, 1, 1, 0, 0},
  { "uimm4",      4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { "uimm4s2",    4, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
  { "negimm5s4",  5, 0, 1, 0, 2, 0, 1, 0, 0, 0, 0},
  { "imm5",       5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  { "imm5d",      5, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
  { "uimm5",      5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { "imm6",       6, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  { "imm7",       7, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  { "imm7d",      7, 0, 1, 0, 0, 0, 0, 0, 1, 3, 0},
  { "imm8",       8, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  { "uimm8",      8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { "pcrel8",     8, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  { "uimm8s4",    8, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
  { "pcrel8s4",   8, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0},
  { "lppcrel10", 10, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  { "pcrel10",   10, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
  { "pcrel12",   12, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
  { "imm16s4",   16, 0, 1, 0, 2, 0, 0, 0, 0, 0, 0},
  { "luimm16",   16, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { "imm16",     16, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  { "imm16d",    16, 0, 1, 0, 0, 0, 0, 0, 1, 3, 0},
  { "huimm16",   16, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { "rimm16",    16, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  { "imm16s2",   16, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
  { "uimm16s4",  16, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
  { "uimm16s4d", 16, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0},
  { "uimm16",    16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { "pcrel24",   24, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
  { "uimm32",    32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { "imm32",     32, 0, 1, 0, 0, 0, 0, 0, 1, 3, 0},
  { "huimm32",   32, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { "huimm32e",  32, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1}
};

#define HOST_LONG_WORD_SIZE (sizeof (int) * 8)
#define XFIELD(w,p,s)       (((w) & ((1 << (s)) - 1) << (p)) >> (p))
#define SIGNEXTEND(v, n)    ((v << (HOST_LONG_WORD_SIZE - (n))) >> (HOST_LONG_WORD_SIZE - (n)))
#define MASKBITS(val, bits) (val & ((1 << bits) - 1))
