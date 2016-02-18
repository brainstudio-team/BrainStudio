#ifndef STIMULATIONWIDGET_H
#define STIMULATIONWIDGET_H

#include "source/model/block.h"
#include "source/simulation/simulator.h"

class ArchitectureWindow;

namespace Ui {
    class StimulationWidget;
}

class StimulationWidget : public QWidget{
    Q_OBJECT
private:
    Ui::StimulationWidget *ui;

protected:
    Block *block;
    Simulator *A;
    bool initialized;

    void refreshWidgetValues();
    void setStimulus(const int &value);
    void setStimulus();

public:
    explicit StimulationWidget(Block *_block, Simulator *_A, QWidget *parent=0);
    ~StimulationWidget();

    void setSimulatorPointer(Simulator *_snn){ A = _snn; }

private slots:
    // Stimulus
    void on_stimulusSlider_valueChanged(int value);
    void on_stimulusFullButton_clicked(){ this->setStimulus(); }
    void on_stimulusSomeButton_clicked(bool checked);
    void on_stimulusSomeSlider_valueChanged(int value);
    void on_startSpinBox_valueChanged();
    void on_sizeSpinBox_valueChanged();
    void on_negativeCheckBox_clicked(bool checked);
    void on_maxSpinBox_valueChanged(int arg1);

    // Oscillation
    void on_oscCheckBox_clicked(bool checked);
    void on_oscFrequencySlider_valueChanged(int value);
    void on_oscPhaseSpinBox_valueChanged();
};

#endif // STIMULATIONWIDGET_H








