#include <stdio.h>
#include "shell.h"

//添加操作数的宏定义
#define j 0x02
#define jal 0x03
#define beq 0x04
#define bne 0x05
#define blez 0x06
#define bgtz 0x07
#define addi 0x08
#define addiu 0x09
#define slti 0x0a
#define sltiu 0x0b
#define andi 0x0c
#define ori 0x0d
#define xori 0x0e
#define lui 0x0f
#define lb 0x20
#define lh 0x21
#define lw 0x23
#define lbu 0x24
#define lhu 0x25
#define sb 0x28
#define sh 0x29
#define sw 0x2b

#define bltz 0x00
#define bgez 0x01
#define bltzal 0x10
#define bgezal 0x11

#define sll 0x00
#define srl 0x02
#define sra 0x03
#define sllv 0x04
#define srlv 0x06
#define srav 0x07
#define jr 0x08
#define jalr 0x09
#define add 0x20
#define addu 0x21
#define sub 0x22
#define subu 0x23
#define and 0x24
#define or 0x25
#define xor 0x26
#define nor 0x27
#define slt 0x2a
#define sltu 0x2b
#define mult 0x18
#define mfhi 0x10
#define mflo 0x12
#define mthi 0x11
#define mtlo 0x13
#define multu 0x19
#define div 0x1a
#define divu 0x1b
#define syscall 0x0c

/// Sign extend the 16 bits immediate
uint32_t sign_ext(uint32_t imm) {
    int32_t signed_imm = *((int16_t*)&imm);
    uint32_t extended_imm = *((uint32_t*)&signed_imm);
    return extended_imm;
}

/// Sign extend a byte to 32 bits
uint32_t sign_ext_byte(uint8_t imm) {
    int32_t signed_imm = *((int8_t*)&imm);
    uint32_t extended_imm = *((uint32_t*)&signed_imm);
    return extended_imm;
}

uint32_t sign_ext_half(uint16_t imm) {
    int32_t signed_imm = *((int16_t*)&imm);
    uint32_t extended_imm = *((uint32_t*)&signed_imm);
    return extended_imm;
}

uint32_t zero_ext(uint32_t imm) { return imm; }

uint32_t zero_ext_byte(uint8_t imm) { return imm; }

uint32_t zero_ext_half(uint16_t imm) { return imm; }


void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
    //printf("instruction: %x\n", CURRENT_STATE.PC);
    uint32_t instruction = mem_read_32(CURRENT_STATE.PC);
    uint32_t op = instruction >> 26;//指令的最高6位
    uint32_t rs = (instruction >> 21) & 0x1f;//指令的第22到26位
    uint32_t rt = (instruction >> 16) & 0x1f;//指令的第17到21位。
    uint32_t rd = (instruction >> 11) & 0x1f;//指令的第12到16位
    uint32_t shamt = (instruction >> 6) & 0x1f;//第7到11位
    uint32_t bitween = (instruction >> 16) & 0x1F;//指令的第16到第20位
    uint32_t funct = instruction & 0x3f;//指令的最低6位
    int32_t imm = (int16_t)(instruction & 0xFFFF); // 符号扩展,指令的最低16位
    int32_t uimm = (uint16_t)(instruction & 0xFFFF); // 无符号扩展,指令的最低16位
    uint32_t target = instruction & 0x3ffffff;//最低26位
    printf("PC: %x\n", CURRENT_STATE.PC);
    printf("instruction: %x\n", instruction);
    printf("op: %x\n", op);

    switch (op)
    {
    case 0x00:
        
        printf("funct: %x\n", funct);
        switch(funct)
        {
            
            case sll:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("sll\n");
                break;
            case srl:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("srl\n");
                break;
            case sra:
                NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rt] >> shamt;
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("sra\n");
                break;
            case sllv:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << CURRENT_STATE.REGS[rs];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("sllv\n");
                break;
            case srlv:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> CURRENT_STATE.REGS[rs];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("srlv\n");
                break;
            case srav: 
                NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rt] >> CURRENT_STATE.REGS[rs];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("srav\n");
                break;
            case jr:
                NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
                printf("jr\n");
                break;
            case jalr:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.PC + 4;
                NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
                printf("jalr\n");
                break;
            case add:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("add\n");
                break;
            case addu:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("addu\n");
                break;
            case sub:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("sub\n");
                break;
            case subu:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("subu\n");
                break;
            case and:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("and\n");
                break;
            case or:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("or\n");
                break;
            case xor:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("xor\n");
                break;

            case nor:
                NEXT_STATE.REGS[rd] = ~(CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("nor\n");
                break;
            case slt:
                NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rs] < (int32_t)CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("slt\n");
                break;
            case sltu:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("sltu\n");
                break;
            case mult:
                {
                    int64_t temp = (int64_t)CURRENT_STATE.REGS[rs] * (int64_t)CURRENT_STATE.REGS[rt];
                    NEXT_STATE.HI = temp >> 32;
                    NEXT_STATE.LO = temp & 0xFFFFFFFF;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    printf("mult\n");
                }

                break;
            case mfhi:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("mfhi\n");
                break;
            case mflo:
                NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("mflo\n");
                break;
            case mthi:
                NEXT_STATE.HI = CURRENT_STATE.REGS[rs];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("mthi\n");
                break;
            case mtlo:
                NEXT_STATE.LO = CURRENT_STATE.REGS[rs];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("mtlo\n");
                break;
            case multu:
                {
                    uint64_t temp = (uint64_t)CURRENT_STATE.REGS[rs] * (uint64_t)CURRENT_STATE.REGS[rt];
                    NEXT_STATE.HI = temp >> 32;
                    NEXT_STATE.LO = temp & 0xFFFFFFFF;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    printf("multu\n");
                }
                break;
            case div:
                NEXT_STATE.LO = (int32_t)CURRENT_STATE.REGS[rs] / (int32_t)CURRENT_STATE.REGS[rt];
                NEXT_STATE.HI = (int32_t)CURRENT_STATE.REGS[rs] % (int32_t)CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("div\n");
                break;
            case divu:
                NEXT_STATE.LO = CURRENT_STATE.REGS[rs] / CURRENT_STATE.REGS[rt];
                NEXT_STATE.HI = CURRENT_STATE.REGS[rs] % CURRENT_STATE.REGS[rt];
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                printf("divu\n");
                break;
            case syscall:
                if (CURRENT_STATE.REGS[2] == 0xa) {
                    RUN_BIT = 0;
                }
                else
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                break;
        }
        break;
    case 0x01:
        switch(bitween)
        {
            case bltz:
                if ((int32_t)CURRENT_STATE.REGS[rs] < 0)
                    NEXT_STATE.PC += (imm << 2) + 4;
                printf("bltz\n");
                break;
            case bgez:
                if ((CURRENT_STATE.REGS[rs] & 0x80000000) == 0) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + (sign_ext(imm)<<2);
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
            
                printf("bgez\n");
                break;
            case bltzal:
                NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
                int32_t target11 = (imm << 2);
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                
                if ((int32_t)CURRENT_STATE.REGS[rs] < 0) {
                    NEXT_STATE.PC = CURRENT_STATE.PC + target11;
                }
                printf("bltzal\n");
                break;
            case bgezal:
                NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
                int32_t target12 = (imm << 2);
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                if ((int32_t)CURRENT_STATE.REGS[rs] >= 0) {
                    
                    NEXT_STATE.PC = CURRENT_STATE.PC + target12;
                }
                printf("bgezal\n");
                break;
        }
        break;
    case j:
        NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000) | (target << 2);
        printf("j\n");
        break;
    case jal:
        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
        NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000) | (target << 2);
        printf("jal\n");
        break;
    case beq:
        {
            int32_t offset = imm<<2; // 使用已经提取并符号扩展的imm字段作为offset  
            if(CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt])
            {
                NEXT_STATE.PC = CURRENT_STATE.PC + offset;
            }
            else
            {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
        }
        printf("beq\n");
       
        break;
    case bne:
        {
            uint32_t offset = sign_ext(imm)<<2; // 使用已经提取并符号扩展的imm字段作为offset  
            if(CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt])
            {
                NEXT_STATE.PC = CURRENT_STATE.PC + offset;
            }
            else
            {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
        }
        printf("bne\n");
        break;
    case blez:
        if (rt == 0) {
                if ((CURRENT_STATE.REGS[rs] & 0x80000000) != 0 ||
                    CURRENT_STATE.REGS[rs] == 0) {
                    NEXT_STATE.PC = CURRENT_STATE.PC + (imm<<2) ;
                } else {
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                }
            } else {
                printf("Unknown instruction: %x\n", instruction);
            }
            break;
        printf("blez\n");
        break;
    case bgtz:
        {
            if (rt == 0) {
                if ((CURRENT_STATE.REGS[rs] & 0x80000000) == 0 &&
                    CURRENT_STATE.REGS[rs] != 0) {
                    NEXT_STATE.PC = CURRENT_STATE.PC + (sign_ext(imm)<<2);
                    //printf("PC: 0x%08x\n", NEXT_STATE.PC);
                } else {
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                }
            } else {
                // Illegal instruction
                printf("Illegal rt in BGTZ.\n");
            }
            break;
        }
    case addi:
        NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + imm;
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        break;
    case addiu:
        printf("addiu\n");
        NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + imm;
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        printf("imm: %x\n", imm);
        printf("sign_ext(imm): %x\n",sign_ext(imm));
        break;
    case slti:
        NEXT_STATE.REGS[rt] = (int32_t)CURRENT_STATE.REGS[rs] < imm;
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        printf("slti\n");
        break;
    case sltiu:
        NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] < imm;
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        printf("sltiu\n");
        break;
    case andi:
        NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & imm;
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        printf("andi\n");
        break;
    case ori:
        NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | uimm;
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        printf("ori\n");
        printf("uimm: %x\n", uimm);
        printf("sign_ext(uimm): %x\n",sign_ext(uimm));
        break;
    case xori:
        NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ imm;
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        printf("xori\n");
        break;
    case lui:
        NEXT_STATE.REGS[rt] = imm << 16;
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        printf("lui\n");
        break;
    case lb:
        {
            int32_t offset = imm; // 使用已经提取并符号扩展的imm字段作为offset  
            uint32_t addr = CURRENT_STATE.REGS[rs] + offset;
            uint32_t data = mem_read_32(addr);
            NEXT_STATE.REGS[rt] = (int8_t)data;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        }
        printf("lb\n");
        break;
    case lh:
        {
            int32_t offset = imm; // 使用已经提取并符号扩展的imm字段作为offset  
            uint32_t addr = CURRENT_STATE.REGS[rs] + offset;
            uint32_t data = mem_read_32(addr);
            NEXT_STATE.REGS[rt] = (int16_t)data;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        }
        printf("lh\n");
        break;
    case lw:
        {
            int32_t offset = imm; // 使用已经提取并符号扩展的imm字段作为offset  
            uint32_t addr = CURRENT_STATE.REGS[rs] + offset;
            uint32_t data = mem_read_32(addr);
            NEXT_STATE.REGS[rt] = data;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        }
        printf("lw\n");
        break;
    case lbu:
        {
            int32_t offset = imm; // 使用已经提取并符号扩展的imm字段作为offset  
            uint32_t addr = CURRENT_STATE.REGS[rs] + offset;
            uint32_t data = mem_read_32(addr);
            NEXT_STATE.REGS[rt] = (uint8_t)data;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        }
        printf("lbu\n");
        break;
    case lhu:
        {
            int32_t offset = imm; // 使用已经提取并符号扩展的imm字段作为offset  
            uint32_t addr = CURRENT_STATE.REGS[rs] + offset;
            uint32_t data = mem_read_32(addr);
            NEXT_STATE.REGS[rt] = (uint16_t)data;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        }
        printf("lhu\n");
        break;
    case sb:
        {
            int32_t offset = imm; // 使用已经提取并符号扩展的imm字段作为offset  
            uint32_t addr = CURRENT_STATE.REGS[rs] + offset;
            mem_write_32(addr, CURRENT_STATE.REGS[rt] & 0xff);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        }
        printf("sb\n");
        break;
    case sh:
        {
            int32_t offset = imm; // 使用已经提取并符号扩展的imm字段作为offset  
            uint32_t addr = CURRENT_STATE.REGS[rs] + offset;
            mem_write_32(addr, CURRENT_STATE.REGS[rt] & 0xffff);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        }
        printf("sh\n");
        break;
    case sw:
        {
            int32_t offset = imm; // 使用已经提取并符号扩展的imm字段作为offset  
            uint32_t addr = CURRENT_STATE.REGS[rs] + offset;
            mem_write_32(addr, CURRENT_STATE.REGS[rt]);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        }
        printf("sw\n");
        break;
    default:
        printf("Unknown instruction: %x\n", instruction);
        break;
    }
    //CURRENT_STATE = NEXT_STATE; // 更新当前状态

    char ch;
    ch=getchar();
}
