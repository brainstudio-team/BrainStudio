#include "basicgraph.h"

BasicGraph::BasicGraph(QString _name,int _range,int _x,int _y,int w,int h,
                       QWidget *parent ): QWidget(parent), range(_range){
    move(_x,_y);
    setFixedSize(w, h);
    name = _name;
    setAxes();

    gap = 10;
    step = 5;
    thickness = 0;
}

BasicGraph::BasicGraph(QString _name, int _range, QWidget *parent ):
                       QWidget(parent), range(_range){
    name = _name;
    setAxes();

    gap = 10;
    step = 5;
    thickness = 0;
}

void BasicGraph::draw(QPainter *painter){
    painter->setPen(Qt::black);
}

void BasicGraph::drawAxes(QPainter *painter){
    painter->setPen(Qt::black);
    // Label
    painter->drawText(10, 10, name);
    // X axis
    painter->drawLine(x_axisX0, x_axisY0, x_axisX1, x_axisY1);
    // Y axis
    painter->drawLine(y_axisX0, y_axisY0, y_axisX1, y_axisY1);
}

void BasicGraph::print(QPainter *painter){
    draw(painter);
}

void BasicGraph::paintEvent(QPaintEvent * event){
    QPainter painter(this);
    painter.setPen(Qt::black);
    drawAxes(&painter);
    draw(&painter);
}

void BasicGraph::resizeEvent(QResizeEvent * event){
    (void)event;
    setAxes();
}


void BasicGraph::setAxes(){
    x_axisX0 = 0;
    x_axisY0 = height()-0.05*width();
    x_axisX1 = width();
    x_axisY1 = height()-0.05*width();

    y_axisX0 = width()-0.05*width();
    y_axisY0 = 0;
    y_axisX1 = width()-0.05*width();
    y_axisY1 = height();
}

void BasicGraph::printAxes(QPainter *painter, QString title){
    painter->setPen(Qt::black);

    // Label, x and y axes
    painter->drawText(x_axisX02, y_axisY02-15, title);

    painter->drawLine(x_axisX02, x_axisY02, x_axisX12, x_axisY12);
    painter->drawLine(y_axisX02, y_axisY02, y_axisX12, y_axisY12);
    for(i=0; i<(x_axisX12-70);i+=1000)
        painter->drawLine(50+i, x_axisY02, 50+i, x_axisY02+10);
    for(i=0; i<(x_axisX12-70);i+=100)
        painter->drawLine(50+i, x_axisY02, 50+i, x_axisY02+5);
}
