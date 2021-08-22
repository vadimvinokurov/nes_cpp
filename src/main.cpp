#include <QApplication>
#include <QWidget>
#include <QTimer>
#include <iostream>
#include <thread>
#include <chrono>

#include "CPU.h"
#include "PPU.h"
#include "BUS.h"
#include "screen.h"
#include <unistd.h>
uint64_t usec_time = 800;

void nes_sleep(uint64_t del){
   for(uint64_t i = 0; i<del; i++) {}
}

[[noreturn]] void NesExect(CPU *cpu, PPU *ppu, BUS *bus){
    cpu->reset();
    ppu->reset();

    while(1) {
        if(bus->dma_transfer) {
            bus->dma_clock();
        } else {
            cpu->clock();
        }


        ppu->clock();
        ppu->clock();
        ppu->clock();

        if(ppu->nmi){
            ppu->nmi = false;
            cpu->NMI();
        }
        nes_sleep(usec_time);
    }
};

int main(int argc, char** argv) {

    QApplication a(argc, argv);

    //atridge catridge("../test/Super Tank (Battle City Pirate) (J) [p1].nes");
    //Catridge catridge("../test/lode-runner-(u).nes");
    Catridge catridge("../test/Super Mario Bros. (World).nes");
    //Catridge catridge("../test/nestest.nes");
    PPU ppu(catridge);
    BUS bus(catridge, ppu);
    CPU cpu(bus);

    std::thread nes = std::thread(NesExect, &cpu, &ppu, &bus);
    Screen s(ppu, bus);
    s.show();
    QTimer rotationTimer;
    rotationTimer.setInterval(1000/60);

    QObject::connect(&rotationTimer, SIGNAL(timeout()), &s, SLOT(update()));
    rotationTimer.start();
    return a.exec();
}