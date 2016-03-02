#ifndef NEWBLOCKWINDOW_H
#define NEWBLOCKWINDOW_H

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui>
#include <QtGui/QPainter>
#include <iostream>
#include <stdlib.h>

#include "architecturewindow.h"
#include "source/helper/helperClasses.h"
#include "source/helper/modelValues.h"

#include "source/helper/backendData.h"

namespace Ui {
    class NewBlockWindow;
}

class NewBlockWindow : public QDialog
{
    Q_OBJECT

protected:
    ArchitectureWindow *schema;
    QPoint pos;

    Function f[3];
    float maxX;
    int SIZEX, SIZEY;
    QVector<NeuronUnit> neuronLoc;

    // Fields
    QGridLayout *fieldsLayout;
    QList<QWidget*> fieldsWidget;
    QList<QLabel*> fieldsLabel;

    // Parameters
    QGridLayout *parsLayout;
    QList<QLineEdit*> parsLineEdit;
    QList<QLabel*> parsLabel;

    // States
    QGridLayout *statesLayout;
    QList<QLineEdit*> statesLineEdit;
    QList<QLabel*> statesLabel;


    void createBlock(bool locations);

    void setCustom(int category);
    bool inhibitory(const int &category);
    void updateFunctionValues();

    void changeParameters(QString model);

    int getNeurons();

public:
    explicit NewBlockWindow(ArchitectureWindow *_schema, QPoint _pos,
                            QWidget *parent = 0);
    ~NewBlockWindow();

private:
    Ui::NewBlockWindow *ui;

    void paintEvent(QPaintEvent * event);

private slots:
    void on_addButton_clicked();
    void on_colourLineEdit_textChanged(){ this->update(); }
    void on_widthSlider_valueChanged(){ this->update(); }
    void on_heightSlider_valueChanged(){ this->update(); }

    void on_functionMaxX_valueChanged(double arg1){maxX = arg1; this->update();}


    void on_spatiallyEmbeddedCheckBox_clicked(bool checked);
    void on_spinBoxNeuronsX_valueChanged(int arg1);
    void on_spinBoxNeuronsY_valueChanged(int arg1);
    void on_neuronsSpinBox_valueChanged(int arg1);
    void on_exitButton_clicked();
    void on_neuronModelComboBox_currentIndexChanged(int index);


    void on_neuronModelComboBox_currentTextChanged(const QString &arg1);
    void on_parameterSetComboBox_currentTextChanged(const QString &paramSetName);
    void on_nameLineEdit_textChanged(const QString &arg1);
};

#endif // NEWBLOCKWINDOW_H




