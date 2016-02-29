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
    int prev_stim_value;

    void refreshWidgetValues();
    void setStimulus(const int &value);
    void setStimulus();

    void update_plot();

public:
    explicit StimulationWidget(Block *_block, QWidget *parent=0);
    ~StimulationWidget();

private slots:
    // Stimulus
    void on_stimulusSlider_valueChanged(int value);
    void on_stimulusFullButton_clicked();
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

    void on_oscAmpSlider_valueChanged(int value);

signals:
    void setStimulus(QString blockName, const int &firstNeuron, const int &lastNeuron,
                        const double &maxStimulus, const double &_freq,
                        const int &_delay, const double &_phase);
    // void stopOscillation(const int &firstNeuron, const int &lastNeuron);

    void setStimulus(QString blockName, int neuronIndex, float value);
    void setStimulus(QString blockName, int firstNeur, int lastNeur, float value);
    void clearStimulus(QString blockName);
    void clearAllStimuli();
};

#endif // STIMULATIONWIDGET_H








