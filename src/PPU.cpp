//
// Created by boris on 17/10/2020.
//

#include "PPU.h"

PPU::PPU(Catridge &catridge) : catridge(catridge) {
    ppuColor[0x00] = 0x757575;
    ppuColor[0x01] = 0x271B8F;
    ppuColor[0x02] = 0x0000AB;
    ppuColor[0x03] = 0x47009F;
    ppuColor[0x04] = 0x8F0077;
    ppuColor[0x05] = 0xAB0013;
    ppuColor[0x06] = 0xA70000;
    ppuColor[0x07] = 0x7F0B00;
    ppuColor[0x08] = 0x432F00;
    ppuColor[0x09] = 0x004700;
    ppuColor[0x0a] = 0x005100;
    ppuColor[0x0b] = 0x003F17;
    ppuColor[0x0c] = 0x1B3F5F;
    ppuColor[0x0d] = 0x000000;
    ppuColor[0x0e] = 0x000000;
    ppuColor[0x0f] = 0x000000;

    ppuColor[0x10] = 0xBCBCBC;
    ppuColor[0x11] = 0x0073EF;
    ppuColor[0x12] = 0x233BEF;
    ppuColor[0x13] = 0x8300F3;
    ppuColor[0x14] = 0xBF00BF;
    ppuColor[0x15] = 0xE7005B;
    ppuColor[0x16] = 0xDB2B00;
    ppuColor[0x17] = 0xCB4F0F;
    ppuColor[0x18] = 0x8B7300;
    ppuColor[0x19] = 0x009700;
    ppuColor[0x1a] = 0x00AB00;
    ppuColor[0x1b] = 0x00933B;
    ppuColor[0x1c] = 0x00838B;
    ppuColor[0x1d] = 0x000000;
    ppuColor[0x1e] = 0x000000;
    ppuColor[0x1f] = 0x000000;

    ppuColor[0x20] = 0xFFFFFF;
    ppuColor[0x21] = 0x3FBFFF;
    ppuColor[0x22] = 0x5F97FF;
    ppuColor[0x23] = 0xA78BFD;
    ppuColor[0x24] = 0xF77BFF;
    ppuColor[0x25] = 0xFF77B7;
    ppuColor[0x26] = 0xFF7763;
    ppuColor[0x27] = 0xFF9B3B;
    ppuColor[0x28] = 0xF3BF3F;
    ppuColor[0x29] = 0x83D313;
    ppuColor[0x2a] = 0x4FDF4B;
    ppuColor[0x2b] = 0x58F898;
    ppuColor[0x2c] = 0x00EBDB;
    ppuColor[0x2d] = 0x000000;
    ppuColor[0x2e] = 0x000000;
    ppuColor[0x2f] = 0x000000;

    ppuColor[0x30] = 0xFFFFFF;
    ppuColor[0x31] = 0xABE7FF;
    ppuColor[0x32] = 0xC7D7FF;
    ppuColor[0x33] = 0xD7CBFF;
    ppuColor[0x34] = 0xFFC7FF;
    ppuColor[0x35] = 0xFFC7DB;
    ppuColor[0x36] = 0xFFBFB3;
    ppuColor[0x37] = 0xFFDBAB;
    ppuColor[0x38] = 0xFFE7A3;
    ppuColor[0x39] = 0xE3FFA3;
    ppuColor[0x3a] = 0xABF3BF;
    ppuColor[0x3b] = 0xB3FFCF;
    ppuColor[0x3c] = 0x9FFFF3;
    ppuColor[0x3d] = 0x000000;
    ppuColor[0x3e] = 0x000000;
    ppuColor[0x3f] = 0x000000;
}

int PPU::getScreenPixel(int x, int y) {
    return screen[x][y];
}

void PPU::reset() {
    address_latch = 0;
    cycle = 0;
    scanline = 261;
    nmi = false;
    status.reg = 0;
    mask.reg = 0;
    control.reg = 0;
    v_vram.reg = 0;
    t_vram.reg = 0;
    fine_x = 0;
    bg_shifter_pattern_lo = 0;
    bg_shifter_patter_hi = 0;
    bg_shifter_at_lo = 0;
    bg_shifter_at_hi = 0;
    bg_next_tile_lsb = 0;
    bg_next_tile_msb = 0;
    bg_next_at_tile = 0;
    oam_addr = 0;
};

void PPU::loadBackgroundShifters() {
    bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
    bg_shifter_patter_hi = (bg_shifter_patter_hi & 0xFF00) | bg_next_tile_msb;
    bg_shifter_at_lo = (bg_shifter_at_lo & 0xFF00) | ((bg_next_at_tile & 0x1) ? 0xFF : 0x00);
    bg_shifter_at_hi = (bg_shifter_at_hi & 0xFF00) | ((bg_next_at_tile & 0x2) ? 0xFF : 0x00);
}

void PPU::updateShifters() {

    if (mask.render_background) {
        bg_shifter_pattern_lo <<= 1;
        bg_shifter_patter_hi <<= 1;
        bg_shifter_at_lo <<= 1;
        bg_shifter_at_hi <<= 1;
    }
    if (mask.render_sprites && cycle >= 1 && cycle <= 256) {
        for (int i = 0; i < sprite_count; i++) {
            if (sOAM[i * 4 + 3] > 0) {
                sOAM[i * 4 + 3]--;
            } else {
                sp_shifter_pattern_lo[i] <<= 1;
                sp_shifter_pattern_hi[i] <<= 1;
            }
        }
    }
}

void PPU::render() {

    uint8_t bg_color = 0;
    uint8_t fg_color = 0;
    uint8_t fg_priority = 0;

    uint8_t bg_zero = 0;
    uint8_t fg_zero = 0;
    if (mask.render_background) {
        uint16_t bit_mux = 0x8000 >> fine_x;
        uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
        uint8_t p1_pixel = (bg_shifter_patter_hi & bit_mux) > 0;
        uint8_t p2_pixel = (bg_shifter_at_lo & bit_mux) > 0;
        uint8_t p3_pixel = (bg_shifter_at_hi & bit_mux) > 0;
        bg_zero = p1_pixel | p0_pixel;
        bg_color = read(0x3F00 + ((p3_pixel << 3) | (p2_pixel << 2) | (p1_pixel << 1) | (p0_pixel << 0)));
    }

    if (mask.render_sprites) {
        spriteZeroHitBegin = false;
        for (int i = 0; i < sprite_count; i++) {
            if (sOAM[i * 4 + 3] == 0) {
                uint8_t p0_pixel = (sp_shifter_pattern_lo[i] & 0x80) > 0;
                uint8_t p1_pixel = (sp_shifter_pattern_hi[i] & 0x80) > 0;
                uint8_t fg_palette = (sOAM[i * 4 + 2] & 0x03);
                fg_priority = (sOAM[i * 4 + 2] & 0x20) == 0;

                fg_zero = p1_pixel | p0_pixel;
                fg_color = read(0x3F10 + ((fg_palette << 2) | (p1_pixel << 1) | (p0_pixel << 0)));
                if (fg_zero != 0) {
                    if (i == 0) {
                        spriteZeroHitBegin = true;
                    }
                    break;
                }
            }
        }
    }


    if (bg_zero == 0 && fg_zero == 0) {
        color = bg_color;
    } else if (bg_zero == 0 && fg_zero != 0) {
        color = fg_color;
    } else if (bg_zero != 0 && fg_zero == 0) {
        color = bg_color;
    } else {
        if (fg_priority) {
            color = fg_color;
        } else {
            color = bg_color;
        }
        if (spriteZeroHitPosible & spriteZeroHitBegin) {
            if (mask.render_background & mask.render_background) {
                if ((1 <= cycle && cycle <= 256)) {
                    status.sprite_zero_hit = 1;
                }
            }
        }
    }

}

void PPU::incrementalScrollX() {
    if (mask.render_background || mask.render_sprites) {
        if (v_vram.coarse_x == 0x1F) {
            v_vram.coarse_x = 0;
            v_vram.nametable_x = ~v_vram.nametable_x;
        } else {
            v_vram.coarse_x++;
        }
    }
}

void PPU::incrementalScrollY() {
    if (mask.render_background || mask.render_sprites) {
        if (v_vram.fine_y != 0x7) {
            v_vram.fine_y++;
        } else {
            v_vram.fine_y = 0;
            if (v_vram.coarse_y == 29) {
                v_vram.coarse_y = 0;
                v_vram.nametable_y ^= 1;
            } else if (v_vram.coarse_y == 31) {
                v_vram.coarse_y = 0;
            } else {
                v_vram.coarse_y++;
            }
        }
    }
}

void PPU::transferAddressX() {
    if (mask.render_background || mask.render_sprites) {
        v_vram.coarse_x = t_vram.coarse_x;
        v_vram.nametable_x = t_vram.nametable_x;
    }
}

void PPU::transferAddressY() {
    if (mask.render_background || mask.render_sprites) {
        v_vram.coarse_y = t_vram.coarse_y;
        v_vram.nametable_y = t_vram.nametable_y;
        v_vram.fine_y = t_vram.fine_y;
    }
}

void PPU::memlog(uint16_t addr) {
    printf("$%04X:", addr & 0xFFF0);
    for (uint16_t i = 0; i < 16; i++) {
        printf(" %02X", read((addr & 0xFFF0) + i));
    }
    printf("\n");
}

void PPU::poammemlog() {
    for (uint16_t page = 0; page <= 0x00F0; page += 0x0010) {
        printf("$%04X:", page);
        for (uint16_t i = 0; i < 16; i++) {
            printf(" %02X", pOAM[page + i]);
        }
        printf("\n");
    }
}

void PPU::soammemlog() {
    for (uint16_t page = 0; page <= 0x0010; page += 0x0010) {
        printf("$%04X:", page);
        for (uint16_t i = 0; i < 16; i++) {
            printf(" %02X", sOAM[page + i]);
        }
        printf("\n");
    }
}

uint8_t bitswap(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

void PPU::clock() {

    uint16_t vram_addr = v_vram.reg & 0x0FFF;

    if ((0 <= scanline && scanline <= 239) || scanline == 261) {
        if ((2 <= cycle && cycle <= 257) || (321 <= cycle && cycle <= 337)) {
            updateShifters();

            switch ((cycle - 1) % 8) {
                case 0:
                    loadBackgroundShifters();
                    bg_next_tile_id = read(VRAM_OFFSET | vram_addr);
                    break;
                case 2:
                    bg_next_at = read(
                            VRAMA_OFFSET | (vram_addr & 0x0C00) | ((vram_addr >> 4) & 0x38) |
                            ((vram_addr >> 2) & 0x7));

                    bg_next_at_tile =
                            (bg_next_at >> (2 * (((vram_addr >> 5) & 0x2) | ((vram_addr >> 1) & 0x1)))) & 0x3;

                    break;
                case 4:
                    bg_next_tile_lsb = read(
                            CHR_ROM_OFFSET[control.pattern_background] + (bg_next_tile_id << 4) + v_vram.fine_y +
                            0);
                    break;
                case 6:
                    bg_next_tile_msb = read(
                            CHR_ROM_OFFSET[control.pattern_background] + (bg_next_tile_id << 4) + v_vram.fine_y +
                            8);
                    break;
                case 7:
                    incrementalScrollX();
                    break;
                default:
                    break;
            }
        }
        if (cycle == 256) {
            incrementalScrollY();
        }
        if (cycle == 257) {
            transferAddressX();
        }
    }

    if ((0 <= scanline && scanline <= 239)) {
        if (cycle == 258) {
            spriteZeroHitPosible = false;
            sprite_count = 0;
            for (int s = 0; s < 32; s++) {
                sOAM[s] = 0xFF;
            }

            int s = 0;
            for (int n = 0; n < 64; n++) {
                if (pOAM[4 * n] <= scanline && scanline <= (pOAM[4 * n] + 7)) {

                    if (sprite_count < 8) {
                        if (n == 0) {
                            spriteZeroHitPosible = true;
                        }
                        for (int m = 0; m < 4; m++) {
                            sOAM[s++] = pOAM[4 * n + m];
                        }
                    }
                    sprite_count++;
                }
            }
            status.sprite_overflow = (sprite_count > 8);
            sprite_count = (sprite_count > 8) ? 8 : sprite_count;

        }
        if (cycle == 340) {
            for (uint8_t i = 0; i < sprite_count; i++) {
                uint8_t sp_pattern_lo, sp_pattern_hi;
                uint16_t sp_pattern_addr_lo, sp_pattern_addr_hi;
                if ((sOAM[i * 4 + 2] & 0x80)) {
                    sp_pattern_addr_lo = (uint16_t(control.pattern_sprite) << 12)
                                         | (uint16_t(sOAM[i * 4 + 1]) << 4)
                                         | (7 - ((scanline - sOAM[i * 4 + 0]) & 0x07));
                } else {
                    sp_pattern_addr_lo = (uint16_t(control.pattern_sprite) << 12)
                                         | (uint16_t(sOAM[i * 4 + 1]) << 4)
                                         | ((scanline - sOAM[i * 4 + 0]) & 0x07);
                }
                sp_pattern_addr_hi = sp_pattern_addr_lo + 8;
                sp_pattern_lo = read(sp_pattern_addr_lo);
                sp_pattern_hi = read(sp_pattern_addr_hi);

                if ((sOAM[i * 4 + 2] & 0x40)) {
                    sp_pattern_lo = bitswap(sp_pattern_lo);
                    sp_pattern_hi = bitswap(sp_pattern_hi);
                }
                sp_shifter_pattern_lo[i] = sp_pattern_lo;
                sp_shifter_pattern_hi[i] = sp_pattern_hi;
            }
        }
    }
    if (0 <= scanline && scanline <= 239) {
        if (1 <= cycle && cycle <= 256) {
            render();
            screen[cycle - 1][scanline] = ppuColor[color];
        }
    }

    if (cycle == 1 && scanline == 241) {
        status.vertical_blank = 1;
        frame++;

        if (control.enable_nmi)
            nmi = true;

    }

    if (cycle == 1 && scanline == 261) {
        status.vertical_blank = 0;
        status.sprite_overflow = 0;
        status.sprite_zero_hit = 0;

        for (int i = 0; i < 8; i++) {
            sp_shifter_pattern_hi[i] = 0;
            sp_shifter_pattern_lo[i] = 0;
        }
    }

    if (scanline == 261) {
        if (280 <= cycle && cycle <= 304) {
            transferAddressY();
        }
    }


    cycle++;
    if (cycle > 340) {
        cycle = 0;
        scanline++;
        if (scanline > 261) {
            scanline = 0;
        }
    }

}

uint8_t PPU::read(uint16_t addr) {
    if (addr >= 0x0000 && addr <= 0x1FFF)
        return catridge.CHR_ROM[addr & 0x1FFF];
    else if (addr >= 0x2000 && addr <= 0x3EFF)
        return VRAM[addr & 0x0FFF];
    else if (addr >= 0x3F00 && addr <= 0x3F1F) {
        addr &= 0x001F;
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;

        if (addr == 0x0004 || addr == 0x0008 || addr == 0x000C) {
            addr = 0x0000;
        }
        return palette[addr];
    } else {
        std::cerr << "Addressig Error - 0x" << std::hex << addr << std::endl;
        exit(-1);
    }
}

void PPU::write(uint16_t addr, uint8_t src) {
    if (addr >= 0x0000 && addr <= 0x1FFF)
        catridge.CHR_ROM[addr & 0x1FFF] = src;
    else if (addr >= 0x2000 && addr <= 0x3EFF)
        VRAM[addr & 0x0FFF] = src;
    else if (addr >= 0x3F00 && addr <= 0x3F1F) {
        addr &= 0x001F;
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;
        palette[addr] = src;
    } else {
//        std::cerr << "Addressig Error - 0x" << std::hex << addr << std::endl;
//        exit(-1);
    }
}

uint8_t PPU::cpuRead(uint16_t addr) {
    uint8_t data = 0;
    switch (addr) {
        case 0x0000:
            data = control.reg;
            break;
        case 0x0001:
            data = mask.reg;
            break;
        case 0x0002:
            data = status.reg & 0xE0;
            status.vertical_blank = 0;
            address_latch = 0;
            break;
        case 0x0003:
            break;
        case 0x0004:
            data = pOAM[oam_addr];
            oam_addr++;
            break;
        case 0x0005:
            break;
        case 0x0006:
            break;
        case 0x0007:
            data = ppuDataBuffer;
            ppuDataBuffer = read(v_vram.reg & 0x3FFF);
            if (v_vram.reg >= 0x3f00) data = ppuDataBuffer;
            v_vram.reg += (control.increment_mode ? 32 : 1);
            break;

    }
    return data;
}

void PPU::cpuWrite(uint16_t addr, uint8_t src) {
    switch (addr) {
        case 0x0000:
            control.reg = src;
            t_vram.nametable_x = control.nametable_x;
            t_vram.nametable_y = control.nametable_y;
            break;
        case 0x0001:
            mask.reg = src;
            break;
        case 0x0002:
            status.reg = src;
            break;
        case 0x0003:
            oam_addr = src;
            break;
        case 0x0004:
            pOAM[oam_addr] = src;
            oam_addr++;
            break;
        case 0x0005:
            if (address_latch == 0) {
                t_vram.coarse_x = (src >> 3) & 0x1F;
                fine_x = src & 0x7;
                address_latch = 1;
            } else {
                t_vram.coarse_y = (src >> 3) & 0x1F;
                t_vram.fine_y = src & 0x7;
                address_latch = 0;
            }
            break;
        case 0x0006:
            if (address_latch == 0) {
                t_vram.reg = (uint16_t(src & 0x3F) << 8) | (t_vram.reg & 0x00FF);
                address_latch = 1;
            } else {
                t_vram.reg = (t_vram.reg & 0xFF00) | src;
                v_vram.reg = t_vram.reg;
                address_latch = 0;
            }

            break;
        case 0x0007:
            write(v_vram.reg & 0x3FFF, src);
            v_vram.reg += (control.increment_mode ? 32 : 1);
            break;
    }
}




