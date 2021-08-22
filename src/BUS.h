//
// Created by boris on 17/10/2020.
//

#ifndef NES_EMU5_BUS_H
#define NES_EMU5_BUS_H

#include <cstdint>
#include <iostream>
#include <fstream>

#include "Catridge.h"
#include "PPU.h"

class BUS {
public:
    uint8_t controller[2];
    BUS(Catridge &catridge, PPU &ppu);
    uint8_t cpuRead(uint16_t addr);
    void cpuWrite(uint16_t addr, uint8_t src);
    void memlog(uint16_t addr);
    void dma_clock();
    bool dma_transfer = false;
private:
    uint8_t cpuRam[2*1024];
    uint8_t audio[32];
    uint8_t controller_state[2];
    uint8_t dma_page = 0;
    uint8_t dma_addr = 0;

    uint8_t debug_memory[0x10000];

    Catridge &catridge;
    PPU &ppu;
};


#endif //NES_EMU5_BUS_H
