#ifndef BLOCKPROPERTIESWIDGET_H
#define BLOCKPROPERTIESWIDGET_H

#include <QDialog>
#include "source/model/block.h"
#include "source/simulation/simulator.h"

class MainWindow;

namespace Ui {
    class BlockPropertiesWidget;
}

class BlockPropertiesWidget : public QWidget{
    Q_OBJECT
private:
    int i,j;
protected:
    Block *block;
    Simulator *A;

    void refreshValues();

    // Fields
    QGridLayout *fieldsLayout;
    QList<QWidget*> fieldsWidget;
    QList<QLabel*> fieldsLabel;

    // Parameters
    QGridLayout *paramsLayout;
    QList<QLineEdit*> parsLineEdit;
    QList<QLabel*> parsLabel;

    // States
    QGridLayout *statesLayout;
    QList<QLineEdit*> statesLineEdit;
    QList<QLabel*> statesLabel;

public:
    explicit BlockPropertiesWidget(Block *_block, Simulator *_A, QWidget *parent = 0);
    ~BlockPropertiesWidget();

    void setSimulatorPointer(Simulator *_snn){ A = _snn; }

private:
    Ui::BlockPropertiesWidget *ui;

    void changeParameters();
    void changeToDefault(QString model);

private slots:
    void parametersChanged();

};

#endif // BLOCKPROPERTIESWIDGET_H

