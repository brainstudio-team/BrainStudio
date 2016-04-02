#include "currentgraph.h"


CurrentGraph::CurrentGraph(QString _name, int _lines, float _min, float _max,
                           int _x, int _y, int w, int h, QColor color1,
                           QColor color2, int _num_points, QWidget *parent):
                           BasicGraph(_name, 0, _x, _y, w, h, parent){
    this->commonConstruction(_lines, _min, _max, color1, color2, _num_points);
}

CurrentGraph::CurrentGraph(QString _name, int _lines, float _min, float _max,
                           QColor color1, QColor color2, int _num_points,
                           QWidget*parent):BasicGraph(_name, 0, parent){
    this->commonConstruction(_lines, _min, _max, color1, color2, _num_points);
}

void CurrentGraph::commonConstruction(int _lines, float _min, float _max,
                                 QColor color1, QColor color2, int _num_points){
    min = _min;
    max = _max;
    range = (int)(max - min);
    rangeF = max - min;

    if(_lines < 1) lines = 1;
    else if(_lines >= MAX_LINES) lines = MAX_LINES;
    else lines = _lines;

    // Randomize colours
    colour.push_back(color1);
    if(lines > 1)
    colour.push_back(color2);
    if(lines > 2)
    colour.push_back(QColor::fromRgb(180, 234, 117)); // Green
    if(lines > 3)
        for(i=3; i<lines; i++)
            colour.push_back(QColor::fromRgb(rand()%255,
                                             rand()%255,
                                             rand()%255));

    this->setAxes();
    this->setNumberOfPoints(_num_points);

    keepHistory = true;
}

void CurrentGraph::setAxes(){
    x_axisX0 = 0;
    x_axisY0 = height() - 2;
    x_axisX1 = width();
    x_axisY1 = height() - 2;

    y_axisX0 = width() - 30;
    y_axisY0 = 0;
    y_axisX1 = width() - 30;
    y_axisY1 = height();
}

void CurrentGraph::setNumberOfPoints(const int &_value){
    if(_value < (width()-15))
      num_points = _value;
    else
      num_points = width()-15;

    value.resize(lines);
    for(i=0; i<lines; i++)
        value[i].resize(num_points);

    t = 0;
    pointer = 0;
}

void CurrentGraph::draw(QPainter *painter){
    painter->drawText(y_axisX0 + 5, gap, QString::number(max, 'g', 3));
    painter->drawText(y_axisX0 + 5, y_axisY1-gap, QString::number(min, 'g', 3));

    ratio1 = ((float)y_axisX1)/num_points;
    ratio2 = ((float)(x_axisY1-y()))/rangeF;
    int j, jplus1;
    int x1,y1,x2,y2;
    for(line=0; line<lines; line++){
        painter->setPen(QPen(QBrush(colour[line]),thickness));
        for(i=0; i<(num_points-1); i++){
            j = (pointer+i+1)%num_points;
            jplus1 = (j+1)%num_points;
            x1 = i  *ratio1;
            y1 = x_axisY1-value[line][  j   ]*ratio2;
            x2 = (i+1)*ratio1;
            y2 = x_axisY1-value[line][jplus1]*ratio2;
            painter->drawLine(x1,y1,x2,y2);
        }
    }
    // ZAF SOS: Another improvement would be to have this two loops two times
    //          one for the different 'i' when i < pointer - 1 and another for
    //          the rest. With this I wont have to calc the modulo every time..
    // Nahh, it won't work.. :P
}


void CurrentGraph::add_new_value(int _line, float data_value){
    if(_line >= lines)
        return;

    // Limits
    if(data_value < min)      data_value = min;
    else if(data_value > max) data_value = max;

    // Transformation
    data_value -= min;

    if(_line == 0){
        if(keepHistory)
            history.push_back(data_value);
        t++;
        pointer = t % num_points;
    }

    value[_line][pointer] = data_value;
}



void CurrentGraph::print(QPainter *painter){
    painter->setPen(Qt::black);

    this->printAxes(painter, name);

    // Numbers on Axis
    painter->drawText(y_axisX02-8, y_axisY02, QString::number(max));
    //painter->drawText(y_axisX02-8, y_axisY02, QString::number(min));

    painter->setPen(Qt::green);

    ratio1 = ((float)x_axisY12)/rangeF;

    for(i=1; i<history.size(); i++)
        painter->drawLine(50 + i-1, x_axisY12 - history[i-1]*ratio1,
                          50 + i,   x_axisY12 - history[ i ]*ratio1);
}



void CurrentGraph::setMaxValue(const float &value){
    max = value;
    range = (int)(max - min);
    rangeF = max - min;
}

void CurrentGraph::setMinValue(const float &value){
    min = value;
    range = (int)(max - min);
    rangeF = max - min;
}




























