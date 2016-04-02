#include "stimulationwidget.h"
#include "ui_stimulationwidget.h"

StimulationWidget::StimulationWidget(Block *_block, QWidget *parent):
                                 QWidget(parent), ui(new Ui::StimulationWidget){
    block = _block;
    prev_stim_value = 0;

    fbands["delta"] = QPoint(0.1, 3.0);
    fbands["theta"] = QPoint(4.0, 7.0);
    fbands["alpha"] = QPoint(8.0, 15.0);
    fbands["beta"] = QPoint(15.0, 30.0);
    fbands["gamma"] = QPoint(30.0, 100.0);

    this->move(0,0);

    ui->setupUi(this);
    ui->plot->addGraph();
    ui->plot->graph(0)->removeFromLegend();

    initialized = false;
    this->refreshWidgetValues();
    initialized = true;
}

StimulationWidget::~StimulationWidget(){
    delete ui;
}

/* Refresh all widgets based on the stimulation values in the block
 * SOS: Always setMaximum() should be called before setValue()!
 */
void StimulationWidget::refreshWidgetValues(){
    Stimulus stimulus = block->getStimulus();

    // Setting boarders
    if(stimulus.baseline == 0.0){
        ui->maxSpinBox->setValue(500);
    }
    else if(abs((int)stimulus.baseline) < 500){
        ui->maxSpinBox->setValue(500);
    }
    else{
        ui->maxSpinBox->setValue((int)stimulus.baseline);
    }
    if(stimulus.baseline < 0.0){
        ui->negativeCheckBox->setChecked(true);
    }
    else{
        ui->negativeCheckBox->setChecked(false);
    }
    ui->stimulusSlider->setMaximum(ui->maxSpinBox->value());
    ui->oscAmpSlider->setMaximum(ui->maxSpinBox->value());
    if(ui->negativeCheckBox->isChecked()){
        ui->stimulusSlider->setMaximum(-ui->maxSpinBox->value());
        ui->oscAmpSlider->setMaximum(-ui->maxSpinBox->value());
    }
    ui->stimulusSlider->setValue((int)stimulus.baseline);

    // Widgets that are updated in the same way for any stimulus
    ui->startSpinBox->setMaximum(stimulus.lastNeuron);
    ui->startSpinBox->setValue(stimulus.firstNeuron);

    ui->stimulusSomeSlider->setMaximum(ui->startSpinBox->maximum());
    ui->stimulusSomeSlider->setValue(ui->startSpinBox->value());

    ui->sizeSpinBox->setMaximum(block->getNeuronsSize());
    ui->sizeSpinBox->setValue(stimulus.lastNeuron - stimulus.firstNeuron);

    // Oscillations
    ui->oscPhaseSpinBox->setValue(stimulus.phase);
    ui->oscFrequencySlider->setValue(stimulus.frequency);
    ui->oscAmpSlider->setValue((int)stimulus.amplitude);
    ui->oscCheckBox->setChecked(ui->oscFrequencySlider->value() > 0);
    this->on_oscCheckBox_clicked(ui->oscCheckBox->isChecked());

    this->update_plot();
    this->update_labels();
}

void StimulationWidget::setStimulus(){
    if(!initialized)
        return;

    emit clearStimulus(block->getId());

    bool isOscillation = ui->oscCheckBox->isChecked();
    bool isBaseline = (ui->stimulusSlider->value() != 0);

    int blockFirst = block->getFirstNeuronIdx();
    int stimFirst = blockFirst + ui->startSpinBox->value();
    int stimLast = stimFirst + ui->sizeSpinBox->value();
    double baseline = (double)ui->stimulusSlider->value();

    if (!isOscillation && !isBaseline){
        // For the schema
        block->setStimulus(Stimulus(block->getFirstNeuronIdx()-blockFirst,
                                    block->getLastNeuronIdx()-blockFirst));
        // For the backend
        emit clearStimulus(block->getId());
    }
    else if (!isOscillation && isBaseline) {
        // For the schema
        block->setStimulus(Stimulus(stimFirst-blockFirst,
                                    stimLast-blockFirst, baseline));
        // For the backend
        emit setStimulus(block->getId(), stimFirst, stimLast, (float)baseline);
    }
    else if (isOscillation && isBaseline) {
        double amplitude = ui->oscAmpSlider->value();
        double frequency = ui->oscFrequencySlider->value();
        double phase = ui->oscPhaseSpinBox->value();

        // For the schema
        block->setStimulus(Stimulus(stimFirst-blockFirst, stimLast-blockFirst,
                                    baseline, amplitude, frequency, phase));
        // For the backend
        emit setStimulus(block->getId(), stimFirst, stimLast, baseline,
                         amplitude, frequency, phase);
    }

}

/* Update the labels of the GUI to account for the changes in sliders
 */
void StimulationWidget::update_labels(){
    int baseline = ui->stimulusSlider->value();
    int frequency = ui->oscFrequencySlider->value();
    int start = ui->stimulusSomeSlider->value();
    int last = start + ui->sizeSpinBox->value();
    bool negativeStim = ui->negativeCheckBox->isChecked();
    bool oscillations = ui->oscCheckBox->isChecked();

    ui->stimulusSomeButton->setText("From " + QString::number(start) + " to " +
                                    QString::number(last));

    if(start != 0 || last != block->getNeuronsSize()){
        ui->stimulusFullButton->setChecked(false);
        ui->stimulusSomeButton->setChecked(true);
    }
    else{
        ui->stimulusFullButton->setChecked(true);
        ui->stimulusSomeButton->setChecked(false);
    }

    QString name;
    if(oscillations){
        name = "Av. current injection ";
    }
    else{
        name = "Current injection ";
    }

    if(baseline){
        ui->stimLabel->setText(name + QString::number(baseline)+" pA");
    }
    else{
        ui->stimLabel->setText(name + "(I=0)");
    }

    if(frequency > 0){
        QString band = "";
        for(QMap<QString,QPoint>::iterator it=fbands.begin();
                                                      it != fbands.end(); it++){
            if(it.value().x() < frequency && frequency < it.value().y()){
                band += " (" + it.key() + " band)";
            }
        }

        ui->oscCheckBox->setText("Oscillation F: " +
                                 QString::number(frequency) + " Hz" + band);
    }
    else{
        ui->oscCheckBox->setText("Oscillation (No freq)");
    }

    if(negativeStim){
        ui->maxMinLabel->setText("Max/min:");
    }
    else{
        ui->maxMinLabel->setText("Max:");
    }
}

void StimulationWidget::update_plot(){
    QVector<double> x, y;
    double plot_range = 1.0;

    // PEDRO: TODO: hack, hack, hackity hack
    // ui->plot->graph(0)->clearData();
    if (ui->plot->graph(0)) {
        ui->plot->graph(0)->clearData();
    }


    if(ui->oscFrequencySlider->value() > 0){
        plot_range = 5.0*1.0/ui->oscFrequencySlider->value();

        for(QMap<QString,QPoint>::iterator it=fbands.begin();
                                                      it != fbands.end(); it++){
            if(it.value().x() < ui->oscFrequencySlider->value() &&
                    ui->oscFrequencySlider->value() < it.value().y()){
                plot_range = 5.0*1.0/it.value().x();
            }
        }
    }
    ui->plot->xAxis->setRange(0, plot_range);

    ui->plot->yAxis->setRange(0, ui->stimulusSlider->value() +
                                 ui->oscAmpSlider->value());

    for(double i=0; i<plot_range; i+=plot_range/200.0){
        x.append(i);
        y.append(ui->stimulusSlider->value() + ui->oscAmpSlider->value()*
                 sin(2.0*M_PI*ui->oscFrequencySlider->value()*i - ui->oscPhaseSpinBox->value()));
    }

    // PEDRO: TODO: hack, hack, hackity hack
    // ui->plot->graph(0)->setData(x,y);
    if (ui->plot->graph(0)) {
        ui->plot->graph(0)->setData(x,y);
    }
    ui->plot->replot();
}


// -------------------------------------------------------------------------- //
//                            GUI methods                                     //
// -------------------------------------------------------------------------- //

void StimulationWidget::on_stimulusFullButton_clicked() {
    if(!initialized)
        return;
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
    this->update_labels();
    this->setStimulus();
}

void StimulationWidget::on_stimulusSomeButton_clicked(){
    if(!initialized)
        return;
    this->update_labels();
    this->setStimulus();
}

void StimulationWidget::on_stimulusSlider_valueChanged(int value){
    if(!initialized)
        return;

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
    this->update_labels();
    this->setStimulus();
}

void StimulationWidget::on_oscFrequencySlider_valueChanged(int value){
    if(!initialized)
        return;
    if(value > 0 && !ui->oscCheckBox->isChecked()){
        ui->oscCheckBox->setChecked(true);
        on_oscCheckBox_clicked(true);
    }
    else if(value <= 0){
        ui->oscCheckBox->setChecked(false);
        on_oscCheckBox_clicked(false);
    }

    this->update_plot();
    this->update_labels();
    this->setStimulus();
}


void StimulationWidget::on_stimulusSomeSlider_valueChanged(int value){
    if(!initialized)
        return;
    // Custom stimulus
    ui->startSpinBox->setValue(value);
    ui->sizeSpinBox->setMaximum(block->getNeuronsSize()
                                  -ui->startSpinBox->value());
    this->update_labels();
    this->setStimulus();
}

void StimulationWidget::on_startSpinBox_valueChanged(){
    if(!initialized)
        return;
    // Custom stimulus
    ui->sizeSpinBox->setMaximum(block->getNeuronsSize()
                               - ui->startSpinBox->value());
    ui->stimulusSomeSlider->setValue(ui->startSpinBox->value());
    this->update_labels();
    this->setStimulus();
}

void StimulationWidget::on_sizeSpinBox_valueChanged(){
    if(!initialized)
        return;
    // Custom stimulus
    ui->startSpinBox->setMaximum(block->getNeuronsSize()
                                 -ui->sizeSpinBox->value());
    ui->stimulusSomeSlider->setMaximum(ui->startSpinBox->maximum());
    this->update_labels();
    this->setStimulus();
}

void StimulationWidget::on_negativeCheckBox_clicked(bool checked){
    if(!initialized)
        return;
    if(checked){
        ui->stimulusSlider->setMinimum(-ui->maxSpinBox->value());
    }
    else{
        ui->stimulusSlider->setMinimum(0);
    }
    this->update_labels();
}

void StimulationWidget::on_maxSpinBox_valueChanged(int arg1){
    if(!initialized)
        return;
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
    if(!initialized)
        return;
    this->update_plot();
    this->update_labels();
    this->setStimulus();
}

                                                                                    // SOS     if(!initialized) return;

void StimulationWidget::on_oscCheckBox_clicked(bool checked){
    ui->oscPhaseSpinBox->setEnabled(checked);
    int height_with, height_without;

#ifdef WIN32
    height_with = 445;
    height_without = 292;
#elif linux
    height_with = 460;
    height_without = 307;
#else
    height_with = 460;
    height_without = 307;
#endif

    if(checked){
        this->setMaximumHeight(height_with);
        this->setMinimumHeight(height_with);
        ui->cFrame->setVisible(checked);
    }
    else{
        ui->cFrame->setVisible(checked);
        this->setMaximumHeight(height_without);
        this->setMinimumHeight(height_without);
    }
    this->resize(this->width(), this->maximumHeight());
    this->update_labels();
    this->setStimulus();
}

void StimulationWidget::on_oscAmpSlider_valueChanged(int value){
    if(!initialized)
        return;
    if(ui->oscAmpSlider->value() > ui->stimulusSlider->value()){
        ui->stimulusSlider->setValue(value);
    }
    ui->oscCheckBox->setChecked(true);
    on_oscCheckBox_clicked(true);
    this->update_plot();
    this->update_labels();
    this->setStimulus();
}

void StimulationWidget::on_exitButton_clicked(){
    this->close();
}
