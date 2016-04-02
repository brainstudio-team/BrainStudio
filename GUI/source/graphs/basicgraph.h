#ifndef BASICGRAPH_H
#define BASICGRAPH_H

#include <QtCore/QString>
#include <QtGui/QPainter>
#include <QtWidgets>

class BasicGraph : public QWidget{
    Q_OBJECT
protected:
    QString name;

    int num_points;
    int start;

    int gap;
    int step;
    int range;

    int i;
    int thickness;


    bool has_axes;

    // Drawing Axes
    int x_axisX0, x_axisY0;
    int x_axisX1, x_axisY1;

    int y_axisX0, y_axisY0;
    int y_axisX1, y_axisY1;

    // Drawing Axes
    int x_axisX02, x_axisY02;
    int x_axisX12, x_axisY12;

    int y_axisX02, y_axisY02;
    int y_axisX12, y_axisY12;


    void paintEvent(QPaintEvent * event);
    void resizeEvent(QResizeEvent * event);

public:
    BasicGraph(QString _name,int _range,int _x,int _y,int w,int h,
               QWidget *parent = 0);
    BasicGraph(QString _name, int _range, QWidget *parent = 0);
    ~BasicGraph(){}

    enum {VERTICAL, HORIZONTAL};

    // ACCESSORS
    virtual int getTime(){ return -1; }

    // MODIFIERS
    virtual void setX(int value){ move(value, y()); setAxes(); }
    virtual void setY(int value){ move(x(), value); setAxes(); }
    virtual void setWidth(int value){ setFixedSize(value, height()); setAxes();}
    virtual void setHeight(int value){ setFixedSize(width(), value); setAxes();}
    virtual void setThickness(const int &value){ thickness = value; }
    virtual void setOrientation(const bool &value) { (void)value; }

    void hasAxis(const bool &yes){ has_axes = yes; this->setAxes();}

    virtual void setAxes();
    void setPrintAxes(const int &xAxisX0, const int &xAxisY0,
                      const int &xAxisX1, const int &xAxisY1,
                      const int &yAxisX0, const int &yAxisY0,
                      const int &yAxisX1, const int &yAxisY1){
        x_axisX02=xAxisX0;x_axisY02=xAxisY0;x_axisX12=xAxisX1;x_axisY12=xAxisY1;
        y_axisX02=yAxisX0;y_axisY02=yAxisY0;y_axisX12=yAxisX1;y_axisY12=yAxisY1;
    }
    virtual void draw(QPainter *painter);
    virtual void drawAxes(QPainter *painter);
    virtual void print(QPainter *painter);
    virtual void printAxes(QPainter *painter, QString title);
    virtual void next_step(){}
    virtual void next_step(float freq){ (void)freq; }
    virtual void add_new_value(int line_num,float data_value)
                                             {(void)line_num; (void)data_value;}
    virtual void add_new_value(const int &_line, double value_x, double value_y)
                                                     {_line; value_x; value_y; }
    virtual void add_spike(int neuronId){ (void)neuronId; }
};

#endif // BASICGRAPH_H







