// Operation numbers:
#define ADD_OP_NUM 0
#define SUB_OP_NUM 1
#define MUL_OP_NUM 2
#define AND_OP_NUM 3
#define OR_OP_NUM 4
#define XOR_OP_NUM 5
#define SLL_OP_NUM 6
#define SRA_OP_NUM 7
#define SRL_OP_NUM 8
#define BEQ_OP_NUM 9
#define BNE_OP_NUM 10
#define BLT_OP_NUM 11
#define BGT_OP_NUM 12
#define BLE_OP_NUM 13
#define BGE_OP_NUM 14
#define JAL_OP_NUM 15
#define LW_OP_NUM 16
#define SW_OP_NUM 17
#define RETI_OP_NUM 18
#define IN_OP_NUM 19
#define OUT_OP_NUM 20
#define HALT_OP_NUM 21

// Register numbers:
#define IMM_REG_NUM 1
#define V0_REG_NUM 2
#define A0_REG_NUM 3
#define A1_REG_NUM 4
#define A2_REG_NUM 5
#define A3_REG_NUM 6
#define T0_REG_NUM 7
#define T1_REG_NUM 8
#define T2_REG_NUM 9
#define S0_REG_NUM 10
#define S1_REG_NUM 11
#define S2_REG_NUM 12
#define GP_REG_NUM 13
#define SP_REG_NUM 14
#define RA_REG_NUM 15

// I/O register numbers:
#define IRQ0ENABLE 0
#define IRQ1ENABLE 1
#define IRQ2ENABLE 2
#define IRQ0STATUS 3
#define IRQ1STATUS 4
#define IRQ2STATUS 5
#define IRQ_HANDLER 6
#define IRQ_RETURN 7
#define CLKS 8
#define LEDS 9
#define DISPLAY7SEG 10
#define TIMER_ENABLE 11
#define TIMER_CURRENT 12
#define TIMER_MAX 13
#define DISK_CMD 14
#define DISK_SECTOR 15
#define DISK_BUFFER 16
#define DISK_STATUS 17
#define RESERVED1 18
#define RESERVED2 19
#define MONITOR_ADDR 20
#define MONITOR_DATA 21
#define MONITOR_CMD 22

// I/O consts:
#define BUSY 1
#define FREE 0
#define NO_COMMAND 0
#define READ_SECTOR 1
#define WRITE_SECTOR 2
#define NUM_SECTORS 128
#define SIZE_SECTOR 128
#define DISK_CYCLES 1024
#define MONITOR_WIDTH 256
#define MONITOR_HEIGHT 256

// General consts:
#define TRUE 1
#define FALSE 0
#define NUM_REGISTERS 16
#define NUM_IO_REGISTERS 23
#define MEMORY_SIZE 4096
#define INSTRUCTION_LENGTH_HEX 6
#define PRINT_FROM_REGISTER_NUM 2
#define LONG_BYTES_NUM 16
#define NO_INTERRUPT -1

const char *IO_REGISTER_NAMES[NUM_IO_REGISTERS];

/* Initializes the relevant globals for the simulation */
void initialize_globals();