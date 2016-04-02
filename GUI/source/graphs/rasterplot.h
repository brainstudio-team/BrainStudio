#ifndef RASTERPLOT_H
#define RASTERPLOT_H

#include <QWidget>
#include <QtGui>
#include "basicgraph.h"



class RasterPlot : public BasicGraph {
    Q_OBJECT

protected:
    int neurons;
    int t; // How many steps we have so far
    unsigned max_time; // Saved in timesteps (e.g. ms). Default=10000: eg: 10sec

    bool orientation;
    QColor spColor;
    int margin_left, margin_down;

    char **values; // [neurons][max_time]
    char *spikes; // [neurons] Store all new spikes until we call next_step()

    QVector<float> frequencies;

public:
    explicit RasterPlot(QString _name, int neurons,
                          int _x = 0, int _y = 0, int w = 300, int h = 150,
                          QColor spike_color = Qt::blue, QWidget *parent = 0);
    explicit RasterPlot(QString _name, int neurons,
                        QColor spike_color = Qt::blue, QWidget *parent = 0);
    ~RasterPlot();
    void commonConstructor();
    void setAxes();
    void setOrientation(const bool &value){ orientation = value;
                                            this->setAxes(); this->update(); }
    // Drawing
    void draw(QPainter *painter);
    void drawAxes(QPainter *painter);
    // Printing
    void print(QPainter *painter);
    void printAxes(QPainter *painter, QString title);
    // Rest
    void next_step();
    void next_step(float freq);
    void add_spike(int neuronId);

    int getTime(){ return t; }
};

#endif // RASTERPLOT_H




















