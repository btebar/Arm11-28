
#ifndef PROJECT_USEFULCONSTANTS_H
#define PROJECT_USEFULCONSTANTS_H

//instead of having a 32 bit mask, we'll shift the bits stored in the CPSR to get the remaining 4 bits
#define N_MASK 0x8
#define Z_MASK 0X4
#define C_MASK 0x2
#define V_MASK 0x1
#define Branch_MASK 0x08000000
#define SDT_MASK 0x04000000
#define Multiply_MASK1 0x00000080
#define Multiply_MASK2 0x00000010
#define Z_MASK_32 0x40000000
#define I_MASK_32 0x02000000
#define C_MASK_32 0x20000000
#define N_MASK_32 0x80000000
#define MEM_BOUNDS 65536
#define BYTES_2 16
#define BYTE_1 8
#define BYTES_3 24
#define OTHERSIDE 31
#define Nposition 28
#define BYTES_4 32
#define sizeReg 4
#define Rnbit 16
#define Rdbit 12
#define RnbitMul 12
#define RdbitMul 16
#define RmbitMul 0
#define RsbitMul 8
#define sizeOffset 12
#define condBits 28
#define sizeopcode 4
#define OpcodeBit 21
#define Cbit 29
#define Vbit 28
#define Zbit 30
#define Nbit 31
#define bits_4 4

#define Ibit 25
#define Sbit 20
#define Pbit 24
#define Ubit 23
#define Lbit 20
#define Abit 21

typedef enum {
    EQ = 0,
    NE = 1,
    GE = 0xA,
    LT = 0xB,
    GT = 0xC,
    LE = 0xD,
    AL = 0xE
}cond_code;

typedef enum {
    Halt,
    None,
    Branch,
    Mult,
    SDT,
    DProc
} instruction_type;

typedef enum{
    AND = 0000,
    EOR = 0001,
    SUB = 0010,
    RSB = 0011,
    ADD = 0100,
    TST = 1000,
    TEQ = 1001,
    CMP = 1010,
    ORR = 1100,
    MOV = 1101
} OPCODE;

typedef enum {
    LSL = 00,
    LSR = 01,
    ASR = 10,
    ROR = 11,
} SHIFT_CODE;

typedef uint32_t word_t;
typedef int cond;






#endif //PROJECT_USEFULCONSTANTS_H
