#ifndef PHASEGRAPH_H
#define PHASEGRAPH_H

#include "basicgraph.h"

class PhaseGraph : public BasicGraph {
    Q_OBJECT
protected:
    int phases;       // The number of different phases in the graph
    int num_points;   // The number of points that this graph shows and stores.
    unsigned t;       // The current time in our graph
    unsigned pointer; // The current time modulo num_points

    QVector< QVector<float> > values;
    QVector< float > fc; // Synchrony of the group

    void setAxes();

public:
    explicit PhaseGraph(QString _name, int _phases, int _num_points = 10,
                        int _x = 0, int _y = 0, int w = 300, int h = 150,
                        QWidget *parent = 0);
    explicit PhaseGraph(QString _name, int _phases, int _num_points = 10,
                        QWidget *parent = 0);

    void draw(QPainter *painter);
    void print(QPainter *painter){}
    void drawAxes(QPainter *painter){}
    void printAxes(QPainter *painter, QString title){}
    void add_new_value(int line, float data_value);
};

#endif // PHASEGRAPH_H
