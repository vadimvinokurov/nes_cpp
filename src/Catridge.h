//
// Created by boris on 17/10/2020.
//

#ifndef NES_EMU5_CATRIDGE_H
#define NES_EMU5_CATRIDGE_H

#include <fstream>
#include <iostream>


class Catridge {
public:
    Catridge(const std::string &filename);
    int PRG_ROM[32*1024];
    int CHR_ROM[8*1024];
};


#endif //NES_EMU5_CATRIDGE_H
