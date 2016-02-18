#ifndef PLOT2D_H
#define PLOT2D_H

#include <QVector>
#include <QtGui>
#include "basicgraph.h"

#define MAX_LINES 256

class Plot2D : public BasicGraph {
    Q_OBJECT
private:
    int line;
    float ratio1;
    float ratio2;

    bool rescaling;

protected:
    unsigned t;       // The current time in our graph
    unsigned pointer; // The current time modulo num_points

    QVector< QVector<float> > value;
    QVector<float> history;
    QVector< QColor > colour;

    bool keepHistory; // If the history of the first curve should be stored.
    int lines;        // The number of different curves of the graph
    int num_points;   // The number of points that this graph shows and stores.
    float min;        // The minimum value that this graph can show
    float max;        // The maximum value that this graph can show
    float rangeF;     // The range of the graph

public:
    explicit Plot2D(QString _name, int _lines, float _min, float _max,
                          int _x = 0, int _y = 0, int w = 300, int h = 150,
                          QColor color1=Qt::red, QColor color2=Qt::blue,
                          int _num_points = 500, QWidget *parent = 0);
    explicit Plot2D(QString _name, int _lines, float _min, float _max,
                          QColor color1=Qt::red, QColor color2=Qt::blue,
                          int _num_points = 500, QWidget *parent = 0);
    void commonConstruction(int _lines, float _min, float _max, QColor color1,
                            QColor color2, int _num_points);
    void setAxes();
    void draw(QPainter *painter);
    void print(QPainter *painter);
    //void drawAxes(QPainter *painter);
    //void printAxes(QPainter *painter, QString title);
    void add_new_value(int line, float data_value);
    void historyOff() { keepHistory = false; }
    void historyOn()  { keepHistory = true; }
    void setMaxValue(const float &value);
    void setMinValue(const float &value);
    void setNumberOfPoints(const int &_value);
    void setRescaling(const bool &on) { rescaling = on; }

    int getNumberOfPoints() const { return num_points; }
    int getLines() const { return lines; }
    bool getRescaling() const { return rescaling; }
};
#endif // PLOT2D_H



