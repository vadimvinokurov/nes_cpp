#include "screen.h"

Screen::Screen(PPU &ppu, BUS &bus, QWidget *parent)
        : QWidget(parent), ppu(ppu), bus(bus) {
    this->resize(w, h);
}

void Screen::paintEvent(QPaintEvent *event) {
    QImage img(w, h, QImage::Format_RGB32);

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            img.setPixel(x, y, ppu.getScreenPixel(x / upscale, y / upscale));
        }
    }

    QPainter pt(this);
    pt.drawPixmap(0, 0, w, h, QPixmap::fromImage(img));
}

void Screen::keyPressEvent(QKeyEvent *event) {

    bus.controller[0] |= (event->key() == Qt::Key_A) ? 0x80 : 0x00;     // A Button
    bus.controller[0] |= (event->key() == Qt::Key_S) ? 0x40 : 0x00;     // B Button
    bus.controller[0] |= (event->key() == Qt::Key_Q) ? 0x20 : 0x00;     // Select
    bus.controller[0] |= (event->key() == Qt::Key_W) ? 0x10 : 0x00;     // Start
    bus.controller[0] |= (event->key() == Qt::Key_Up) ? 0x08 : 0x00;
    bus.controller[0] |= (event->key() == Qt::Key_Down) ? 0x04 : 0x00;
    bus.controller[0] |= (event->key() == Qt::Key_Left) ? 0x02 : 0x00;
    bus.controller[0] |= (event->key() == Qt::Key_Right) ? 0x01 : 0x00;

}
void Screen::keyReleaseEvent(QKeyEvent *event) {
    bus.controller[0] = 0;
}

Screen::~Screen() {
}


