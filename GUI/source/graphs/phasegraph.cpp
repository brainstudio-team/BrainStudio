#include "phasegraph.h"

PhaseGraph::PhaseGraph(QString _name, int _phases, int _num_points,
                       int _x, int _y, int w, int h,
                       QWidget *parent) : BasicGraph(_name, 0,_x,_y,w,h,parent),
                       phases(_phases), num_points(_num_points) {

    values.resize(phases);
    for(i=0; i<phases; i++)
        values[i].resize(num_points);
    fc.resize(num_points);

    t = pointer = 0;
}

PhaseGraph::PhaseGraph(QString _name, int _phases, int _num_points,
                       QWidget *parent) : BasicGraph(_name, 0, parent),
                       phases(_phases), num_points(_num_points) {

    values.resize(phases);
    for(i=0; i<phases; i++)
        values[i].resize(num_points);
    fc.resize(num_points);

    t = pointer = 0;
}

void PhaseGraph::setAxes(){
    x_axisX0 = 0;
    x_axisY0 = 0.5*height();
    x_axisX1 = width();
    x_axisY1 = 0.5*height();

    y_axisX0 = 0.5*width();
    y_axisY0 = 0;
    y_axisX1 = 0.5*width();
    y_axisY1 = height();
}


void PhaseGraph::draw(QPainter *painter){
    //painter->drawText(y_axisX0 + 5, gap, "AAA");

    QColor c = QColor("");
    painter->setPen(QPen(c));

    painter->drawLine(x_axisX0, x_axisY0, x_axisX1, x_axisY1);
    painter->drawLine(y_axisX0, y_axisY0, y_axisX1, y_axisY1);

    painter->drawLine(0, 0, width(), height());


    int Rsmall = 10;
    int R, PosX, PosY;

    if(height() < width()){
        R = height()/2.0-Rsmall;
        PosX = width()/2.0 - R;
        PosY = Rsmall;
    }
    else{
        R = width()/2.0-Rsmall;
        PosX = Rsmall;
        PosY = height()/2.0 - R;
    }


    bool synch_to_col = true;

    float toDeg = 1.0f; //0.017453292

    painter->setPen(QPen(QBrush(Qt::black), 2));
    QColor c2 = QColor(100,100,200,100);
    painter->setBrush(QBrush(c2));
    painter->drawEllipse(PosX, PosY, 2*R, 2*R);

    painter->setPen(QPen(QBrush(Qt::black), 1));

    /*//if(highlighted != -1){
        painter->setBrush(QColor(0, 0, 255, 80));
        painter->drawEllipse(PosX, PosY, 2*R, 2*R);
    //}*/

    if(synch_to_col)
        painter->setBrush(QColor(fc[pointer]*255,25,255*(1-fc[pointer]),255));
    else
        painter->setBrush(QColor(25,25,255,255));

    for(int i=0; i<phases; i++){
        painter->drawEllipse(PosX+R+R*cos(values[i][pointer]*toDeg)-Rsmall,
                             PosY+R+R*sin(values[i][pointer]*toDeg)-Rsmall,
                             2*Rsmall, 2*Rsmall);
    }
}

void PhaseGraph::add_new_value(int _phase, float value){
    if(_phase >= phases)
        return;
    /*
    // Limits
    if(data_value < min)      data_value = min;
    else if(data_value > max) data_value = max;

    // Transformation
    data_value -= min;

    */
    if(_phase == 0){
        //if(keepHistory)
        //    history.push_back(data_value);
        t++;
        pointer = t % num_points;
    }

    values[_phase][pointer] = value;
}
