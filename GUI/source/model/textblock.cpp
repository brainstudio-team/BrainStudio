#include "textblock.h"

Item::Item(QString name, int _x, int _y, int _w, int _h, QString colour,
           QWidget *parent):_name(name), initial_x(_x), initial_y(_y),
              WINDOW_X(0), WINDOW_Y(0), _colour(colour)/*, QWidget(parent)*/{
    this->move(_x, _y);
    this->resize(_w, _h);
}

