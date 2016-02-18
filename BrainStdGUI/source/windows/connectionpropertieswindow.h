#ifndef BLOCKPROPERTIESWINDOW_H
#define BLOCKPROPERTIESWINDOW_H

#include <QDialog>
#include <QPainter>
#include <QKeyEvent>
#include "newwindow.h"
#include "architecturewidget.h"
#include "nemonetwork.h"
#include "graphs/graphs.h"

class CameraWindow;
class RobotWindow;
class NewWindow;

namespace Ui {
    class BlockPropertiesWindow;
}

class BlockPropertiesWindow : public QDialog
{
    Q_OBJECT
private:
    int i,j;
protected:
    ArchitectureWidget *schema;
    NemoNetwork *A;
    NewWindow *newWindow;
    BasicGraph *plot;
    CameraWindow *cameraWindow;
    RobotWindow *robotWindow;
    int highlighted;

    bool lockApply;
    bool vectorStimulus;
    bool singleNeuronStimulus;
    bool someNeuronStimulus;

    void refreshValues();

public:
    explicit BlockPropertiesWindow(ArchitectureWidget *_schema, NemoNetwork *_A,
                         CameraWindow* _cameraWindow=NULL, QWidget *parent = 0);
    ~BlockPropertiesWindow();

    void setHighlighted(int value);
    void setPlot(BasicGraph *p){ plot = p; }


private:
    Ui::BlockPropertiesWindow *ui;

    void paintEvent(QPaintEvent * event);
    //void keyPressEvent(QKeyEvent * event){ if(event->key()==Qt::Key_Escape) this->close(); }

private slots:
    void on_closeButton_clicked(){ this->close(); }
    /*void on_colourLineEdit_textChanged(){ this->update(); }
    void on_widthSlider_valueChanged(){ this->update(); }
    void on_heightSlider_valueChanged(){ this->update(); }*/
    void on_randomizeStimulusCheckBox_clicked(bool checked);
    void on_stimulusSlider_valueChanged(int value);
    void on_stimulusVectorSlider_valueChanged(int value);
    void on_stimulusSingleNeuronSlider_valueChanged(int value);
    void on_stimulusSlider_valueChanged();
    void on_stimulusVectorSlider_valueChanged();
    void on_stimulusSingleNeuronSlider_valueChanged();
    void on_aSlider_valueChanged();
    void on_bSlider_valueChanged();
    void on_cSlider_valueChanged();
    void on_dSlider_valueChanged();
    void on_aVarSlider_valueChanged();
    void on_bVarSlider_valueChanged();
    void on_cVarSlider_valueChanged();
    void on_dVarSlider_valueChanged();
    void on_sigmaSlider_valueChanged();
    void on_stimulusFullButton_clicked(){        singleNeuronStimulus = false;
                                                 vectorStimulus = false;
                                                 someNeuronStimulus = false;
                                                 on_stimulusSlider_valueChanged();}
    void on_stimulusVectorButton_clicked(){      singleNeuronStimulus = false;
                                                 vectorStimulus = true;
                                                 someNeuronStimulus = false;
                                                 on_stimulusVectorSlider_valueChanged();}
    void on_stimulusSingleNeuronButton_clicked(){singleNeuronStimulus=true;
                                                 vectorStimulus=false;
                                                 someNeuronStimulus = false;
                                                 on_stimulusSingleNeuronSlider_valueChanged();}
    void on_robotStimulusButton_clicked(){       singleNeuronStimulus=false;
                                                 vectorStimulus=false;
                                                 someNeuronStimulus = false;
                                                 on_stimulusSlider_valueChanged();}
    void on_someStimulusButton_clicked(bool checked);
    void on_tabWidget_currentChanged(int index);
};

#endif // BLOCKPROPERTIESWINDOW_H






