//
// Created by boris on 17/10/2020.
//

#include "Catridge.h"

Catridge::Catridge(const std::string &filename) {
    std::ifstream nesFile(filename, std::ifstream::binary);
    if (!nesFile.is_open()) {
        std::cerr << "Error: Nes file not exist" << std::endl;
        exit(-1);
    }
    char tmpByte;
    uint32_t ines_constant = 0;
    uint16_t prg_rom_size = 0;
    uint16_t chr_rom_size = 0;
    nesFile.read((char *) &ines_constant, 4);
    nesFile.read((char *) &prg_rom_size, 1);
    nesFile.read((char *) &chr_rom_size, 1);
    prg_rom_size = 16384 * prg_rom_size;
    chr_rom_size = 8192 * chr_rom_size;

    nesFile.seekg(16);
    for (int i = 0x8000 - prg_rom_size; i < 0x8000; i++) {
        nesFile.read(&tmpByte, 1);
        PRG_ROM[i] = tmpByte;
    }


    for (int i =8192 - chr_rom_size; i < 8192 ; i++) {
        nesFile.read(&tmpByte, 1);
        CHR_ROM[i] = tmpByte;
    }


    nesFile.close();

}

