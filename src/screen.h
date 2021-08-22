#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QImage>
#include <vector>

#include "PPU.h"
#include "BUS.h"


class Screen : public QWidget
{
    Q_OBJECT

public:
    Screen(PPU &ppu, BUS &bus, QWidget *parent = nullptr);
    ~Screen();

    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private:
    PPU &ppu;
    BUS &bus;
    const int upscale = 2;
    const int w = 256 * upscale;
    const int h = 240 * upscale;



};
#endif // SCREEN_HPP
