#include "phaseportraitgraph.h"

PhasePortraitGraph::PhasePortraitGraph(int _lines, double _minX, double _maxX,
                    double _minY, double _maxY, int _x, int _y, int w, int h,
                    QColor color1, QColor color2, int _num_points,
                    QWidget *parent):BasicGraph("", 0, _x, _y, w, h, parent),
                    minX(_minX), minY(_minY), maxX(_maxX), maxY(_maxY),
                    num_points(_num_points){
    //range = (int)(max - min);
    //rangeF = max - min;
    rangeX = maxX - minX;
    rangeY = maxY - minY;

    if(_lines < 1) lines = 1;
    else if(_lines >= MAX_LINES) lines = MAX_LINES;
    else lines = _lines;

    // Randomize colours
    colour.push_back(color1);
    if(lines > 1)
    colour.push_back(color2);
    if(lines > 2)
        for(i=2; i<lines; i++)
            colour.push_back(QColor::fromRgb(rand()%255,
                                             rand()%255,
                                             rand()%255));

    setAxes();



    valueX.resize(lines);
    valueY.resize(lines);
    for(i=0; i<lines; i++){
        valueX[i].resize(num_points);
        valueY[i].resize(num_points);
    }

    t = 0;
    pointer = 0;
    keepHistory = true;
}

void PhasePortraitGraph::setAxes(){
    x_axisX0 = 0;
    x_axisY0 = height()-0.05*height();
    x_axisX1 = width();
    x_axisY1 = height()-0.05*height();

    y_axisX0 = width()-0.05*width();
    y_axisY0 = 0;
    y_axisX1 = width()-0.05*width();
    y_axisY1 = height();
}

void PhasePortraitGraph::draw(QPainter *painter){
    painter->drawText(0.96*(double)width(), 0.10*(double)height(), QString::number(maxY));
    painter->drawText(0.96*(double)width(), 0.90*(double)height(), QString::number(minY));

    ratio1 = ((double)y_axisX1)/rangeX;
    ratio2 = ((double)(x_axisY1-y()))/rangeY;
    int j, jplus1;
    int x1,y1,x2,y2;
    for(line=0; line<lines; line++){
        painter->setPen(colour[line]);
        for(i=0; i<(num_points-1); i++){
            //j = (pointer+i+1)%num_points;
            //jplus1 = (j+1)%num_points;
            x1 = valueX[line][ i ]*ratio1; y1 = x_axisY1-valueY[line][ i ]*ratio2;
            x2 = valueX[line][i+1]*ratio1; y2 = x_axisY1-valueY[line][i+1]*ratio2;
            //if(abs(x2-x1) < 10  && abs(y2-y1) < 10)
            if( i != pointer && abs(x2-x1) < 70  && abs(y2-y1) < 70)
                painter->drawLine(x1,y1,x2,y2);
        }
        painter->drawEllipse(QPoint( valueX[line][pointer] * ratio1,
                            x_axisY1-valueY[line][pointer] * ratio2), 3, 3);
    }

    // ZAF SOS: Another improvement would be to have this two loops two times
    //          one for the different 'i' when i < pointer - 1 and another for
    //          the rest. With this I wont have to calc the modulo every time..
    // Nahh, it won't work.. :P
}


void PhasePortraitGraph::add_new_value(const int &_line,double value_x,
                                                        double value_y){
    if(_line >= lines)
        return;

    // Limits
    if(value_x < minX)      value_x = minX;
    else if(value_x > maxX) value_x = maxX;
    if(value_y < minY)      value_y = minY;
    else if(value_y > maxY) value_y = maxY;

    // Transformation
    value_x -= minX;
    value_y -= minY;

    if(_line == 0){
        if(keepHistory){
            historyX.push_back(value_x);
            historyY.push_back(value_y);
        }
        t++;
        pointer = t % num_points;
    }

    valueX[_line][pointer] = value_x;
    valueY[_line][pointer] = value_y;
}



void PhasePortraitGraph::print(QPainter *painter){
    painter->setPen(Qt::black);

    this->printAxes(painter, name);

    // Numbers on Axis
    painter->drawText(y_axisX02-8, y_axisY02, QString::number(maxY));
    //painter->drawText(y_axisX02-8, y_axisY02, QString::number(minY));

    painter->setPen(Qt::green);

    ratio1 = ((double)x_axisY12)/rangeY;

    for(i=1; i<historyX.size(); i++)
        painter->drawLine(
                  50 + historyX[i-1]*ratio1, x_axisY12 - historyY[i-1]*ratio1,
                  50 + historyX[ i ]*ratio1, x_axisY12 - historyY[ i ]*ratio1);
}
































