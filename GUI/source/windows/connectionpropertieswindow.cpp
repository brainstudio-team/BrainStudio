#include "blockpropertieswindow.h"
#include "ui_blockpropertieswindow.h"

BlockPropertiesWindow::BlockPropertiesWindow(ArchitectureWidget *_schema,
                 NemoNetwork *_A, CameraWindow *_cameraWindow, QWidget *parent):
                             QDialog(parent), ui(new Ui::BlockPropertiesWindow){
    schema = _schema;
    A = _A;
    cameraWindow = _cameraWindow;
    lockApply = false;
    highlighted = -1;
    vectorStimulus = false;
    singleNeuronStimulus = false;
    someNeuronStimulus = false;
    newWindow = (NewWindow*)parent;
    if(newWindow != NULL)
        plot = newWindow->getPlot();
    ui->setupUi(this);
    this->move(1555,400);
    if(plot != NULL && plot->getWidth() < 350-15)
        this->setMinimumSize(350, 500);
    else if(plot != NULL)
        this->setMinimumSize(plot->getWidth()+30, 500);
    else
        this->setMinimumSize(350, 500);

    ui->startSpinBox->setMinimum(0);
    ui->endSpinBox->setMinimum(0);
}

BlockPropertiesWindow::~BlockPropertiesWindow(){
    delete ui;
}


void BlockPropertiesWindow::paintEvent(QPaintEvent * event){
    QPainter painter(this);
    if(highlighted >= 0){
        if(ui->rasterPlotRadioButton->isChecked() && plot != NULL)
            plot->draw(&painter);
        //else if(ui->current1RadioButton->isChecked() && currentPlot1 != NULL)
    }
/*    QColor c1;
    if(ui->colourLineEdit->text().length() == 7)
        c1.setNamedColor(ui->colourLineEdit->text());
    else
        c1.setNamedColor("#ffffff");
    painter.setBrush(QBrush(c1));
    painter.drawRect(25,
                     320-ui->heightSlider->value(),
                     ui->widthSlider->value(),
                     ui->heightSlider->value());
    painter.drawText(30,300,"("+QString::number(pos.x()) +", " +
                            QString::number(pos.y()) + ")");
    painter.drawText(30,315,"("+QString::number(ui->widthSlider->value())+", " +
                            QString::number(ui->heightSlider->value()) + ")");*/
}


void BlockPropertiesWindow::setHighlighted(int value){
    highlighted = value;
    lockApply = true;
    if(value >=0){
        this->setWindowTitle(schema->blocks[value].getName()+"'s properties");
        ui->stimulusSlider->setValue(schema->blocks[value].getStimulus());
        ui->stimulusVectorSlider->setValue(schema->blocks[value].getAngle());
        ui->stimulusSingleNeuronSlider->setMaximum(
                                        schema->blocks[value].getNeuronsNo()-1);
        ui->stimulusSingleNeuronSlider->setValue(
                                 schema->blocks[value].getNeuronWithStimulus());
        ui->aSlider->setValue(schema->blocks[value].getA()*1000);
        ui->bSlider->setValue(schema->blocks[value].getB()*1000);
        ui->cSlider->setValue(schema->blocks[value].getC()*10);
        ui->dSlider->setValue(schema->blocks[value].getD()*10);
        ui->aVarSlider->setValue(schema->blocks[value].getAVar()*1000);
        ui->bVarSlider->setValue(schema->blocks[value].getBVar()*1000);
        ui->cVarSlider->setValue(schema->blocks[value].getCVar()*10);
        ui->dVarSlider->setValue(schema->blocks[value].getDVar()*10);
        ui->sigmaSlider->setValue(schema->blocks[value].getSigma()*100);
        ui->randomizeStimulusCheckBox->setChecked(false);
        ui->startSpinBox->setMaximum(schema->blocks[value].getNeuronsNo()-2);
        ui->endSpinBox->setMaximum(schema->blocks[value].getNeuronsNo()-1);
        ui->endSpinBox->setValue((schema->blocks[value].getNeuronsNo()-1)/2);
    }
    else{
        this->setWindowTitle("no block");
        ui->stimulusSlider->setValue(0);
        ui->stimulusVectorSlider->setValue(0);
        ui->aSlider->setValue(0);
        ui->bSlider->setValue(0);
        ui->cSlider->setValue(0);
        ui->dSlider->setValue(0);
        ui->aVarSlider->setValue(0);
        ui->bVarSlider->setValue(0);
        ui->cVarSlider->setValue(0);
        ui->dVarSlider->setValue(0);
        ui->sigmaSlider->setValue(0);
        ui->randomizeStimulusCheckBox->setChecked(false);
        ui->startSpinBox->setEnabled(false);
        ui->endSpinBox->setEnabled(false);
    }

    if(newWindow != NULL)
        plot = newWindow->getPlot();
    lockApply = false;
    if(ui->tabWidget->currentIndex() == 3)
        refreshValues();
}


void BlockPropertiesWindow::on_randomizeStimulusCheckBox_clicked(bool checked){
    if(!checked)
        exit(1);
}

void BlockPropertiesWindow::on_stimulusSlider_valueChanged(int value){
    ui->stimulusLabel->setText("Stimulus ("+QString::number(value)+")");
    if(highlighted >= 0){
        if(vectorStimulus){
            A->setStimulus(schema->blocks[highlighted].getFirstNeuronIdx(),
                          schema->blocks[highlighted].getLastNeuronIdx(),
                          0.0f);
            A->setStimulus(schema->blocks[highlighted].getFirstNeuronIdx() +
                     (float)ui->stimulusVectorSlider->value()*
                     (float)schema->blocks[highlighted].getNeuronsSize()/360.0f,
                     schema->blocks[highlighted].getFirstNeuronIdx() +
                     ((float)ui->stimulusVectorSlider->value()+1)*
                     (float)schema->blocks[highlighted].getNeuronsSize()/360.0f,
                     (float)value);
            schema->blocks[highlighted].setStimulusAngle(value,
                                             ui->stimulusVectorSlider->value());
        }
        else if(singleNeuronStimulus){
            A->setStimulus(schema->blocks[highlighted].getFirstNeuronIdx(),
                          schema->blocks[highlighted].getLastNeuronIdx(),
                          0.0f);
            A->setStimulus(schema->blocks[highlighted].getFirstNeuronIdx() +
                           (float)ui->stimulusSingleNeuronSlider->value(),
                           (float)value);
            schema->blocks[highlighted].setSingleNeuronStimulus(value,
                                       ui->stimulusSingleNeuronSlider->value());
        }
        else if(someNeuronStimulus){
            A->setStimulus(schema->blocks[highlighted].getFirstNeuronIdx(),
                          schema->blocks[highlighted].getLastNeuronIdx(),
                          0.0f);
            A->setStimulus(schema->blocks[highlighted].getFirstNeuronIdx() +
                           ui->startSpinBox->value(),
                           schema->blocks[highlighted].getFirstNeuronIdx() +
                           ui->endSpinBox->value(), (float)value);
            // TODO: set to schema->blocks[highlighted] the first and last neuron with stimulus..
        }

        else{
            if(ui->robotStimulusButton->isChecked()){
                /*cameraWindow = new CameraWindow(schema, A, newWindow);
                cameraWindow->setHighlighted(highlighted);
                cameraWindow->show();*/
                if(robotWindow != NULL){
                    robotWindow = new RobotWindow(schema, A,
                                                  cameraWindow, newWindow);
                    //robotWindow->setHighlighted(highlighted);
                    robotWindow->show();
                }
                else{
                }
            }
            else{
            A->setStimulus(schema->blocks[highlighted].getFirstNeuronIdx(),
                          schema->blocks[highlighted].getLastNeuronIdx(),
                          (float)value);
            schema->blocks[highlighted].setStimulus(value);
            }
        }
    }
}



void BlockPropertiesWindow::on_aSlider_valueChanged(){
    if(highlighted >=0){
        if(!lockApply){
            schema->blocks[highlighted].setA(
                                           (float)ui->aSlider->value()/1000.0f);
            A->setA(schema->blocks[highlighted].getFirstNeuronIdx(),
                    schema->blocks[highlighted].getLastNeuronIdx(),
                    schema->blocks[highlighted].getA(),
                    schema->blocks[highlighted].getAVar());
        }
        ui->aLabel->setText("a: "+QString::number(
                                           schema->blocks[highlighted].getA()));
    }
    else
        ui->aLabel->setText("a: "+QString::number(
                                    (float)ui->aSlider->value()/1000.0f,'f',2));
}

void BlockPropertiesWindow::on_bSlider_valueChanged(){
    if(highlighted >=0){
        if(!lockApply){
            schema->blocks[highlighted].setB(
                                           (float)ui->bSlider->value()/1000.0f);
            A->setB(schema->blocks[highlighted].getFirstNeuronIdx(),
                    schema->blocks[highlighted].getLastNeuronIdx(),
                    schema->blocks[highlighted].getB(),
                    schema->blocks[highlighted].getBVar());
        }
        ui->bLabel->setText("b: "+QString::number(
                                           schema->blocks[highlighted].getB()));
    }
    else
        ui->bLabel->setText("b: "+QString::number(
                                    (float)ui->bSlider->value()/1000.0f,'f',2));
}


void BlockPropertiesWindow::on_cSlider_valueChanged(){
    if(highlighted >=0){
        if(!lockApply){
            schema->blocks[highlighted].setC((float)ui->cSlider->value()/10.0f);
            A->setC(schema->blocks[highlighted].getFirstNeuronIdx(),
                    schema->blocks[highlighted].getLastNeuronIdx(),
                    schema->blocks[highlighted].getC(),
                    schema->blocks[highlighted].getCVar());
        }
        ui->cLabel->setText("c: "+QString::number(
                                           schema->blocks[highlighted].getC()));
    }
    else
        ui->cLabel->setText("c: "+QString::number(
                                      (float)ui->cSlider->value()/10.0f,'f',2));
}


void BlockPropertiesWindow::on_dSlider_valueChanged(){
    if(highlighted >=0){
        if(!lockApply){
            schema->blocks[highlighted].setD((float)ui->dSlider->value()/10.0f);
            A->setD(schema->blocks[highlighted].getFirstNeuronIdx(),
                    schema->blocks[highlighted].getLastNeuronIdx(),
                    schema->blocks[highlighted].getD(),
                    schema->blocks[highlighted].getDVar());
        }
        ui->dLabel->setText("d: "+QString::number(
                                           schema->blocks[highlighted].getD()));
    }
    else
        ui->dLabel->setText("d: "+QString::number(
                                      (float)ui->dSlider->value()/10.0f,'f',2));
}

void BlockPropertiesWindow::on_aVarSlider_valueChanged(){
    if(highlighted >=0){
        if(!lockApply){
            schema->blocks[highlighted].setAVar(
                                        (float)ui->aVarSlider->value()/1000.0f);
            A->setA(schema->blocks[highlighted].getFirstNeuronIdx(),
                    schema->blocks[highlighted].getLastNeuronIdx(),
                    schema->blocks[highlighted].getA(),
                    schema->blocks[highlighted].getAVar());
        }
        ui->aVarLabel->setText("aVar: "+QString::number(
                                        schema->blocks[highlighted].getAVar()));
    }
    else
        ui->aVarLabel->setText("aVar: "+QString::number(
                                 (float)ui->aVarSlider->value()/1000.0f,'f',2));
}


void BlockPropertiesWindow::on_bVarSlider_valueChanged(){
    if(highlighted >=0){
        if(!lockApply){
            schema->blocks[highlighted].setBVar(
                                        (float)ui->bVarSlider->value()/1000.0f);
            A->setB(schema->blocks[highlighted].getFirstNeuronIdx(),
                    schema->blocks[highlighted].getLastNeuronIdx(),
                    schema->blocks[highlighted].getB(),
                    schema->blocks[highlighted].getBVar());
        }
        ui->bVarLabel->setText("bVar: "+QString::number(
                                        schema->blocks[highlighted].getBVar()));
    }
    else
        ui->bVarLabel->setText("bVar: "+QString::number(
                                 (float)ui->bVarSlider->value()/1000.0f,'f',2));
}

void BlockPropertiesWindow::on_cVarSlider_valueChanged(){
    if(highlighted >=0){
        if(!lockApply){
            schema->blocks[highlighted].setCVar(
                                          (float)ui->cVarSlider->value()/10.0f);
            A->setC(schema->blocks[highlighted].getFirstNeuronIdx(),
                    schema->blocks[highlighted].getLastNeuronIdx(),
                    schema->blocks[highlighted].getC(),
                    schema->blocks[highlighted].getCVar());
        }
        ui->cVarLabel->setText("cVar: "+QString::number(
                                        schema->blocks[highlighted].getCVar()));
    }
    else
        ui->cVarLabel->setText("cVar: "+QString::number(
                                   (float)ui->cVarSlider->value()/10.0f,'f',2));
}

void BlockPropertiesWindow::on_dVarSlider_valueChanged(){
    if(highlighted >=0){
        if(!lockApply){
            schema->blocks[highlighted].setDVar(
                                          (float)ui->dVarSlider->value()/10.0f);
            A->setD(schema->blocks[highlighted].getFirstNeuronIdx(),
                    schema->blocks[highlighted].getLastNeuronIdx(),
                    schema->blocks[highlighted].getD(),
                    schema->blocks[highlighted].getDVar());
        }
        ui->dVarLabel->setText("dVar: "+QString::number(
                                        schema->blocks[highlighted].getDVar()));
    }
    else
        ui->dVarLabel->setText("dVar: "+QString::number(
                                   (float)ui->dVarSlider->value()/10.0f,'f',2));
}


void BlockPropertiesWindow::on_sigmaSlider_valueChanged(){
    if(highlighted >=0){
        if(!lockApply){
            schema->blocks[highlighted].setSigma(
                                        (float)ui->sigmaSlider->value()/100.0f);
            A->setSigma(schema->blocks[highlighted].getFirstNeuronIdx(),
                        schema->blocks[highlighted].getLastNeuronIdx(),
                        schema->blocks[highlighted].getSigma());
        }
        ui->sigmaLabel->setText("sigma: "+QString::number(
                                       schema->blocks[highlighted].getSigma()));
    }
    else
        ui->sigmaLabel->setText("sigma: "+QString::number(
                                 (float)ui->sigmaSlider->value()/100.0f,'f',2));
}

void BlockPropertiesWindow::on_stimulusVectorSlider_valueChanged(int value){
    ui->stimulusVectorButton->setText("Neuron population vector (angle "+
                                                 QString::number(value)+"deg)");
    if(highlighted >= 0){
        if(vectorStimulus){
            A->setStimulus(schema->blocks[highlighted].getFirstNeuronIdx(),
                      schema->blocks[highlighted].getLastNeuronIdx(),
                      0.0f);
            A->setStimulus(schema->blocks[highlighted].getFirstNeuronIdx() +
                     (float)value*
                     (float)schema->blocks[highlighted].getNeuronsSize()/360.0f,
                     schema->blocks[highlighted].getFirstNeuronIdx() +
                     ((float)value+1)*
                     (float)schema->blocks[highlighted].getNeuronsSize()/360.0f,
                     (float)ui->stimulusSlider->value());
        }
        schema->blocks[highlighted].setStimulusAngle(ui->stimulusSlider->value()
                                                                       , value);
    }
}

void BlockPropertiesWindow::on_stimulusSingleNeuronSlider_valueChanged(int value){
    ui->stimulusSingleNeuronButton->setText("Single neuron(neuron number: "+
                                                    QString::number(value)+")");
    if(highlighted >= 0){
        if(singleNeuronStimulus){
            A->setStimulus(schema->blocks[highlighted].getFirstNeuronIdx(),
                      schema->blocks[highlighted].getLastNeuronIdx(),
                      0.0f);
            A->setStimulus( schema->blocks[highlighted].getFirstNeuronIdx()
                                   + value, (float)ui->stimulusSlider->value());
        }
        schema->blocks[highlighted].setSingleNeuronStimulus(
                                            ui->stimulusSlider->value(), value);
    }
}


void BlockPropertiesWindow::on_stimulusSlider_valueChanged(){
    on_stimulusSlider_valueChanged(ui->stimulusSlider->value());
}
void BlockPropertiesWindow::on_stimulusVectorSlider_valueChanged(){
    on_stimulusVectorSlider_valueChanged(ui->stimulusVectorSlider->value());
}

void BlockPropertiesWindow::on_stimulusSingleNeuronSlider_valueChanged(){
    on_stimulusSingleNeuronSlider_valueChanged(ui->stimulusSingleNeuronSlider->value());
}

void BlockPropertiesWindow::on_someStimulusButton_clicked(bool checked){
    if(checked){
        singleNeuronStimulus=false;
        vectorStimulus=false;
        someNeuronStimulus = true;
    }
    ui->startSpinBox->setEnabled(checked);
    ui->endSpinBox->setEnabled(checked);
    if(checked)
        on_stimulusSlider_valueChanged();
}


void BlockPropertiesWindow::refreshValues(){
    ui->valuesTextEdit->clear();
    j=schema->blocks[highlighted].getFirstNeuronIdx();
    QString line;
    for(i=0; i<schema->blocks[highlighted].getNeuronsSize(); i++){
        line = QString::number(i+j)+")";
        line = line.leftJustified(7, ' ')+
               QString::number(A->getRealParameterA(i+j),'f',3)+"  "+
               QString::number(A->getRealParameterB(i+j),'f',3)+"  "+
               QString::number(A->getRealParameterC(i+j),'f',3)+"  "+
               QString::number(A->getRealParameterD(i+j),'f',3)+"  ";
        ui->valuesTextEdit->append(line);
    }
}

void BlockPropertiesWindow::on_tabWidget_currentChanged(int index){
    if(index == 3){
        refreshValues();
    }
}



































