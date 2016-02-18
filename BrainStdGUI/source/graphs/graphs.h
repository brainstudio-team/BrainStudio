#ifndef GRAPHS_H
#define GRAPHS_H

#include "currentgraph.h"
#include "rasterplot.h"
#include "phaseportraitgraph.h"
#include "plot2D.h"

class Graphs
{
private:
    int i,j,startX,startY;
protected:
    int x,y;
    int hGraphsNo;
    int cGraphsNo;

    CurrentGraph *hGraph[32];
    CurrentGraph *cGraph[32];
    RasterPlot *sGraph[5];

    bool mode6;
    RasterPlot *corticalGraph[32];
    int corticalGraphsNo;
    int corticalNeurons[64];

    int neurons;

public:
    Graphs(int historyNo, int currentNo, int neuronsNo, bool _mode6,
            int maxSpikes, int position_x, int position_y);
    ~Graphs();

    void draw(QPainter *painter);
    void add_new_value(int index, int data_num, float data_value);
    void add_new_value_current(int index, int data_num, float data_value);
    void add_spike(int neuronId);
    void next_step();

    void add_cortical_graph(int neuronsNo);
    void add_input_graph(int neuronsNo);
    void add_output_graph(int neuronsNo, int inputNeurons);
    void add_module_graph(int neuronsNo, QString title);
};

#endif // GRAPHS_H
