//
// Created by boris on 29/09/2020.
//

#include "CPU.h"
#include <time.h>

CPU::CPU(BUS &bus) : bus(bus) {

    using a = CPU;
    lookup =
            {
                    {"BRK", &a::BRK, &a::IMM, 7},
                    {"ORA", &a::ORA, &a::IZX, 6},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 8},
                    {"???", &a::NOP, &a::IMP, 3},
                    {"ORA", &a::ORA, &a::ZP0, 3},
                    {"ASL", &a::ASL, &a::ZP0, 5},
                    {"???", &a::XXX, &a::IMP, 5},
                    {"PHP", &a::PHP, &a::IMP, 3},
                    {"ORA", &a::ORA, &a::IMM, 2},
                    {"ASL", &a::ASL, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"???", &a::NOP, &a::IMP, 4},
                    {"ORA", &a::ORA, &a::ABS, 4},
                    {"ASL", &a::ASL, &a::ABS, 6},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"BPL", &a::BPL, &a::REL, 2},
                    {"ORA", &a::ORA, &a::IZY, 5},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 8},
                    {"???", &a::NOP, &a::IMP, 4},
                    {"ORA", &a::ORA, &a::ZPX, 4},
                    {"ASL", &a::ASL, &a::ZPX, 6},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"CLC", &a::CLC, &a::IMP, 2},
                    {"ORA", &a::ORA, &a::ABY, 4},
                    {"???", &a::NOP, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 7},
                    {"???", &a::NOP, &a::IMP, 4},
                    {"ORA", &a::ORA, &a::ABX, 4},
                    {"ASL", &a::ASL, &a::ABX, 7},
                    {"???", &a::XXX, &a::IMP, 7},
                    {"JSR", &a::JSR, &a::ABS, 6},
                    {"AND", &a::AND, &a::IZX, 6},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 8},
                    {"BIT", &a::BIT, &a::ZP0, 3},
                    {"AND", &a::AND, &a::ZP0, 3},
                    {"ROL", &a::ROL, &a::ZP0, 5},
                    {"???", &a::XXX, &a::IMP, 5},
                    {"PLP", &a::PLP, &a::IMP, 4},
                    {"AND", &a::AND, &a::IMM, 2},
                    {"ROL", &a::ROL, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"BIT", &a::BIT, &a::ABS, 4},
                    {"AND", &a::AND, &a::ABS, 4},
                    {"ROL", &a::ROL, &a::ABS, 6},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"BMI", &a::BMI, &a::REL, 2},
                    {"AND", &a::AND, &a::IZY, 5},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 8},
                    {"???", &a::NOP, &a::IMP, 4},
                    {"AND", &a::AND, &a::ZPX, 4},
                    {"ROL", &a::ROL, &a::ZPX, 6},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"SEC", &a::SEC, &a::IMP, 2},
                    {"AND", &a::AND, &a::ABY, 4},
                    {"???", &a::NOP, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 7},
                    {"???", &a::NOP, &a::IMP, 4},
                    {"AND", &a::AND, &a::ABX, 4},
                    {"ROL", &a::ROL, &a::ABX, 7},
                    {"???", &a::XXX, &a::IMP, 7},
                    {"RTI", &a::RTI, &a::IMP, 6},
                    {"EOR", &a::EOR, &a::IZX, 6},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 8},
                    {"???", &a::NOP, &a::IMP, 3},
                    {"EOR", &a::EOR, &a::ZP0, 3},
                    {"LSR", &a::LSR, &a::ZP0, 5},
                    {"???", &a::XXX, &a::IMP, 5},
                    {"PHA", &a::PHA, &a::IMP, 3},
                    {"EOR", &a::EOR, &a::IMM, 2},
                    {"LSR", &a::LSR, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"JMP", &a::JMP, &a::ABS, 3},
                    {"EOR", &a::EOR, &a::ABS, 4},
                    {"LSR", &a::LSR, &a::ABS, 6},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"BVC", &a::BVC, &a::REL, 2},
                    {"EOR", &a::EOR, &a::IZY, 5},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 8},
                    {"???", &a::NOP, &a::IMP, 4},
                    {"EOR", &a::EOR, &a::ZPX, 4},
                    {"LSR", &a::LSR, &a::ZPX, 6},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"CLI", &a::CLI, &a::IMP, 2},
                    {"EOR", &a::EOR, &a::ABY, 4},
                    {"???", &a::NOP, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 7},
                    {"???", &a::NOP, &a::IMP, 4},
                    {"EOR", &a::EOR, &a::ABX, 4},
                    {"LSR", &a::LSR, &a::ABX, 7},
                    {"???", &a::XXX, &a::IMP, 7},
                    {"RTS", &a::RTS, &a::IMP, 6},
                    {"ADC", &a::ADC, &a::IZX, 6},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 8},
                    {"???", &a::NOP, &a::IMP, 3},
                    {"ADC", &a::ADC, &a::ZP0, 3},
                    {"ROR", &a::ROR, &a::ZP0, 5},
                    {"???", &a::XXX, &a::IMP, 5},
                    {"PLA", &a::PLA, &a::IMP, 4},
                    {"ADC", &a::ADC, &a::IMM, 2},
                    {"ROR", &a::ROR, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"JMP", &a::JMP, &a::IND, 5},
                    {"ADC", &a::ADC, &a::ABS, 4},
                    {"ROR", &a::ROR, &a::ABS, 6},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"BVS", &a::BVS, &a::REL, 2},
                    {"ADC", &a::ADC, &a::IZY, 5},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 8},
                    {"???", &a::NOP, &a::IMP, 4},
                    {"ADC", &a::ADC, &a::ZPX, 4},
                    {"ROR", &a::ROR, &a::ZPX, 6},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"SEI", &a::SEI, &a::IMP, 2},
                    {"ADC", &a::ADC, &a::ABY, 4},
                    {"???", &a::NOP, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 7},
                    {"???", &a::NOP, &a::IMP, 4},
                    {"ADC", &a::ADC, &a::ABX, 4},
                    {"ROR", &a::ROR, &a::ABX, 7},
                    {"???", &a::XXX, &a::IMP, 7},
                    {"???", &a::NOP, &a::IMP, 2},
                    {"STA", &a::STA, &a::IZX, 6},
                    {"???", &a::NOP, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"STY", &a::STY, &a::ZP0, 3},
                    {"STA", &a::STA, &a::ZP0, 3},
                    {"STX", &a::STX, &a::ZP0, 3},
                    {"???", &a::XXX, &a::IMP, 3},
                    {"DEY", &a::DEY, &a::IMP, 2},
                    {"???", &a::NOP, &a::IMP, 2},
                    {"TXA", &a::TXA, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"STY", &a::STY, &a::ABS, 4},
                    {"STA", &a::STA, &a::ABS, 4},
                    {"STX", &a::STX, &a::ABS, 4},
                    {"???", &a::XXX, &a::IMP, 4},
                    {"BCC", &a::BCC, &a::REL, 2},
                    {"STA", &a::STA, &a::IZY, 6},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"STY", &a::STY, &a::ZPX, 4},
                    {"STA", &a::STA, &a::ZPX, 4},
                    {"STX", &a::STX, &a::ZPY, 4},
                    {"???", &a::XXX, &a::IMP, 4},
                    {"TYA", &a::TYA, &a::IMP, 2},
                    {"STA", &a::STA, &a::ABY, 5},
                    {"TXS", &a::TXS, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 5},
                    {"???", &a::NOP, &a::IMP, 5},
                    {"STA", &a::STA, &a::ABX, 5},
                    {"???", &a::XXX, &a::IMP, 5},
                    {"???", &a::XXX, &a::IMP, 5},
                    {"LDY", &a::LDY, &a::IMM, 2},
                    {"LDA", &a::LDA, &a::IZX, 6},
                    {"LDX", &a::LDX, &a::IMM, 2},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"LDY", &a::LDY, &a::ZP0, 3},
                    {"LDA", &a::LDA, &a::ZP0, 3},
                    {"LDX", &a::LDX, &a::ZP0, 3},
                    {"???", &a::XXX, &a::IMP, 3},
                    {"TAY", &a::TAY, &a::IMP, 2},
                    {"LDA", &a::LDA, &a::IMM, 2},
                    {"TAX", &a::TAX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"LDY", &a::LDY, &a::ABS, 4},
                    {"LDA", &a::LDA, &a::ABS, 4},
                    {"LDX", &a::LDX, &a::ABS, 4},
                    {"???", &a::XXX, &a::IMP, 4},
                    {"BCS", &a::BCS, &a::REL, 2},
                    {"LDA", &a::LDA, &a::IZY, 5},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 5},
                    {"LDY", &a::LDY, &a::ZPX, 4},
                    {"LDA", &a::LDA, &a::ZPX, 4},
                    {"LDX", &a::LDX, &a::ZPY, 4},
                    {"???", &a::XXX, &a::IMP, 4},
                    {"CLV", &a::CLV, &a::IMP, 2},
                    {"LDA", &a::LDA, &a::ABY, 4},
                    {"TSX", &a::TSX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 4},
                    {"LDY", &a::LDY, &a::ABX, 4},
                    {"LDA", &a::LDA, &a::ABX, 4},
                    {"LDX", &a::LDX, &a::ABY, 4},
                    {"???", &a::XXX, &a::IMP, 4},
                    {"CPY", &a::CPY, &a::IMM, 2},
                    {"CMP", &a::CMP, &a::IZX, 6},
                    {"???", &a::NOP, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 8},
                    {"CPY", &a::CPY, &a::ZP0, 3},
                    {"CMP", &a::CMP, &a::ZP0, 3},
                    {"DEC", &a::DEC, &a::ZP0, 5},
                    {"???", &a::XXX, &a::IMP, 5},
                    {"INY", &a::INY, &a::IMP, 2},
                    {"CMP", &a::CMP, &a::IMM, 2},
                    {"DEX", &a::DEX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"CPY", &a::CPY, &a::ABS, 4},
                    {"CMP", &a::CMP, &a::ABS, 4},
                    {"DEC", &a::DEC, &a::ABS, 6},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"BNE", &a::BNE, &a::REL, 2},
                    {"CMP", &a::CMP, &a::IZY, 5},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 8},
                    {"???", &a::NOP, &a::IMP, 4},
                    {"CMP", &a::CMP, &a::ZPX, 4},
                    {"DEC", &a::DEC, &a::ZPX, 6},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"CLD", &a::CLD, &a::IMP, 2},
                    {"CMP", &a::CMP, &a::ABY, 4},
                    {"NOP", &a::NOP, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 7},
                    {"???", &a::NOP, &a::IMP, 4},
                    {"CMP", &a::CMP, &a::ABX, 4},
                    {"DEC", &a::DEC, &a::ABX, 7},
                    {"???", &a::XXX, &a::IMP, 7},
                    {"CPX", &a::CPX, &a::IMM, 2},
                    {"SBC", &a::SBC, &a::IZX, 6},
                    {"???", &a::NOP, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 8},
                    {"CPX", &a::CPX, &a::ZP0, 3},
                    {"SBC", &a::SBC, &a::ZP0, 3},
                    {"INC", &a::INC, &a::ZP0, 5},
                    {"???", &a::XXX, &a::IMP, 5},
                    {"INX", &a::INX, &a::IMP, 2},
                    {"SBC", &a::SBC, &a::IMM, 2},
                    {"NOP", &a::NOP, &a::IMP, 2},
                    {"???", &a::SBC, &a::IMP, 2},
                    {"CPX", &a::CPX, &a::ABS, 4},
                    {"SBC", &a::SBC, &a::ABS, 4},
                    {"INC", &a::INC, &a::ABS, 6},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"BEQ", &a::BEQ, &a::REL, 2},
                    {"SBC", &a::SBC, &a::IZY, 5},
                    {"???", &a::XXX, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 8},
                    {"???", &a::NOP, &a::IMP, 4},
                    {"SBC", &a::SBC, &a::ZPX, 4},
                    {"INC", &a::INC, &a::ZPX, 6},
                    {"???", &a::XXX, &a::IMP, 6},
                    {"SED", &a::SED, &a::IMP, 2},
                    {"SBC", &a::SBC, &a::ABY, 4},
                    {"NOP", &a::NOP, &a::IMP, 2},
                    {"???", &a::XXX, &a::IMP, 7},
                    {"???", &a::NOP, &a::IMP, 4},
                    {"SBC", &a::SBC, &a::ABX, 4},
                    {"INC", &a::INC, &a::ABX, 7},
                    {"???", &a::XXX, &a::IMP, 7},
            };

}


uint8_t CPU::IMP() {
    fetched = a;
    return 0;
}

uint8_t CPU::IMM() {
    addr_abs = pc;
    pc++;
    return 0;
}

uint8_t CPU::ZP0() {
    addr_abs = read(pc);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t CPU::ZPX() {
    addr_abs = read(pc) + x;
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t CPU::ZPY() {
    addr_abs = read(pc) + y;
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t CPU::REL() {
    uint8_t tmp = read(pc);
    pc++;
    addr_rel = uint16_t(int8_t(tmp));

    return 0;
}

uint8_t CPU::ABS() {

    uint16_t lo = read(pc);
    pc++;

    uint16_t hi = read(pc);
    pc++;
    addr_abs = (hi << 8) | lo;

    return 0;
}

uint8_t CPU::ABX() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;
    addr_abs = (hi << 8) | lo;
    addr_abs += uint16_t(x);
    return 0;
}

uint8_t CPU::ABY() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;
    addr_abs = (hi << 8) | lo;
    addr_abs += uint16_t(y);
    return 0;
}

uint8_t CPU::IND() {
    uint16_t ptr_lo = read(pc);
    pc++;
    uint16_t ptr_hi = read(pc);
    pc++;

    uint16_t ptr = (ptr_hi << 8) | ptr_lo;

    if (ptr_lo == 0x00FF) // Simulate page boundary hardware bug
    {
        addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
    }
    else // Behave normally
    {
        addr_abs = (uint16_t(read(ptr + 1))<< 8) | read(ptr + 0);
    }

    return 0;
}

uint8_t CPU::IZX() {
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read((t + x) & 0x00FF);
    uint16_t hi = read((t + x + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t CPU::IZY() {
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read(t & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    addr_abs += y;
    return 0;
}


uint8_t CPU::ADC() {
    fetch();
    uint16_t tmp = uint16_t(a) + uint16_t(fetched) + uint16_t(getFlag(C));

    setFlag(C, tmp & 0x0100);
    setFlag(Z, (tmp & 0x00FF) == 0);
    setFlag(N, tmp & 0x0080);
    setFlag(V, (a ^ tmp) & (fetched ^ tmp) & 0x0080);

    a = tmp & 0x00FF;

    return 0;
}

uint8_t CPU::AND() {
    fetch();
    a = a & fetched;

    setFlag(Z, a == 0);
    setFlag(N, a & 0x80);

    return 0;
}

uint8_t CPU::ASL() {
    fetch();

    uint16_t tmp = (uint16_t) a << 1;

    setFlag(C, tmp & 0x0100);
    setFlag(Z, (tmp & 0x00FF) == 0);
    setFlag(N, tmp & 0x0080);

    if (lookup[opcode].addrmode == &CPU::IMP)
        a = tmp & 0x00FF;
    else
        write(addr_abs, tmp & 0x00FF);
    return 0;
}

uint8_t CPU::BCS() {
    if (getFlag(C)) {
        cycle++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycle++;

        pc = pc + addr_rel;

    }
    return 0;
}

uint8_t CPU::BEQ() {
    if (getFlag(Z)) {
        cycle++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycle++;

        pc = pc + addr_rel;
    }
    return 0;
}

uint8_t CPU::BIT() {
    fetch();
    uint8_t tmp = a & fetched;
    setFlag(Z, tmp == 0);
    setFlag(V, fetched & 0x40);
    setFlag(N, fetched & 0x80);


    return 0;
}

uint8_t CPU::BMI() {
    if (getFlag(N)) {
        cycle++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycle++;
        pc = pc + addr_rel;
    }
    return 0;
}

uint8_t CPU::BNE() {
    if (getFlag(Z) == 0) {
        cycle++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycle++;

        pc = pc + addr_rel;
    }
    return 0;
}

uint8_t CPU::BPL() {
    if (getFlag(N) == 0) {
        cycle++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycle++;
        pc = pc + addr_rel;
    }
    return 0;
}

uint8_t CPU::BRK() {
    pc++;
    setFlag(I, 1);

    write(0x0100 + s, (pc >> 8) & 0x00FF);
    s--;
    write(0x0100 + s, pc & 0x00FF);
    s--;
    setFlag(B, 1);
    write(0x0100 + s, status);
    s--;
    setFlag(B, 0);

    uint8_t al = read(0xFFFE);
    uint8_t ah = read(0xFFFF);
    pc = (uint16_t(ah) << 8) | uint16_t(al);
    return 0;
}

uint8_t CPU::BVC() {
    if (getFlag(V) == 0) {
        cycle++;
        addr_abs = pc + addr_rel;
        pc = pc + addr_rel;
    }
    return 0;
}

uint8_t CPU::BVS() {
    if (getFlag(V) == 1) {
        cycle++;
        addr_abs = pc + addr_rel;
        pc = pc + addr_rel;
    }
    return 0;
}

uint8_t CPU::CLC() {
    setFlag(C, 0);
    return 0;
}

uint8_t CPU::CLD() {
    setFlag(D, 0);
    return 0;
}

uint8_t CPU::CLI() {
    setFlag(I, 0);
    return 0;
}

uint8_t CPU::CLV() {
    setFlag(V, 0);
    return 0;
}

uint8_t CPU::CMP() {
    fetch();
    uint16_t tmp = uint16_t(a) - uint16_t(fetched);
    setFlag(N, tmp & 0x0080);
    setFlag(Z, (tmp & 0x00FF) == 0);
    setFlag(C, a >= fetched);
    return 0;
}

uint8_t CPU::CPX() {
    fetch();
    uint16_t tmp = uint16_t(x) - uint16_t(fetched);
    setFlag(N, tmp & 0x0080);
    setFlag(Z, (tmp & 0x00FF) == 0);
    setFlag(C, x >= fetched);
    return 0;
}

uint8_t CPU::CPY() {
    fetch();
    uint16_t tmp = uint16_t(y) - uint16_t(fetched);
    setFlag(N, tmp & 0x0080);
    setFlag(Z, (tmp & 0x00FF) == 0);
    setFlag(C, y >= fetched);

    return 0;
}

uint8_t CPU::DEC() {
    fetch();
    uint8_t tmp = fetched - 1;
    setFlag(N, tmp & 0x80);
    setFlag(Z, tmp == 0);
    write(addr_abs, tmp);
    return 0;
}

uint8_t CPU::DEX() {
    x--;
    setFlag(N, x & 0x80);
    setFlag(Z, x == 0);

    return 0;
}

uint8_t CPU::DEY() {
    y--;
    setFlag(N, y & 0x80);
    setFlag(Z, y == 0);
    return 0;
}

uint8_t CPU::EOR() {
    fetch();
    a = a ^ fetched;
    setFlag(N, a & 0x80);
    setFlag(Z, a == 0);
    return 0;
}

uint8_t CPU::INC() {
    fetch();
    uint8_t tmp = fetched + 1;
    setFlag(N, tmp & 0x80);
    setFlag(Z, tmp == 0);
    write(addr_abs, tmp);
    return 0;
}

uint8_t CPU::INX() {
    x++;
    setFlag(N, x & 0x80);
    setFlag(Z, x == 0);

    return 0;
}

uint8_t CPU::INY() {
    y++;
    setFlag(N, y & 0x80);
    setFlag(Z, y == 0);
    return 0;
}

uint8_t CPU::JMP() {
    pc = addr_abs;
    return 0;
}

uint8_t CPU::JSR() {
    pc--;

    write(0x0100 + s, (pc >> 8) & 0x00FF);
    s--;
    write(0x0100 + s, pc & 0x00FF);
    s--;

    pc = addr_abs;
    return 0;
}

uint8_t CPU::LDA() {
    fetch();
    a = fetched;
    setFlag(N, a & 0x80);
    setFlag(Z, a == 0);
    return 0;
}

uint8_t CPU::LDX() {
    //std::cout<< std::hex <<"LDX_address: " << int(addr_abs) << std::endl;

    fetch();
    x = fetched;
    setFlag(N, x & 0x80);
    setFlag(Z, x == 0);
    return 0;
}

uint8_t CPU::LDY() {
    //std::cout<< std::hex <<"LDY_address: " << int(addr_abs) << std::endl;

    fetch();
    y = fetched;
    setFlag(N, y & 0x80);
    setFlag(Z, y == 0);
    return 0;
}

uint8_t CPU::LSR() {
    fetch();
    uint8_t tmp = fetched >> 1;
    setFlag(C, fetched & 0x01);
    setFlag(N, tmp & 0x80);
    setFlag(Z, tmp == 0);
    if (lookup[opcode].addrmode == &CPU::IMP)
        a = tmp & 0x00FF;
    else
        write(addr_abs, tmp & 0x00FF);
    return 0;
}

uint8_t CPU::NOP() {
    return 0;
}

uint8_t CPU::ORA() {
    fetch();
    a = a | fetched;

    setFlag(Z, a == 0);
    setFlag(N, a & 0x80);
    return 0;
}

uint8_t CPU::PHA() {
    write(0x0100 + s, a);
    s--;
    return 0;
}

uint8_t CPU::PHP() {
    write(0x0100 + s, status | B | U);
    setFlag(B, 0); //Flag test error with it;
    setFlag(U, 0);
    s--;

    return 0;
}


uint8_t CPU::PLA() {
    s++;
    a = read(0x0100 + s);
    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);
    return 0;
}

uint8_t CPU::PLP() {
    s++;
    status = read(0x0100 + s);
    setFlag(U, 1);
    return 0;
}

uint8_t CPU::ROL() {
    fetch();
    //std::cout << "ROL fetched: " << std::hex << (uint16_t)fetched << std::endl;
    uint16_t tmp = (uint16_t(fetched) << 1) | uint16_t(getFlag(C));
    setFlag(FLAGS::C, tmp & 0xFF00);
    setFlag(FLAGS::N, tmp & 0x0080);
    setFlag(FLAGS::Z, (tmp & 0x00FF) == 0);

    if (lookup[opcode].addrmode == &CPU::IMP)
        a = tmp & 0x00FF;
    else
        write(addr_abs, tmp & 0x00FF);
    return 0;
}

uint8_t CPU::ROR() {
    fetch();
    uint8_t tmp = (uint8_t(getFlag(FLAGS::C)) << 7) | (fetched >> 1);
    setFlag(C, fetched & 0x0001);
    setFlag(N, tmp & 0x0080);
    setFlag(Z, (tmp & 0x00FF) == 0);
    if (lookup[opcode].addrmode == &CPU::IMP)
        a = tmp & 0x00FF;
    else
        write(addr_abs, tmp & 0x00FF);

    return 0;
}

uint8_t CPU::RTI() {
    s++;
    status = read(0x0100 + s);
    setFlag(I, 1);
    setFlag(B, 1);

    s++;
    pc = uint16_t(read(0x0100 + s));
    s++;
    pc |= read(0x0100 + s) << 8;

    return 0;
}

uint8_t CPU::RTS() {
    s++;
    pc = uint16_t(read(0x0100 + s));
    s++;
    pc |= uint16_t(read(0x0100 + s)) << 8;

    pc++;
    return 0;
}

uint8_t CPU::SBC() {
    fetch();

    uint16_t value = ((uint16_t) fetched) ^0x00FF;

    uint16_t tmp = (uint16_t) a + value + (uint16_t) getFlag(C);

    setFlag(C, tmp & 0xFF00);
    setFlag(Z, (tmp & 0x00FF) == 0);
    setFlag(N, tmp & 0x80);
    setFlag(V, (tmp ^ (uint16_t) a) & (tmp ^ value) & 0x0080);

    a = tmp & 0x00FF;

    return 0;
}

uint8_t CPU::SEC() {
    setFlag(FLAGS::C, 1);
    return 0;
}

uint8_t CPU::SED() {
    setFlag(FLAGS::D, 1);
    return 0;
}

uint8_t CPU::SEI() {
    setFlag(FLAGS::I, 1);
    return 0;
}

uint8_t CPU::STA() {
    //std::cout<< std::hex <<"STA_address: " << int(addr_abs) << std::endl;
    write(addr_abs, a);
    return 0;
}

uint8_t CPU::STX() {
    //std::cout<< std::hex <<"STX_address: " << int(addr_abs) << std::endl;
    write(addr_abs, x);
    return 0;
}

uint8_t CPU::STY() {
    //std::cout<< std::hex <<"STY_address: " << int(addr_abs) << std::endl;
    write(addr_abs, y);
    return 0;
}

uint8_t CPU::TAX() {
    x = a;

    setFlag(FLAGS::N, x & 0x80);
    setFlag(FLAGS::Z, x == 0);
    return 0;
}

uint8_t CPU::TAY() {
    y = a;

    setFlag(FLAGS::N, y & 0x80);
    setFlag(FLAGS::Z, y == 0);
    return 0;
}

uint8_t CPU::TSX() {
    x = s;

    setFlag(FLAGS::N, x & 0x80);
    setFlag(FLAGS::Z, x == 0);
    return 0;
}

uint8_t CPU::TXA() {
    a = x;

    setFlag(FLAGS::N, a & 0x80);
    setFlag(FLAGS::Z, a == 0);
    return 0;
}

uint8_t CPU::TXS() {
    s = x;

    //setFlag(FLAGS::N, s & 0x80);
    //setFlag(FLAGS::Z, s == 0);
    return 0;
}

uint8_t CPU::TYA() {
    a = y;

    setFlag(FLAGS::N, a & 0x80);
    setFlag(FLAGS::Z, a == 0);
    return 0;
}

uint8_t CPU::BCC() {
    if (getFlag(C) == 0) {
        cycle++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycle++;

        pc = pc + addr_rel;


    }
    return 0;
}

uint8_t CPU::XXX() {
    return 0;
}

uint8_t CPU::getFlag(CPU::FLAGS p) {
    return ((status & p) > 0) ? 1 : 0;
}

void CPU::setFlag(CPU::FLAGS p, bool a) {
    a ? (status |= p) : (status &= ~p);
}

uint8_t CPU::read(uint16_t addr) {

    return bus.cpuRead(addr);
    //return ram[addr];
}

void CPU::write(uint16_t addr, uint8_t src) {
    bus.cpuWrite(addr, src);
    //ram[addr] = src;
}

void CPU::reset() {
    bus.memlog(0xFFF0);
    addr_abs = 0xFFFC;
    uint16_t lo = read(addr_abs + 0);
    uint16_t hi = read(addr_abs + 1);

    // Set it
    pc = (hi << 8) | lo;
    // Reset internal registers
    a = 0;
    x = 0;
    y = 0;
    s = 0xFD;
    status = 0x00 | U;

    // Clear internal helper variables
    addr_rel = 0x0000;
    addr_abs = 0x0000;
    fetched = 0x00;

    cycle = 8;
}

void CPU::fetch() {
    if (lookup[opcode].addrmode != &CPU::IMP){
        fetched = read(addr_abs);
    }

}


void CPU::IRQ() {
    if (getFlag(I) == 0) {
        write(0x0100 + s, (pc >> 8) & 0x00FF);
        s--;
        write(0x0100 + s, pc & 0x00FF);
        s--;
        write(0x0100 + s, status);
        s--;
        uint8_t al = read(0xFFFA);
        uint8_t ah = read(0xFFFB);
        pc = (uint16_t(ah) << 8) | uint16_t(al);
        setFlag(I, 1);
        setFlag(B, 0);
        setFlag(U, 1);

        cycle = 8;
    }
}

void CPU::NMI() {
    write(0x0100 + s, (pc >> 8) & 0x00FF);
    s--;
    write(0x0100 + s, pc & 0x00FF);
    s--;
    write(0x0100 + s, status);
    s--;
    uint8_t al = read(0xFFFA);
    uint8_t ah = read(0xFFFB);
    pc = (uint16_t(ah) << 8) | uint16_t(al);
    setFlag(I, 1);
    setFlag(B, 0);
    setFlag(U, 1);


    cycle = 8;
}

void CPU::clock() {
    if (cycle == 0) {

        opcode = read(pc);
        if(pc == 0xDBB5){
            //debug_flag = true;
        }
        if (debug_flag) {

            show_info();

            //debug_flag = false;
        }

        pc++;

        (this->*lookup[opcode].addrmode)();
        (this->*lookup[opcode].operate)();
        cycle = lookup[opcode].cycles;

        if(debug_flag) {
            system("pause");
        }
    }
    cycle--;
    global_cycle++;
}

void CPU::show_info() {
//    bus.memlog(addr_abs);
//    printf("\n");

    printf("A: %02X X: %02X Y: %02X P: %02X SP: %02X CYC:%d PC %04X\n", a, x, y, status&0xCF, s, global_cycle - 1, pc);

//    printf("Opcode: %02X (%s )\n", opcode, (lookup[opcode].name).c_str());
//    printf("           NV1BDIZC\n");
//    printf("cpu_reg P: ");
//    std::cout << bool(getFlag(N)) << bool(getFlag(V)) << bool(getFlag(U)) << bool(getFlag(B)) << bool(getFlag(D))
//              << bool(getFlag(I)) << bool(getFlag(Z)) << bool(getFlag(C)) << std::endl;



}
