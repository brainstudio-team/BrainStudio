#ifndef PHASEPORTRAITGRAPH_H
#define PHASEPORTRAITGRAPH_H

#include <iostream>
#include <deque>
#include <QVector>
#include <QWidget>
#include <QtGui>
#include "basicgraph.h"

#define MAX_LINES 256

class PhasePortraitGraph : public BasicGraph {
    Q_OBJECT
private:
    int line;
    double ratio1;
    double ratio2;

protected:
    unsigned t;       // The current time in our graph
    unsigned pointer; // The current time modulo num_points

    std::deque< std::deque<double> > valueX;
    std::deque< std::deque<double> > valueY;
    std::deque<double> historyX;
    std::deque<double> historyY;
    QVector< QColor > colour;

    bool keepHistory; // If the history of the first curve should be stored.
    int lines;        // The number of different curves of the graph
    int num_points;   // The number of points that this graph shows and stores.
    double minX;      // The minimum value of X that this graph can show
    double maxX;      // The maximum value of X that this graph can show
    double minY;      // The minimum value of Y that this graph can show
    double maxY;      // The maximum value of Y that this graph can show
    float rangeF;     // The range of the graph
    double rangeX;    //
    double rangeY;    //

public:
    explicit PhasePortraitGraph(int _lines, double _minX, double _maxX,
                                double _minY, double _maxY, int _x = 0,
                                int _y = 0, int w = 300, int h = 150,
                                QColor color1=Qt::red, QColor color2=Qt::blue,
                                int _num_points = 500, QWidget *parent = 0);
    void setAxes();
    void draw(QPainter *painter);
    void print(QPainter *painter);
    //void drawAxes(QPainter *painter);
    //void printAxes(QPainter *painter, QString title);
    void add_new_value(const int &_line, double value_x,
                       double value_y);

    void add_new_value(int line, float data_value){
                      std::cout<<"Error: phase portraits need 2D values! (line:"
                               <<line<<", value:"<<data_value<<")\n";}
    void historyOff() { keepHistory = false; }
    void historyOn()  { keepHistory = true; }

};

#endif // PHASEPORTRAITGRAPH_H








