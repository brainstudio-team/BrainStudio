#include "stimulationwidget.h"
#include "ui_stimulationwidget.h"

StimulationWidget::StimulationWidget(Block *_block, QWidget *parent):
                                 QWidget(parent), ui(new Ui::StimulationWidget){
    block = _block;
    initialized = false;
    prev_stim_value = 0;

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

    ui->plot->addGraph();
    ui->plot->graph(0)->removeFromLegend();

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
    if(ui->oscCheckBox->isChecked()){
        // TODO: Understand why???
        ui->sizeSpinBox->setEnabled(true);
    }
    this->on_oscCheckBox_clicked(ui->oscCheckBox->isChecked());
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
    QString name;
    if(ui->oscCheckBox->isChecked()){
        name = "Av. current injection ";
    }
    else{
        name = "Current injection ";
    }


    if(value){
        ui->stimLabel->setText(name + QString::number(value)+" pA");
    }
    else{
        ui->stimLabel->setText(name + "(I=0)");
    }

    // Oscillatory stuff
    if(ui->oscCheckBox->isChecked()){
        if(ui->oscAmpSlider->value() == prev_stim_value){
            ui->oscAmpSlider->setValue(value);
        }
        else if(ui->oscAmpSlider->value() > value){
            ui->oscAmpSlider->setValue(value);
        }
        this->update_plot();
    }

    prev_stim_value = value;
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

    this->update_plot();
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
        ui->oscAmpSlider->setMaximum(arg1);
        if(ui->negativeCheckBox->isChecked()){
            ui->stimulusSlider->setMinimum(-arg1);
            ui->oscAmpSlider->setMinimum(-arg1);
        }
        else
            ui->stimulusSlider->setMinimum(0);
        ui->oscAmpSlider->setMinimum(0);
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
    if(checked){
        this->setMaximumHeight(445);
        this->setMinimumHeight(445);
        ui->cFrame->setVisible(checked);
    }
    else{
        ui->cFrame->setVisible(checked);
        this->setMaximumHeight(292);
        this->setMinimumHeight(292);
    }
    this->resize(this->width(), this->maximumHeight());
}

void StimulationWidget::update_plot(){
    QVector<double> x, y;
    ui->plot->graph(0)->clearData();
    for(double i=0; i<5.0; i+=0.01){
        x.append(i);
        y.append(ui->stimulusSlider->value() + ui->oscAmpSlider->value()*
                 sin(2.0*M_PI*ui->oscFrequencySlider->value()*i));
    }
    ui->plot->graph(0)->setData(x,y);
    ui->plot->replot();
}

void StimulationWidget::on_oscAmpSlider_valueChanged(int value){
    this->update_plot();
}

























