//
// Created by boris on 17/10/2020.
//

#include "BUS.h"


BUS::BUS(Catridge &catridge, PPU &ppu) : catridge(catridge), ppu(ppu) {
    for (int i = 0; i < 2 * 1024; i++)
        cpuRam[i] = 0;
}

uint8_t BUS::cpuRead(uint16_t addr) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        return cpuRam[addr & 0x07FF];
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        return ppu.cpuRead(addr & 0x0007);
    } else if (addr >= 0x4016 && addr <= 0x4017) {
        uint8_t data = (controller_state[addr & 0x0001] & 0x80) > 0;
        controller_state[addr & 0x0001] <<= 1;
        return data;
    } else if (addr >= 0x8000 && addr <= 0xFFFF) {
        return catridge.PRG_ROM[addr & 0x7FFF];
    } else {
        std::cerr << "cpuRead Addressig Error - 0x" << std::hex << addr << std::endl;
        return debug_memory[addr];
    }
}

void BUS::cpuWrite(uint16_t addr, uint8_t src) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        cpuRam[addr & 0x07FF] = src;
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        ppu.cpuWrite(addr & 0x0007, src);
    } else if (addr >= 0x4000 && addr <= 0x4013) {
        audio[addr & 0x001F] = src;
    } else if (addr == 0x4014) {
        dma_page = src;
        dma_addr = 0;
        dma_transfer = true;
    } else if (addr == 0x4015) {
        uint8_t a = 0;
    } else if (addr >= 0x4016 && addr <= 0x4017) {
        controller_state[addr & 0x0001] = controller[addr & 0x0001];
    } else if (addr >= 0x8000 && addr <= 0xFFFF) {
        catridge.PRG_ROM[addr & 0x7FFF] = src;
    } else {
        debug_memory[addr] = src;
        std::cerr << "cpuWrite Addressig Error - 0x" << std::hex << addr << std::endl;
    }
}

void BUS::memlog(uint16_t addr) {
    printf("$%04X:", addr & 0xFFF0);
    for (uint16_t i = 0; i < 16; i++) {
        printf(" %02X", cpuRead((addr & 0xFFF0) + i));
    }
    printf("\n");
}

void BUS::dma_clock() {
    if (dma_transfer) {
        ppu.pOAM[dma_addr] = cpuRead((uint16_t(dma_page) << 8) | dma_addr);
        dma_addr++;
        if (dma_addr == 0) {
            dma_transfer = false;
        }
    }
}

