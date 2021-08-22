//
// Created by boris on 17/10/2020.
//

#ifndef NES_EMU5_PPU_H
#define NES_EMU5_PPU_H

#include <vector>
#include "Catridge.h"

const uint16_t CHR_ROM_OFFSET[] = {0x0000, 0x1000};
const uint16_t VRAM_OFFSET = 0x2000;
const uint16_t VRAMA_OFFSET = 0x23C0;

class PPU {
public:
    explicit PPU(Catridge &catridge);
    int getScreenPixel(int x, int y);
    void clock();
    void reset();

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t src);

    uint8_t cpuRead(uint16_t addr);
    void cpuWrite(uint16_t addr, uint8_t src);

    bool nmi = false;

    uint8_t pOAM[256];
    uint8_t sOAM[32];
    union
    {
        struct
        {
            uint8_t unused : 5;
            uint8_t sprite_overflow : 1;
            uint8_t sprite_zero_hit : 1;
            uint8_t vertical_blank : 1;
        };

        uint8_t reg;
    } status;
    void memlog(uint16_t addr);
    void poammemlog();
    void soammemlog();

private:
    void loadBackgroundShifters();
    void updateShifters();

    void incrementalScrollX();
    void incrementalScrollY();
    void transferAddressX();
    void transferAddressY();
    void render();


    int cycle = 0;
    int scanline = 0;
    uint64_t frame = 0;

    uint8_t bg_next_tile_id = 0;
    uint8_t bg_next_at = 0;
    uint8_t bg_next_at_tile = 0;
    uint8_t bg_next_tile_lsb = 0;
    uint8_t bg_next_tile_msb = 0;

    uint16_t bg_shifter_pattern_lo = 0;
    uint16_t bg_shifter_patter_hi = 0;
    uint16_t bg_shifter_at_lo = 0;
    uint16_t bg_shifter_at_hi = 0;

    uint8_t sprite_count = 0;
    uint8_t sp_shifter_pattern_lo[8];
    uint8_t sp_shifter_pattern_hi[8];

    bool spriteZeroHitPosible = false;
    bool spriteZeroHitBegin = false;
    uint8_t color = 0;

    Catridge &catridge;

    uint8_t VRAM[0x1000];
    uint8_t palette[32];

    int ppuColor[0x100];
    int screen[341][261];

    union vram_t {
        struct {
            uint16_t coarse_x: 5;
            uint16_t coarse_y: 5;
            uint16_t nametable_x: 1;
            uint16_t nametable_y: 1;
            uint16_t fine_y: 3;
            uint16_t unused: 1;
        };
        uint16_t reg;
    };
    vram_t v_vram;
    vram_t t_vram;
    uint8_t fine_x;

    uint8_t oam_addr = 0;



    union
    {
        struct
        {
            uint8_t grayscale : 1;
            uint8_t render_background_left : 1;
            uint8_t render_sprites_left : 1;
            uint8_t render_background : 1;
            uint8_t render_sprites : 1;
            uint8_t enhance_red : 1;
            uint8_t enhance_green : 1;
            uint8_t enhance_blue : 1;
        };

        uint8_t reg;
    } mask;

    union
    {
        struct
        {
            uint8_t nametable_x : 1;
            uint8_t nametable_y : 1;
            uint8_t increment_mode : 1;
            uint8_t pattern_sprite : 1;
            uint8_t pattern_background : 1;
            uint8_t sprite_size : 1;
            uint8_t slave_mode : 1; // unused
            uint8_t enable_nmi : 1;
        };

        uint8_t reg;
    } control;

    uint8_t address_latch = 0;
    uint8_t ppuDataBuffer = 0;


};


#endif //NES_EMU5_PPU_H
