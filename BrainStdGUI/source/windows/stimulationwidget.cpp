#include "stimulationwidget.h"
#include "ui_stimulationwidget.h"

StimulationWidget::StimulationWidget(Block *_block, Simulator *_A,
                                     QWidget *parent): QWidget(parent),
                                     ui(new Ui::StimulationWidget){
    block = _block;
    A = _A;
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

    int firstN = block->getFirstNeuronIdx(), lastN = block->getLastNeuronIdx();

    // Stop previous stimulation
    if(A != NULL)
        A->setStimulus(firstN, lastN, 0.0f);

    if(ui->stimulusSomeButton->isChecked()){
        // Update simulator
        firstN += ui->startSpinBox->value();
        lastN = firstN + ui->sizeSpinBox->value();
        if(A != NULL)
            A->setStimulus(firstN, lastN, (float)value);

        // Update schema
        int first=firstN + block->getFirstNeuronIdx();
        int last =lastN  + block->getFirstNeuronIdx();
        block->setStimulus(value, first, last);
    }
    else{ // Single value
        // Update simulator
        if(A != NULL)
            A->setStimulus(firstN, lastN, (float)value);
        // Update schema
        block->setStimulus(value);
    }

    // OSCILLATIONS
    if(ui->oscCheckBox->isChecked()){
        // Update simulator
        if(A != NULL)
            A->setOscillation(firstN, lastN, value,
                              ui->oscFrequencySlider->value(),
                              ui->oscPhaseSpinBox->value(), 0.0);
    }
    else{
        // Update simulator
        if(A != NULL)
            A->stopOscillation(firstN, lastN);
    }
    // Update schema
    block->setOscillationFrequency(ui->oscFrequencySlider->value());
    block->setOscillationPhase(ui->oscPhaseSpinBox->value());
}

void StimulationWidget::setStimulus(){
    this->setStimulus(ui->stimulusSlider->value());
}




// -------------------------------------------------------------------------- //
//                            GUI methods                                     //
// -------------------------------------------------------------------------- //

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
                               -ui->startSpinBox->value());
    ui->stimulusSomeSlider->setValue(ui->startSpinBox->value());
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




























