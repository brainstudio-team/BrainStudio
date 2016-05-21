#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H

#include <QWidget>
#include <QDebug>
#include <QMouseEvent>

#include "source/simulation/simulator.h"
#include "source/model/block.h"

namespace Ui {
class RightPanel;
}

class RightPanel : public QWidget{
    Q_OBJECT

    Simulator *sim;
    Block *block;

public:
    explicit RightPanel(Simulator *_sim, Block *_b, QWidget *parent = 0);
    ~RightPanel();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void on_stimulationButton_clicked();
    void on_connectionButton_clicked();
    void on_actionsButton_clicked();
    void on_plotButton_clicked();

    void on_actionExecuteButton_clicked();

private:
    Ui::RightPanel *ui;
};

#endif // RIGHTPANEL_H
