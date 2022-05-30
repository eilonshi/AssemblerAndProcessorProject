// Operation numbers:
#define ADD_OP_NUM  0
#define SUB_OP_NUM  1
#define MUL_OP_NUM  2
#define AND_OP_NUM  3
#define OR_OP_NUM   4
#define XOR_OP_NUM  5
#define SLL_OP_NUM  6
#define SRA_OP_NUM  7
#define SRL_OP_NUM  8
#define BEQ_OP_NUM  9
#define BNE_OP_NUM  10
#define BLT_OP_NUM  11
#define BGT_OP_NUM  12
#define BLE_OP_NUM  13
#define BGE_OP_NUM  14
#define JAL_OP_NUM  15
#define LW_OP_NUM   16
#define SW_OP_NUM   17
#define RETI_OP_NUM 18
#define IN_OP_NUM   19
#define OUT_OP_NUM  20
#define HALT_OP_NUM 21

// Register numbers:
#define IMM_REG_NUM  1
#define V0_REG_NUM   2
#define A0_REG_NUM   3
#define A1_REG_NUM   4
#define A2_REG_NUM   5
#define A3_REG_NUM   6
#define T0_REG_NUM   7
#define T1_REG_NUM   8
#define T2_REG_NUM   9
#define S0_REG_NUM   10
#define S1_REG_NUM   11
#define S2_REG_NUM   12
#define GP_REG_NUM   13
#define SP_REG_NUM   14
#define RA_REG_NUM   15

// General consts:
#define NUM_REGISTERS    16
#define NUM_IO_REGISTERS 23
#define MEMORY_SIZE      4096

// Globals:
extern state;