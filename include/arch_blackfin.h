#pragma once

#include <vector>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bfin.h"
#include "Decoder.h"

#if defined(_MSC_VER)
	#define snprintf _snprintf
	#define restrict __restrict
	#define inline __inline
#else
	#include <stdlib.h>
	#ifdef __cplusplus
	#define restrict __restrict
	#endif
#endif

#define MAX_OPERANDS 20

#define UNCONDITIONAL(c) (((c) == COND_NONE) || ((c) == COND_NONE2))
#define CONDITIONAL(c) (((c) != COND_NONE) && ((c) != COND_NONE2))

#ifdef __cplusplus
#define restrict __restrict

namespace blackfin {
#endif


enum OperationClass {
    BF_PROGCTRL,
    BF_CACHE,
    BF_PUSHPOP,
    BF_PUSHPOPMULTIPLE,
    BF_CCMV,
    BF_CCFLAG,
    // here
    BF_CC2DREG,
    BF_CC2STAT,
    BF_BRCC,
    BF_UJUMP,
    BF_REGMV,
    BF_ALU2op,
    BF_PTR2op,
    BF_LOGI2op,
    BF_COMP3op,
    BF_COMPI2opD,
    BF_COMPI2opP,
    BF_LDSTpmod,
    BF_dagMODim,
    BF_dagMODik,
    BF_dspLDST,
    BF_LDST,
    BF_LDSTiiFP,
    BF_LDSTii,
    BF_LOOPSETUP,
    BF_LDIMMHW,
    BF_JMPABS,
    BF_LDSTidxI,
    BF_LINKAGE,
    BF_DSP32MAC,
    BF_DSP32MUL,
    BF_DSP32ALU,
    BF_DSP32SHIFT,
    BF_DSP32SHIFTIMM,
    BF_PSEUDODBG,
    BF_PSEUDOOCHAR,
    BF_PSEUDODBG_ASSERT,
    BF_ILLEGAL
};

enum OperandClass {
    NONE,
    IMM,
    UIMM,
    MNEMOMIC,
    REG,
    PREG,
    DREG,
    IREG,
    MREG,
    SREG,
    AREG,
    REG_RANGE,
    REG_RANGE_PAIR,
    OPERATOR,
    MEM_ACCESS,
    EXTENDER,
    REGLO,
    REGHI
};

enum OpLiteral {
    OL_NOP,
    OL_RTS,
    OL_RTI,
    OL_RTX,
    OL_RTN,
    OL_RTE,
    OL_IDLE,
    OL_CSYNC,
    OL_SSYNC,
    OL_EMUEXCPT,
    OL_CLI,
    OL_STI,
    OL_JUMP,
    OL_CALL,
    OL_RAISE,
    OL_EXCPT,
    OL_TESTSET,
    OL_PREFETCH,
    OL_FLUSHINV,
    OL_FLUSH,
    OL_IFLUSH,
    OL_IF,
    OL_JUMPS,
    OL_DIVQ,
    OL_DIVS,
    OL_BITTST,
    OL_BITSET,
    OL_BITTGL,
    OL_BITCLR,
    OL_LSETUP,
    OL_JUMPL,
    OL_LINK,
    OL_UNLINK,
    OL_FUFLAG,
    OL_ISFLAG,
    OL_IUFLAG,
    OL_S2RNDFLAG,
    OL_ISS2FLAG,
    OL_MFLAG,
    OL_TFLAG,
    OL_W32FLAG,
    OL_TFUFLAG,
    OL_IHFLAG,
    OL_RND20,
    OL_RND12,
    OL_SMOD,
    OL_NSMOD,
    OL_COMOD,
    OL_SCOMOD,
    OL_ASRMOD,
    OL_ASLMOD,
    OL_RMOD,
    OL_BYTEOP2P,
    OL_THMOD,
    OL_TLMOD,
    OL_RNDHMOD,
    OL_RNDLMOD,
    OL_ABS,
    OL_HIMOD,
    OL_RNDMOD,
    OL_VMOD,
    OL_BYTEOP3P,
    OL_LOMOD,
    OL_SIGN,
    OL_SAA,
    OL_DISALGNEXCPT,
    OL_BYTEOP1P,
    OL_TMOD,
    OL_BYTEOP16P,
    OL_BYTEOP16M,
    OL_MIN,
    OL_MAX,
    OL_BYTEPACK,
    OL_BYTEUNPACK,
    OL_SEARCH,
    OL_GE,
    OL_GT,
    OL_LE,
    OL_LT,
    OL_ASHIFT,
    OL_BY,
    OL_LSHIFT,
    OL_ROT,
    OL_PACK,
    OL_SIGNBITS,
    OL_ONES,
    OL_EXPADJ,
    OL_BITMUX,
    OL_VITMAX,
    OL_EXTRACT,
    OL_DEPOSIT,
    OL_BXORSHIFT,
    OL_BXOR,
    OL_ALIGN8,
    OL_ALIGN16,
    OL_ALIGN24,
    OL_BREV
};

static const char *mnemonics[] = {
    "nop", 
    "rts", "rti", "rtx", "rtn", "rte",
    "idle",
    "csync",
    "ssync",
    "emuexcpt",
    "cli",
    "sti",
    "jump",
    "call",
    "raise",
    "excpt",
    "testset",
    "prefetch",
    "flushinv",
    "flush",
    "iflush",
    "if",
    "jump.s",
    "divq", "divs",
    "bittst",
    "bitset",
    "bittgl",
    "bitclr",
    "lsetup",
    "jump.l",
    "link",
    "unlink",
    "(fu)",
    "(is)",
    "(iu)",
    "(s2rnd)",
    "(iss2)",
    "(m)",
    "(t)",
    "(w32)",
    "(tfu)",
    "(ih)",
    "(rnd20)",
    "(rnd12)",
    "(s)",
    "(ns)",
    "(co)",
    "(sco)",
    "(asr)",
    "(asl)",
    "(r)",
    "byteop2p",
    "(th)",
    "(tl)",
    "(rndh)",
    "(rndl)",
    "abs",
    "(hi)",
    "(rnd)",
    "(v)",
    "byteop3p",
    "(lo)",
    "sign",
    "saa",
    "disalgnexcpt",
    "byteop1p",
    "(t)",
    "byteop16p",
    "byteop16m",
    "min",
    "max",
    "bytepack",
    "byteunpack",
    "search",
    "(ge)",
    "(gt)",
    "(le)",
    "(lt)",
    "ashift",
    "by",
    "lshift",
    "rot",
    "pack",
    "signbits",
    "ones",
    "expadj",
    "bitmux",
    "vit_max",
    "extract",
    "deposit",
    "bxorshift",
    "bxor",
    "align8",
    "align16",
    "align24",
    "(brev)"
};

enum Operator {
    OPER_EQ,
    OPER_PLUSEQ,
    OPER_EQEQ,
    OPER_LT,
    OPER_LTE,
    OPER_LTU,
    OPER_LTEU,
    OPER_OREQ,
    OPER_ANDEQ,
    OPER_XOREQ,
    OPER_PLUS,
    OPER_LSHIFTR,
    OPER_LSHIFTL,
    OPER_ASHIFTR,
    OPER_LSHIFTREQ,
    OPER_LSHIFTLEQ,
    OPER_ASHIFTREQ,
    OPER_MULEQ,
    OPER_EQNEG,
    OPER_EQNOT,
    OPER_COMMA,
    OPER_MINUSEQ,
    OPER_MINUS,
    OPER_AND,
    OPER_OR,
    OPER_XOR,
    OPER_PLUSPLUS,
    OPER_MINUSMINUS,
    OPER_OPENP,
    OPER_CLOSEP,
    OPER_MUL,
    OPER_COLON,
    OPER_MINUSORPLUS,
    OPER_PLUSORPLUS,
    OPER_MINUSORMINUS,
    OPER_PLUSORMINUS,
};

static const char *operator_names[] = {
    " = ", " += ", " == ", " < ", " <= ", " u< ", " u<= ", 
    " |= ", " &= ", " ^= ", " + ", " >> ", " << ", " >>> ",
    " >>= ", " <<= ", " >>>= ", " *= ", " = -", " = ~", ", ",
    " -= ", " - ", " & ", " | ", " ^ ", " ++ ", " -- ", "(",
    ") ", " * ", ":", " -|+ ", " +|+ ", " -|- ", " +|+ "
};

enum Operation {
    OP_NOP,
    OP_RET,
    OP_IDLE,
    OP_EEVENT,
    OP_JMPABS,
    OP_JMPREL,
    OP_CACHE,
    OP_POP,
    OP_PUSH,
    OP_POPMULT,
    OP_PUSHMULT,
    OP_CCMOVE,
    OP_CCFLAG,
    OP_LDIMM,
    OP_LDIMMHW,
    OP_CCDREG,
    OP_CCSTAT,
    OP_BRCC,
    OP_MV,
    OP_ALU2OP,
    OP_ALUADDSHIFTED,
    OP_ALUADDTHENSHIFT,
    OP_ALUDIVPRIM,
    OP_MINUSEQ,
    OP_MVSHIFTED,
    OP_MVADDTHENSHIFT,
    OP_RCADD,
    OP_CCBITTST,
    OP_BITSET,
    OP_BITTGL,
    OP_BITCLR,
    OP_LDREG3,
    OP_LD,
    OP_ST,
    OP_RCPLUSEQ,
    OP_LSETUP,
    OP_CALLABS,
    OP_CALLREL,
    OP_JMPREGABS,
    OP_JMPREGREL,
    OP_CALLREGABS,
    OP_CALLREGREL,
    OP_LINK,
    OP_UNLINK,
    OP_DSPMAC,
    OP_DSPMUL,
    OP_DSPALU,
    OP_RAISE,
    OP_EXCPT,
    OP_DSPSHIFT,
    OP_DSPSHIFTIMM,
    OP_MAX,
    OP_MIN,
    OP_ROT,
    OP_VECTOR16,
    OP_ADD,
    OP_SUB,
    OP_BITOP,

    OP_UNSUPPORTED,
    OP_ILLEGAL
};

enum MemAccessMode {
    MEM_REG,
    MEM_IMM,
    MEM_REGREG,
    MEM_REGIMM,
    MEM_IMMREG,
    MEM_IMMIMM,
    MEM_REGOFF,
    MEM_REGMOD
};

struct MemAccess {
    enum MemAccessMode mode;
    enum Register ptr_reg;
    enum Register idx_reg;
    int ptr_imm;
    int idx_imm;
    int offset;
    enum Operator oper;
    int width;
    bool pre_dec;
    bool post_inc;
};

struct InstructionOperand {
	enum OperandClass cls;
	struct {
        bool is_mem_index;
        bool is_index_offset;
        bool reg_range_pair;
        bool cc_inverted;
        bool sign_extended;
        bool zero_extended;
        bool branch_predicted;
        bool pcrelative;
        int inc_ammount;
	} flags;
	enum Register reg;
    struct MemAccess mem_access;
    struct {
        enum Register top;
        enum Register bottom;
    } r_r;
    struct {
        enum Register top;
        enum Register bottom;
    } r_p;
	union {
        uint32_t uimm;
		int32_t imm;
		double immd;
		float immf;
		uint64_t imm64;
	};
    union {
        enum OpLiteral mnemonic;
        enum Operator operat;
    };
};

struct Instruction {
	struct InstructionOperand operands[MAX_OPERANDS];
	enum Operation operation;
    int operand_count;
    uint32_t opcode;
    uint32_t length;
    struct {
        bool FU;
        bool IS;
        bool IU;
        bool S2RND;
        bool ISS2;
        bool M;
        bool T;
        bool W32;
        bool TFU;
        bool IH;
        bool S;
        bool NS;
        bool CO;
        bool SCO;
        bool ASR;
        bool ASL;
        bool R;
        bool TH;
        bool TL;
        bool RNDH;
        bool RNDL;
        bool HI;
        bool RND;
        bool V;
        bool LO;
    } flags;

};

typedef union _ieee754 {
	uint32_t value;
	struct {
		uint32_t fraction:23;
		uint32_t exponent:8;
		uint32_t sign:1;
	};
	float fvalue;
}ieee754;

typedef union _ieee754_double {
	uint64_t value;
	struct {
		uint64_t fraction:52;
		uint64_t exponent:11;
		uint64_t sign:1;
	};
	double fvalue;
}ieee754_double;

#ifndef __cplusplus
	typedef enum OperandClass OperandClass;
	typedef enum Operation Operation;
	typedef enum Shift Shift;
	typedef enum Condition Condition;
	typedef enum Register Register;
	typedef enum BankedRegister BankedRegister;
	typedef enum SpecRegister SpecRegister;
	typedef enum CoprocRegisterP CoprocRegisterP;
	typedef enum CoprocRegisterC CoprocRegisterC;
	typedef enum DataType DataType;
	typedef enum Iflags Iflags;
	typedef enum EndianSpec EndianSpec;
	typedef enum DsbOption DsbOption;
	typedef struct InstructionOperand InstructionOperand;
	typedef struct Instruction Instruction;
#endif

#ifdef __cplusplus
	extern "C" {
#endif
	uint32_t armv7_decompose(
	        uint32_t instructionValue,
	        Instruction* restrict instruction,
	        uint32_t address,
	        uint32_t littleEndian);

	uint32_t armv7_disassemble(
			Instruction* restrict instruction,
			char* outBuffer,
			uint32_t outBufferSize);

    const char *get_register_name(Register reg);
    const char *get_operator_string(Operator op);
    const char *get_mnemonic_string(OpLiteral mnemonic);
    int get_register_size(Register reg);
    enum Register get_reg_for_reg_part(uint32_t reg);
    /*
	//Helpers for disassembling the instruction operands to strings
	const char* get_operation(Operation operation);
	char* get_full_operation(char* outBuffer, size_t outBufferSize, Instruction* restrict instruction);
	const char* get_vector_data_type(DataType dataType);
	const char* get_register_name(Register reg);
	const char* get_banked_register_name(BankedRegister regb);
	const char* get_coproc_register_c_name(CoprocRegisterC regc);
	const char* get_coproc_register_p_name(CoprocRegisterP regp);
	const char* get_spec_register_name(SpecRegister regs);
	const char* get_iflag(Iflags iflag);
	const char* get_endian(EndianSpec spec);
	const char* get_dsb_option(DsbOption opt);
	const char* get_shift(Shift shift);
	const char* get_condition(Condition cond);
	uint32_t get_register_size(Register reg);
	uint32_t get_register_names(Register reg, const char** regNames, OperandClass type);
    */
#ifdef __cplusplus
	} //end extern "C"
#endif

#ifdef __cplusplus
} //end namespace
#endif
