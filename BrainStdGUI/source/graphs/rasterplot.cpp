#include "rasterplot.h"

#include<QtGui>
#include<iostream>

RasterPlot::RasterPlot(QString _name, int _neurons, int _x, int _y, int w,int h,
            QColor spike_color, QWidget *parent):
            BasicGraph(_name, 0, _x, _y, w, h, parent),neurons(_neurons),
            spColor(spike_color){
    this->commonConstructor();
}

RasterPlot::RasterPlot(QString _name, int _neurons, QColor spike_color,
                       QWidget *parent):BasicGraph(_name, 0, parent),
                       neurons(_neurons), spColor(spike_color){
    this->commonConstructor();
}

RasterPlot::~RasterPlot(){
    delete []spikes;

    for(int i=0; i<neurons; i++){
        delete []values[i];
    }
    delete []values;
}


void RasterPlot::commonConstructor(){
    max_time = 10000;
    orientation = BasicGraph::HORIZONTAL;
    has_axes = true;

    spikes = new char[neurons];

    values = new char*[neurons];

    for(int i=0; i<neurons; i++){
        values[i] = new char[max_time];
        spikes[i] = 0;
    }
    t = 0;
    setAxes();
    setPrintAxes(0,0,0,0,0,0,0,0);
}

void RasterPlot::setAxes(){
    if(orientation == BasicGraph::HORIZONTAL){
        if(has_axes){
            margin_left = 17;
            margin_down = 17;
        }
        else{
            margin_left = 0;
            margin_down = 0;
        }
        num_points = width() - margin_left;
    }
    else{
        if(has_axes){
            margin_left = 17;
            margin_down = 17;
        }
        else{
            margin_left = 0;
            margin_down = 0;
        }
        num_points = height() - margin_down;
    }

    x_axisX0 = 0;
    x_axisY0 = height() - margin_down;
    x_axisX1 = width();
    x_axisY1 = height() - margin_down;

    y_axisX0 = width() - margin_left;
    y_axisY0 = 0;
    y_axisX1 = width() - margin_left;
    y_axisY1 = height();
}

void RasterPlot::add_spike(int neuronId){
    spikes[neuronId] = 1;
}

void RasterPlot::next_step(){
    for(i=0; i<neurons; i++){
        values[i][t] = spikes[i];
    }
    for(i=0; i<neurons;i++)
        spikes[i] = 0;
    t++;
}

void RasterPlot::next_step(float freq){
    for(i=0; i<neurons; i++){
        values[i][t] = spikes[i];
    }
    for(i=0; i<neurons;i++)
        spikes[i] = 0;
    t++;

    // Frequency Plot
    frequencies.append(freq);
}

/* --------------------------------------------------------------------------
                                Drawing
   --------------------------------------------------------------------------
   How it works: We save all the points but we show only the [t-num_points, t]
*/
void RasterPlot::draw(QPainter *painter){
    if(neurons == 0)
        return;

    if(t>=max_time)
        t=0;

    int i, n;
    if(orientation == BasicGraph::HORIZONTAL){
        // Horizontal
        QPixmap pixmap(num_points, neurons);
        pixmap.fill(Qt::transparent);
        //pixmap.fill(Qt::gray); // For debugging. Makes this widget visible!

        QPainter pp(&pixmap);

        // Number on Axis
        pp.setPen(spColor);

        if(t < num_points){
            for(n=0; n<neurons; n++)
                for(i=0; i<t; i++)
                    if(values[n][i])
                        pp.drawLine( i, neurons-n+1, i, neurons-n );
        }
        else{
            for(n=0; n<neurons; n++)
                for(int i=(t-num_points); i<t;i++)
                    if(values[n][i])
                        pp.drawLine( i-t+num_points, neurons-n+1,
                                     i-t+num_points, neurons-n  );
        }

        painter->drawPixmap(0, 0, this->width() - margin_left,
                                  this->height()- margin_down, pixmap);
    }
    else{
        // Horizontal
        QPixmap pixmap(neurons, num_points);
        pixmap.fill(Qt::transparent);
        //pixmap.fill(Qt::gray); // For debugging. Makes this widget visible!

        QPainter pp(&pixmap);

        // Number on Axis
        pp.setPen(spColor);

        if(t < num_points){ // ZAF: is it correct in general???
            for(n=0; n<neurons; n++)
                for(i=0; i<t; i++)
                    if(values[n][i])
                        pp.drawLine( n+1, i, n, i);
        }
        else{
            for(n=0; n<neurons; n++)
                for(i=(t-num_points); i<t; i++)
                    if(values[n][i])
                        pp.drawLine( n+1, i-t+num_points, n, i-t+num_points);
        }

        painter->drawPixmap(0, 0, this->width() - margin_left,
                                  this->height()- margin_down, pixmap);
    }






}

void RasterPlot::drawAxes(QPainter *painter){
    if(!has_axes)
        return;

    painter->setPen(Qt::black);

    if(orientation == BasicGraph::HORIZONTAL){

        painter->drawText(x_axisX0, y_axisY0-15, name);

        // Label on Y axis
        painter->rotate(90);
        painter->drawText(gap, -y_axisX0 - 5,
                          "# neurons("+QString::number(neurons)+")");
        painter->rotate(-90);


        // Label on X axis
        int start;
        if (t < num_points)
            start = 0;
        else
            start = t - num_points;

        painter->drawText(x_axisX0 + 5, x_axisY0 + 15,
                          "t: " + QString::number(start) + " msec");
        painter->drawText(y_axisX1 - 50,x_axisY0 + 15,
                          QString::number(t) +" msec");

    }
    else{
        // Label on Y axis
        painter->rotate(90);
        painter->drawText(gap, -y_axisX0 - 5,
                          "t: "+QString::number(t) +" msec");
        painter->rotate(-90);

        // Label on X Axis
        painter->drawText(x_axisX0 + 5, x_axisY0 + 15,
                          "# neurons("+QString::number(neurons)+")");

    }

    painter->drawLine(x_axisX0, x_axisY0, x_axisX1, x_axisY1);
    painter->drawLine(y_axisX0, y_axisY0, y_axisX1, y_axisY1);
    for(i=0; i<t;i+=1000)
        painter->drawLine(50+i, x_axisY0, 50+i, x_axisY0+10);
    for(i=0; i<t;i+=100)
        painter->drawLine(50+i, x_axisY0, 50+i, x_axisY0+5);
}





















// -----------------------------------------------------------------------------
// -- PRINT STUFF --------------------------------------------------------------
// -----------------------------------------------------------------------------

void RasterPlot::print(QPainter *painter){
    painter->setPen(Qt::black);

    // X axis
    x_axisX02 = 50;
    x_axisY02 = 50 + neurons;
    x_axisX12 = 70 + t;
    x_axisY12 = 50 + neurons;

    // Y axis
    y_axisX02 = 50 + t;
    y_axisY02 = 50;
    y_axisX12 = 50 + t;
    y_axisY12 = 70 + neurons;

    if(neurons == 0)
        return;
    {// RASTER PLOT
        this->printAxes(painter, "Raster plot");

        // Number on Axis
        painter->drawText(y_axisX02-8, y_axisY02, QString::number(neurons));

        // Spikes
        painter->setPen(spColor);
        for(int k=0; k<neurons; k++){
            for(int i=0; i<t;i++)
                if(values[k][i])
                    painter->drawLine( 50 + i, x_axisY12 - (k+1),
                                       50 + i, x_axisY12 - (k  ));
        }
    }

    {// STATS
        //painter->setPen(Qt::black);
        //painter->drawText(20, y_axisY12+35, "time: "+QString::number(t));
    }

    // FREQUENCY PLOT

        // X axis
        x_axisY02 += 150;
        x_axisY12 += 150;

        // Y axis
        y_axisY02 += 150;
        y_axisY12 += 150;

        //SOS ZAF: print: "Frequency Plot"
        this->printAxes(painter, "");

        painter->setPen(Qt::red);

        float ratio = 100.0f/150.0f;//150/100;
        if(frequencies.size() == t){
            for(int i=1; i<frequencies.size();i++){
                painter->drawLine( 50 + i-1, x_axisY12 - frequencies[i-1]*ratio,
                                   50 + i,   x_axisY12 - frequencies[i]*ratio);
            }
        }
#ifndef LOGS
        else{
            std::cout << "NOT EVEN: frequencies.size(): "
                      << frequencies.size() << " t: " << t << std::endl;
        }
#endif

}

void RasterPlot::printAxes(QPainter *painter, QString title){
    painter->setPen(Qt::black);

    // Label, x and y axes
    painter->drawText(x_axisX02, y_axisY02-15, title);

    painter->drawText(x_axisX02 + 5, x_axisY02 + 12, "t: 0 msec");
    painter->drawText(y_axisX12 - 50,x_axisY02 + 12,QString::number(t)+" msec");

    painter->drawLine(x_axisX02, x_axisY02, x_axisX12, x_axisY12);
    painter->drawLine(y_axisX02, y_axisY02, y_axisX12, y_axisY12);
    for(i=0; i<t;i+=1000)
        painter->drawLine(50+i, x_axisY02, 50+i, x_axisY02+10);
    for(i=0; i<t;i+=100)
        painter->drawLine(50+i, x_axisY02, 50+i, x_axisY02+5);
}









