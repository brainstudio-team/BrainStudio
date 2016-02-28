#include "stimulationwidget.h"
#include "ui_stimulationwidget.h"

StimulationWidget::StimulationWidget(Block *_block, QWidget *parent):
                                 QWidget(parent), ui(new Ui::StimulationWidget){
    block = _block;
    initialized = false;

    ui->setupUi(this);
    this->move(0,0);

    ui->stimulusSlider->setMaximum(50);
    ui->startSpinBox->setMinimum(0);
    ui->sizeSpinBox->setMinimum(0);
    ui->sizeSpinBox->setValue(10);
    ui->stimulusSomeSlider->setMinimum(0);
    ui->maxSpinBox->setValue(ui->stimulusSlider->maximum());

    this->refreshWidgetValues();

    initialized = true;
}

StimulationWidget::~StimulationWidget(){
    delete ui;
}

void StimulationWidget::refreshWidgetValues(){
    ui->stimulusSlider->setValue((int)block->getStimulusAmount());
    ui->startSpinBox->setValue(block->getStimulusFirst());
    ui->startSpinBox->setMaximum(block->getNeuronsSize());
    ui->sizeSpinBox->setValue(block->getStimulusLast() -
                              block->getStimulusFirst());
    ui->sizeSpinBox->setMaximum(block->getNeuronsSize());

    ui->oscPhaseSpinBox->setValue((int)block->getOscillationPhase());
    ui->stimulusSomeSlider->setValue(ui->startSpinBox->value());
    ui->stimulusSomeSlider->setMaximum(ui->startSpinBox->maximum());

    ui->oscFrequencySlider->setValue(block->getOscillationFrequency());
    if(ui->oscCheckBox->isChecked())
        ui->sizeSpinBox->setEnabled(true);
}

void StimulationWidget::setStimulus(const int &value){
    if(!initialized)
        return;

    emit clearStimulus(block->getId());

    bool isOscillation = ui->oscCheckBox->isChecked();
    bool isBaseline = (ui->stimulusSlider->value() != 0);

    int blockFirst = block->getFirstNeuronIdx();
    int stimFirst = blockFirst + ui->startSpinBox->value();
    int stimLast = stimFirst + ui->sizeSpinBox->value();

    if (!isOscillation && !isBaseline) {
        emit clearStimulus(block->getId());
        block->setStimulus(0);

    } else if (!isOscillation && isBaseline) {
        emit setStimulus(block->getId(), stimFirst, stimLast, (float) value);
        block->setStimulus(value, stimFirst, stimLast);

    } else if (isOscillation && isBaseline) {

        double baseline = value;
        double amplitude = value;
        double frequency = ui->oscFrequencySlider->value();
        double phase = ui->oscPhaseSpinBox->value();

        emit setStimulus(block->getId(), stimFirst, stimLast, baseline, amplitude, frequency, phase);

        block->setStimulus(value, stimFirst, stimLast);
        block->setOscillationFrequency(ui->oscFrequencySlider->value());
        block->setOscillationPhase(ui->oscPhaseSpinBox->value());

    } else if (isOscillation && !isBaseline) {

        double baseline = value;
        double amplitude = value;
        double frequency = ui->oscFrequencySlider->value();
        double phase = ui->oscPhaseSpinBox->value();

        emit setStimulus(block->getId(), stimFirst, stimLast, baseline, amplitude, frequency, phase);

        block->setOscillationFrequency(ui->oscFrequencySlider->value());
        block->setOscillationPhase(ui->oscPhaseSpinBox->value());

    }

}

void StimulationWidget::setStimulus(){
    this->setStimulus(ui->stimulusSlider->value());
}




// -------------------------------------------------------------------------- //
//                            GUI methods                                     //
// -------------------------------------------------------------------------- //

void StimulationWidget::on_stimulusFullButton_clicked() {
    // If the "single value" option is checked, apply stimulus to all neurons
    if (ui->stimulusFullButton->isChecked()) {
        // Start in the first neuron
        ui->startSpinBox->setMaximum(block->getNeuronsSize());
        ui->startSpinBox->setMinimum(0);
        ui->startSpinBox->setValue(0);

        // Size of the stimulus is the whole block
        ui->sizeSpinBox->setMaximum(block->getNeuronsSize());
        ui->sizeSpinBox->setMinimum(0);
        ui->sizeSpinBox->setValue(block->getNeuronsSize());
    }
    this->setStimulus();
}

void StimulationWidget::on_stimulusSlider_valueChanged(int value){
    // Show value to GUI
    if(value) ui->stimLabel->setText("Current injection " +
                                                  QString::number(value)+" pA");
    else      ui->stimLabel->setText("Current injection (I=0)");

    this->setStimulus(value);
}

void StimulationWidget::on_oscFrequencySlider_valueChanged(int value){
    if(value > 0){
        ui->oscCheckBox->setText(
                                "Oscillation F: "+QString::number(value)+" Hz");
        if(!ui->oscCheckBox->isChecked()){
            ui->oscCheckBox->setChecked(true);
            ui->sizeSpinBox->setEnabled(true);
        }
    }
    else{
        ui->oscCheckBox->setText("Oscillation (No freq)");
        ui->oscCheckBox->setChecked(false);
        ui->sizeSpinBox->setEnabled(false);
    }

    this->setStimulus();
}


void StimulationWidget::on_stimulusSomeSlider_valueChanged(int value){
    // Custom stimulus
    ui->startSpinBox->setValue(value);
    ui->stimulusSomeButton->setText("From " + QString::number(value) + " to " +
                               QString::number(value+ui->sizeSpinBox->value()));

    ui->sizeSpinBox->setMaximum(block->getNeuronsSize()
                                  -ui->startSpinBox->value());
    this->setStimulus();
}

void StimulationWidget::on_startSpinBox_valueChanged(){
    // Custom stimulus
    ui->sizeSpinBox->setMaximum(block->getNeuronsSize()
                               - ui->startSpinBox->value());
    ui->stimulusSomeSlider->setValue(ui->startSpinBox->value());
    ui->stimulusSomeButton->setText("From " +
                                QString::number(ui->startSpinBox->value()) +
                                " to " +
                                QString::number(ui->startSpinBox->value()+
                                                ui->sizeSpinBox->value()));
    if(ui->startSpinBox->value() == 0 && ui->startSpinBox->value() +
            ui->sizeSpinBox->value() == block->getNeuronsSize()){
        ui->stimulusFullButton->setChecked(true);
    } else {
        ui->stimulusSomeButton->setChecked(true);
    }
    this->setStimulus();
}

void StimulationWidget::on_sizeSpinBox_valueChanged(){
    // Custom stimulus
    ui->startSpinBox->setMaximum(block->getNeuronsSize()
                                 -ui->sizeSpinBox->value());
    ui->stimulusSomeSlider->setMaximum(ui->startSpinBox->maximum());
    ui->stimulusSomeButton->setText("From " +
                                QString::number(ui->startSpinBox->value()) +
                                " to " +
                                QString::number(ui->startSpinBox->value()+
                                                ui->sizeSpinBox->value()));
    if(ui->startSpinBox->value() == 0 && ui->startSpinBox->value() +
            ui->sizeSpinBox->value() == block->getNeuronsSize()){
        ui->stimulusFullButton->setChecked(true);
    }
    else{
        ui->stimulusSomeButton->setChecked(true);
    }
    this->setStimulus();
}

void StimulationWidget::on_stimulusSomeButton_clicked(bool checked){
    ui->startSpinBox->setEnabled(checked);
    ui->sizeSpinBox->setEnabled(checked);
    this->setStimulus();
}


void StimulationWidget::on_negativeCheckBox_clicked(bool checked){
    if(checked){
        ui->stimulusSlider->setMinimum(-ui->maxSpinBox->value());
        ui->maxMinLabel->setText("Max/min:");
    }
    else{
        ui->stimulusSlider->setMinimum(0);
        ui->maxMinLabel->setText("Max:");
    }
}

void StimulationWidget::on_maxSpinBox_valueChanged(int arg1){
    if(arg1 > 0){
        ui->stimulusSlider->setMaximum(arg1);
        if(ui->negativeCheckBox->isChecked())
            ui->stimulusSlider->setMinimum(-arg1);
        else
            ui->stimulusSlider->setMinimum(0);
    }
    else{
        ui->maxSpinBox->setValue(1);
    }
}

void StimulationWidget::on_oscPhaseSpinBox_valueChanged(){
    this->setStimulus();
}


void StimulationWidget::on_oscCheckBox_clicked(bool checked){
    ui->oscPhaseSpinBox->setEnabled(checked);
}




























