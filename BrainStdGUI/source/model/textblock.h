#ifndef ITEM_H
#define ITEM_H

#include <QtGui/QColor>
#include <QtWidgets>
#include <QPoint>
#include <QString>
#include <QVector>

class Item/* : public QWidget*/{
//    Q_OBJECT
protected:
    QString _name;
    QString _colour;
    int initial_x, initial_y;
    int WINDOW_X, WINDOW_Y;
    int xx,yy,ww,hh;
public:
    Item(QString name = "item", int _x = 100, int _y = 100, int _w = 100,
         int _h = 100, QString colour = "999999", QWidget *parent = 0);

    QString name() const { return _name; }
    QString colour() const { return _colour; }

    void setWINDOW_X(const int &value){ WINDOW_X = value; this->moveAll(); }
    void setWINDOW_Y(const int &value){ WINDOW_Y = value; this->moveAll(); }

    void moveAll(){ this->move(initial_x + WINDOW_X, initial_y + WINDOW_Y); }

    void move(int xxx, int yyy){ xx=xxx;yy=yyy;}
    void resize(int www, int hhh){ ww=www;hh=hhh;}
    int x(){ return xx;}
    int y(){ return yy;}
    int width() {return ww; }
    int height() {return hh; }
};


class TextBlock : public Item{
//    Q_OBJECT
private:
    int _fontSize;

public:
    TextBlock(QString text = "text", int fontSize=12, int _x =200, int _y = 200,
              int _w = 200, int _h = 100, QString colour = "000000",
              QWidget *parent=0):Item(text, _x, _y, _w, _h, colour, parent),
              _fontSize(fontSize){}

    int fontSize(){ return _fontSize; }
    QString text(){ return _name; }

    void setFontSize(int value){ _fontSize = value; }
    void setText(QString text){ _name = text; }
};
#endif // ITEM_H
