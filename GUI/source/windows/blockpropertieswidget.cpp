#include "blockpropertieswidget.h"
#include "ui_blockpropertieswidget.h"

BlockPropertiesWidget::BlockPropertiesWidget(Block *_block,
                                             Simulator *_A,
                                             QWidget *parent):
                                             QWidget(parent),
                                             ui(new Ui::BlockPropertiesWidget){
    block = _block;
    A = _A;
    ui->setupUi(this);

    this->setWindowTitle(block->getId()+"'s properties");

    this->move(0,0);
    //this->setMinimumSize(350, 500);

    paramsLayout = new QGridLayout(ui->parsGroupBox);
    paramsLayout->setSpacing(2);
    paramsLayout->setMargin(2);

    statesLayout = new QGridLayout(ui->statesGroupBox);
    statesLayout->setSpacing(2);
    statesLayout->setMargin(2);

    fieldsLayout = new QGridLayout(ui->fieldsGroupBox);
    fieldsLayout->setSpacing(2);
    fieldsLayout->setMargin(2);

    // Update type combo box
    ui->typeComboBox->clear();
    int current_index = -1, temp_idx=0;
    QMap<QString, BackendData::my_list>::iterator it;
    for(it = BackendData::node_params.begin();
        it != BackendData::node_params.end(); it++){
        ui->typeComboBox->addItem(it.key());
        if(it.key() == block->getType())
            current_index = temp_idx;
        temp_idx++;
    }
    if(current_index == -1){
        qDebug() << "BlockPropertiesWidget::BlockPropertiesWidget: "
                    "Problem with the type: not found in BackendData!";
        ui->typeComboBox->setCurrentText(block->getType());
    }
    else{
        ui->typeComboBox->setCurrentIndex(current_index);
    }

    connect(ui->typeComboBox, SIGNAL(currentIndexChanged(int)),
            this,             SLOT(parametersChanged()));

    this->changeParameters();

    ui->fieldsGroupBox->setVisible(false);
    ui->statesGroupBox->setVisible(false);
}

BlockPropertiesWidget::~BlockPropertiesWidget(){
    delete ui;
}



// ZAF SOS: Normally here I should take the pars from BackendData and check
//          which ones I already have, and add the rest/ delete the prev.
void BlockPropertiesWidget::changeParameters(){
    // Clear previous form
    for(int i=0;i<parsLabel.size();i++){
        paramsLayout->removeWidget(parsLabel[i]);
        paramsLayout->removeWidget(parsLineEdit[i]);
        disconnect(parsLineEdit[i], SIGNAL(editingFinished()),
                   this,            SLOT(parametersChanged()));
        delete parsLabel[i];
        delete parsLineEdit[i];
    }
    parsLabel.clear();
    parsLineEdit.clear();

    // Add the new ones
    QMap<QString,QString> params = block->getParams();
    QMap<QString, QString>::iterator i;
    for(i=params.begin(); i != params.end(); i++){
        parsLabel.append(new QLabel(i.key(),ui->parsGroupBox));
        paramsLayout->addWidget(parsLabel.last(),parsLabel.size()-1,0);
        parsLabel.last()->setMinimumWidth(100);

        parsLineEdit.append(new QLineEdit(ui->parsGroupBox));
        paramsLayout->addWidget(parsLineEdit.last(),parsLineEdit.size()-1,1);
        parsLineEdit.last()->setText(i.value());
        connect(parsLineEdit.last(), SIGNAL(editingFinished()),
                this,                SLOT(parametersChanged()));
    }


    int target_height = parsLabel.size()*30 + 60;
    if(target_height > 600) target_height = 600;
    if(target_height < 500) target_height = 500;
    this->setMinimumHeight(target_height);
    this->setMaximumHeight(target_height);
}



void BlockPropertiesWidget::changeToDefault(QString model){

    if(!BackendData::nodes.contains(model)){
        qDebug() << "NewBlockWindow::changeParameters: ERROR: Model "
                    "not in backend data!";
        return;
    }

    // -- FIELDS ---------------------------------------------------------------
    for(int i=0;i<fieldsLabel.size();i++){
        fieldsLayout->removeWidget(fieldsLabel[i]);
        fieldsLayout->removeWidget(fieldsWidget[i]);
        delete fieldsLabel[i];
        delete fieldsWidget[i];
    }
    fieldsLabel.clear();
    fieldsWidget.clear();

    BackendData::my_2d_list fields = BackendData::node_fields[model];

    // fields[i][0] <- name
    // fields[i][1] <- text showed as help
    // fields[i][2] <- type (integer, bool, float, string)
    // fields[i][3] <- Default value (also min value if there is min/max)
    // fields[i][4] <- max value (if there is min/max)

    for(int i=0;i<fields.size();i++){
        fieldsLabel.append(new QLabel(fields[i][0], ui->fieldsGroupBox));
        fieldsLayout->addWidget(fieldsLabel.last(),fieldsLabel.size()-1,0);
        fieldsLabel.last()->setMinimumWidth(100);
        fieldsLabel.last()->setToolTip(fields[i][1]);

        if(fields[i][2] == "integer"){
            QSpinBox *temp_widget = new QSpinBox(ui->fieldsGroupBox);
            temp_widget->setValue(fields[i][3].toInt());
            temp_widget->setMinimum(fields[i][3].toInt());
            if(fields[i][4] != "")
                temp_widget->setMaximum(fields[i][4].toInt());
            else
                temp_widget->setMaximum(9999999);
            fieldsWidget.append(temp_widget);
        }
        else if(fields[i][2] == "bool"){
            QCheckBox *temp_widget = new QCheckBox(ui->fieldsGroupBox);
            if(fields[i][3] == "False")
                temp_widget->setChecked(false);
            else if(fields[i][3] == "True")
                temp_widget->setChecked(true);
            else
                qDebug() << "SOS: I cannot parse this boolean field:"
                         << fields[i][3];
            fieldsWidget.append(temp_widget);
        }
        else if(fields[i][2] == "float"){
            QDoubleSpinBox *temp_widget = new QDoubleSpinBox(ui->fieldsGroupBox);
            temp_widget->setValue(fields[i][3].toDouble());
            temp_widget->setMinimum(fields[i][3].toDouble());
            if(fields[i][4] != "")
                temp_widget->setMaximum(fields[i][4].toDouble());
            else
                temp_widget->setMaximum(9999999.0);
            fieldsWidget.append(temp_widget);
        }
        else if(fields[i][2] == "string"){
            QLineEdit *temp_widget = new QLineEdit(ui->fieldsGroupBox);
            temp_widget->setText(fields[i][3]);
            fieldsWidget.append(temp_widget);
        }
        else{
            qDebug() << "SOS: I cannot deal with this field now.";
            QMessageBox msgBox;
            msgBox.critical(0,"Error", "SOS: I cannot deal with this field now.");
            return;
        }
        fieldsLayout->addWidget(fieldsWidget.last(),fieldsWidget.size()-1,1);
    }
    if(fieldsLabel.size() > 0){
        ui->fieldsGroupBox->setVisible(true);
    }
    else{
        ui->fieldsGroupBox->setVisible(false);
    }

    // -- PARAMETERS -----------------------------------------------------------
    for(int i=0;i<parsLabel.size();i++){
        paramsLayout->removeWidget(parsLabel[i]);
        paramsLayout->removeWidget(parsLineEdit[i]);
        delete parsLabel[i];
        delete parsLineEdit[i];
    }
    parsLabel.clear();
    parsLineEdit.clear();

    QList<QString> params = BackendData::node_params[model];
    QMap<QString, QList<QString> > params_default = BackendData::node_params_default[model];

    for (int i = 0; i < params.size(); i++){
        parsLabel.append(new QLabel(params[i], ui->parsGroupBox));
        paramsLayout->addWidget(parsLabel.last(),parsLabel.size()-1,0);
        parsLabel.last()->setMinimumWidth(100);

        parsLineEdit.append(new QLineEdit(ui->parsGroupBox));
        paramsLayout->addWidget(parsLineEdit.last(),parsLineEdit.size()-1,1);
        parsLineEdit[i]->setText(block->getParam(params[i]));
    }

    if(parsLabel.size() > 0){
        ui->parsGroupBox->setVisible(true);
        ui->parsGroupBox->setWindowTitle("Unit parameter values");
    }
    else{
        ui->parsGroupBox->setVisible(false);
    }

    // -- STATES ---------------------------------------------------------------
    for(int i=0;i<statesLabel.size();i++){
        statesLayout->removeWidget(statesLabel[i]);
        statesLayout->removeWidget(statesLineEdit[i]);
        delete statesLabel[i];
        delete statesLineEdit[i];
    }
    statesLabel.clear();
    statesLineEdit.clear();
    QList<QString> states = BackendData::node_states[model];
    QList<QString> states_default = BackendData::node_states_default[model];

    for(int i=0;i<states.size();i++){
        statesLabel.append(new QLabel(states[i],ui->statesGroupBox));
        statesLayout->addWidget(statesLabel.last(),statesLabel.size()-1,0);
        statesLabel.last()->setMinimumWidth(100);

        statesLineEdit.append(new QLineEdit(ui->statesGroupBox));
        statesLayout->addWidget(statesLineEdit.last(),statesLineEdit.size()-1,1);
        if(states_default[i] == "true" || states_default[i] == "false") // This means empty!
            statesLineEdit.last()->setText("");
        else
            statesLineEdit.last()->setText(states_default[i]);
    }
    if(statesLabel.size() > 0){
        ui->statesGroupBox->setVisible(true);
    }
    else{
        ui->statesGroupBox->setVisible(false);
    }

    int target_height = parsLabel.size()*30 + 60;
    if(target_height > 600) target_height = 600;
    if(target_height < 500) target_height = 500;
    this->setMinimumHeight(target_height);
    this->setMaximumHeight(target_height);
}




// -------------------------------------------------------------------------- //
//                            GUI methods                                     //
// -------------------------------------------------------------------------- //


void BlockPropertiesWidget::refreshValues(){
    /*ui->valuesTextEdit->clear();
    if(highlighted != "" && A != NULL){
        j=block->getFirstNeuronIdx();
        QString line;
        if(block->getNeuronModelStr() == "simple" ||
           block->getNeuronModelStr() == "simple_bg"){
            for(i=0; i<block->getNeuronsSize(); i++){
                line = QString::number(i+j)+")";
                line = line.leftJustified(7, ' ')+
                       QString::number(A->getRealParameterA(i+j),'f',3)+"  "+
                       QString::number(A->getRealParameterB(i+j),'f',3)+"  "+
                       QString::number(A->getRealParameterC(i+j),'f',3)+"  "+
                        QString::number(A->getRealParameterD(i+j),'f',3)+" D:"+
                        QString::number(A->getRealParameterD1(i+j),'f',3)+"/"+
                        QString::number(A->getRealParameterD2(i+j),'f',3)+" ("+
                       QString::number(A->getRealNeuronStateU(i+j),'f',3)+", "+
                       QString::number(A->getRealNeuronStateV(i+j),'f',3)+")";
                ui->valuesTextEdit->append(line);
            }
        }
        else{
            for(i=0; i<block->getNeuronsSize(); i++){
                line = QString::number(i+j)+")<b>a,b,c,d/vpe,vr,vt,k,C:</b>";
                ui->valuesTextEdit->append(line);
                line = QString::number(A->getRealParameter(i+j, 0),'f',2)+", "+
                       QString::number(A->getRealParameter(i+j, 1),'f',2)+", "+
                       QString::number(A->getRealParameter(i+j, 2),'f',2)+", "+
                       QString::number(A->getRealParameter(i+j, 3),'f',2);
                ui->valuesTextEdit->append(line);
                line = QString::number(A->getRealParameter(i+j, 4),'f',2)+", "+
                       QString::number(A->getRealParameter(i+j, 5),'f',2)+", "+
                       QString::number(A->getRealParameter(i+j, 6),'f',2)+", "+
                       QString::number(A->getRealParameter(i+j, 7),'f',2)+", "+
                       QString::number(A->getRealParameter(i+j, 8),'f',1);
                ui->valuesTextEdit->append(line);
                line = "<b>sig,D1,D2:</b> " +
                       QString::number(A->getRealParameter(i+j, 9),'f',2)+", "+
                       QString::number(A->getRealParameter(i+j, 10),'f',2)+", "+
                       QString::number(A->getRealParameter(i+j, 11),'f',2);
                ui->valuesTextEdit->append(line);
                line = "<b>U,V:</b> " +
                        QString::number(A->getRealNeuronStateU(i+j),'f',2)+", "+
                        QString::number(A->getRealNeuronStateV(i+j),'f',2)+"";
                ui->valuesTextEdit->append(line);
            }
        }
    }*/
}

void BlockPropertiesWidget::parametersChanged(){
    QString model = ui->typeComboBox->currentText();

    if(block->getType() != model){
        block->clearParams();
        this->changeToDefault(model);
        block->setType(model);
    }

    // Add fields
    for(int i=0; i<fieldsLabel.size(); i++){
        if(BackendData::node_fields[model][i][2] == "integer")
            block->setParam(fieldsLabel[i]->text(), QString::number(
                            ((QSpinBox*)fieldsWidget[i])->value()));
        else if(BackendData::node_fields[model][i][2] == "float")
            block->setParam(fieldsLabel[i]->text(), QString::number(
                            ((QDoubleSpinBox*)fieldsWidget[i])->value()));
        else if(BackendData::node_fields[model][i][2] == "bool")
            block->setParam(fieldsLabel[i]->text(),((QCheckBox*)
                            fieldsWidget[i])->isChecked() ?"True" : "False");
        if(BackendData::node_fields[model][i][2] == "string")
            block->setParam(fieldsLabel[i]->text(),
                            ((QLineEdit*)fieldsWidget[i])->text());
        else{
            qDebug() << "BlockPropertiesWidget::getNeurons:"
                     << "Error: field type not contained!";
        }
    }

    // Add parameters
    for(int i=0; i<parsLineEdit.size(); i++){
        block->setParam(parsLabel[i]->text(), parsLineEdit[i]->text());
    }

    // Add states
    for(int i=0; i<statesLineEdit.size(); i++){
        block->setState(statesLabel[i]->text(), statesLineEdit[i]->text());
    }

    block->update();
}
