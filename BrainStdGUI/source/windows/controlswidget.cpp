#include "controlswidget.h"
#include "ui_controlswidget.h"

ControlsWidget::ControlsWidget(ArchitectureWindow *_schema,
                   QWidget *parent):QWidget(parent), ui(new Ui::ControlsWidget){
    ui->setupUi(this);
    schema = _schema;
    ui->moreFrame->setVisible(false);
    this->resize(460, this->height());
    this->setEverythingFromSchema();
}

ControlsWidget::~ControlsWidget(){
    delete ui;
}

void ControlsWidget::setEverythingFromSchema(){
    if(schema==NULL){
        qDebug() << "ControlsWidget::setEverythingFromSchema: Error: "
                    "schema is NULL!";
        return;
    }
    ui->speedSlider->setValue(schema->getSpeed());
    // ZAF: continue with everything.....
    // ....
    // ...
    // ..
    // .
}

// -----------------------------------------------------------------------------
//   PUBLIC ACCESSORS
// -----------------------------------------------------------------------------
bool ControlsWidget::experiment(){ return ui->experimentCheckBox->isChecked(); }
bool ControlsWidget::learning(){ return ui->learningCheckBox->isChecked(); }
bool ControlsWidget::updateDetails(){ return ui->detailsCheckBox->isChecked(); }
bool ControlsWidget::updateSchema(){
    return ui->updateSchemaCheckBox->isChecked();
}
bool ControlsWidget::updateConnections(){
    return ui->connectionsCheckBox->isChecked();
}
bool ControlsWidget::modeStatesPixels() const { return (ui->modeStatesRadioButton->isChecked() &&
                                                        ui->statesType->currentText() == "Pixels");  }
bool ControlsWidget::modeEdit()         const { return ui->modeEditRadioButton->isChecked(); }
bool ControlsWidget::modeC()            const { return ui->modeCRadioButton->isChecked(); }
bool ControlsWidget::modeRasters()      const { return ui->modeRasterRadioButton->isChecked(); }
bool ControlsWidget::modeStatesPlots()  const { return (ui->modeStatesRadioButton->isChecked() &&
                                                        ui->statesType->currentText() == "2D plots"); }


// -----------------------------------------------------------------------------
//   SIGNALS eeeh???
// -----------------------------------------------------------------------------

void ControlsWidget::on_firingRateSlider_valueChanged(){
   ui->firingRateLabel->setText("FR depth: "+
                                QString::number(ui->firingRateSlider->value()));
   //schemaPtr()->setFiringRateDepth(firingRateSlider->value());
    if (schema != NULL) {
        schema->setFiringRateDepth(ui->firingRateSlider->value());
    }
}

void ControlsWidget::on_experimentCheckBox_clicked(bool checked){
    if(schema != NULL){
        if(checked){
            schema->showActions();
        }
        else{
            schema->hideActions();
        }
    }
    schema->update();
}


// -----------------------------------------------------------------------------
//   PUBLIC MODIFIERS
// -----------------------------------------------------------------------------






// -----------------------------------------------------------------------------
//   GUI SLOTS
// -----------------------------------------------------------------------------
void ControlsWidget::on_modeStatesRadioButton_clicked(){
    if(ui->statesType->currentText() == "Pixels")
        this->setModeStatesPixels();
    else if(ui->statesType->currentText() == "2D plots")
        this->setModeStatesPlots();
}
void ControlsWidget::on_statesType_currentIndexChanged(const QString &arg1){
    ui->modeStatesRadioButton->setChecked(true);
    this->on_modeStatesRadioButton_clicked();
}
void ControlsWidget::on_modeEditRadioButton_clicked(){
    this->setModeEdit();
}
void ControlsWidget::on_modeCRadioButton_clicked(){
    this->setModeC();
}
void ControlsWidget::on_modeRasterRadioButton_clicked(){
    this->setModeRasters();
}
void ControlsWidget::on_modeNetworkRadioButton_clicked(){
    this->setModeNetwork();
}
void ControlsWidget::on_speedSlider_valueChanged(int value){
    ui->speedLabel->setText("Speed (" + QString::number(value) + "ms)");
    emit setTimeSpeed(value);
}

// -------------------------------------------------------------------------- //
//                         'Change Mode' Buttons                              //
// -------------------------------------------------------------------------- //
void ControlsWidget::setModeStatesPixels(){
    if(schema == NULL){
        qDebug()<<"ControlsWidget::setModeStatesPixels: Error: schema is NULL!";
        return;
    }
    schema->setModeStatesPixels();
}
void ControlsWidget::setModeEdit(){
    if(schema == NULL){
        qDebug() << "ControlsWidget::setModeEdit: Error: schema is NULL!";
        return;
    }
    schema->setModeEdit();
}
void ControlsWidget::setModeC(){
    if(schema == NULL){
        qDebug() << "ControlsWidget::setModeC: Error: schema is NULL!";
        return;
    }
    schema->setModeC();
}
void ControlsWidget::setModeRasters(){
    if(schema == NULL){
        qDebug() << "ControlsWidget::setModeRasters: Error: schema is NULL!";
        return;
    }
    QMap<QString,Block*>::iterator it;
    for(it=schema->blocks.begin(); it!=schema->blocks.end(); it++)
        it.value()->setSpikeArray(NULL, -1);
    schema->setModeRasters();
}
void ControlsWidget::setModeStatesPlots(){
    if(schema == NULL){
        qDebug() <<"ControlsWidget::setModeStatesPlots: Error: schema is NULL!";
        return;
    }
    schema->setModeStatesPlots();
}
void ControlsWidget::setModeNetwork(){
    if(schema == NULL){
        qDebug() << "ControlsWidget::setModeNetwork: Error: schema is NULL!";
        return;
    }
    schema->setModeNetwork();
}
// -------------------------------------------------------------------------- //
//                  End of 'Change Mode' Buttons                              //
// -------------------------------------------------------------------------- //



void ControlsWidget::on_pushButton_clicked(){
    if(ui->pushButton->text() == "show more"){
        ui->pushButton->setText("show less");
        ui->moreFrame->setVisible(true);
        this->resize(655, this->height());
    }
    else{
        ui->pushButton->setText("show more");
        ui->moreFrame->setVisible(false);
        this->resize(460, this->height());
    }
}

void ControlsWidget::on_netAnalysischeckBox_clicked(bool checked){
    if(schema == NULL){
        qDebug() << "ControlsWidget::on_nacb_clicked: Error: schema is NULL!";
        return;
    }
    if(checked && ui->networkMode->currentText() == "Mutual information"){
        schema->setNetworkMode(Controls::NET_MI);
    }
    else if(checked && ui->networkMode->currentText() == "Transfer entropy"){
        schema->setNetworkMode(Controls::NET_TE);
    }
    else{
        schema->setNetworkMode(Controls::NET_NONE);
    }
}

void ControlsWidget::on_networkMode_activated(const QString &arg1){
    (void)arg1;
    this->on_netAnalysischeckBox_clicked(ui->netAnalysischeckBox->isChecked());
}



















