#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QDialog>
#include <QPainter>
#include <QImage>
#include <QKeyEvent>
#include <deque>
#include "architecturewindow.h"
#include "source/simulation/simulator.h"
#include "source/graphs/graphs.h"
#include "source/helper/firingrate.h"
#include "source/helper/userdata.h"

namespace Ui {
    class PlotWindow;
}

class PlotWindow : public QDialog{
    Q_OBJECT
private:
    int i, j;
protected:
    ArchitectureWindow *schema;
    Simulator *A;
    RasterPlot *rasterPlot;
    CurrentGraph *memPotGraph;
    CurrentGraph *LFPgraph;
    CurrentGraph *MFRgraph;
    PhasePortraitGraph *PPgraph;
    QString highlighted;
    int my_index;

    FiringRate MFR[10];
    double SUM;
    double MFRmax;
    int channelSize;
    int timer;

public:
    explicit PlotWindow(QString _highlighted, ArchitectureWindow *_schema,
                        Simulator *_A, const int &_my_index,
                        QWidget *parent = 0);
    ~PlotWindow();

    void setMyIndex(const int &value){ my_index = value; }

    void updateActivePlot();

    void printToJpeg();

private:
    Ui::PlotWindow *ui;

    //void paintEvent(QPaintEvent * event);
    void resizeEvent(QResizeEvent * event );
    void keyPressEvent(QKeyEvent * event);

signals:
    void plotWindowDeleteRequest(int my_index);

private slots:
    void on_printButton_clicked();
    void on_graphComboBox_currentIndexChanged(int index);
    void on_channelsSpinBox_valueChanged(int arg1);
    void on_depthSpinBox_valueChanged(int arg1);
    void on_updateRateSpinBox_valueChanged(int arg1);
    void on_lineThicknessSpinBox_valueChanged(int arg1);
    void on_rescaleButton_clicked();
    void on_exitButton_clicked();
    void on_verticalCheckBox_clicked(bool checked);
};

#endif // PLOTWINDOW_H







