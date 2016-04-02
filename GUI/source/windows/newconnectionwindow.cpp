#include "newconnectionwindow.h"
#include "ui_newconnectionwindow.h"

NewConnectionWindow::NewConnectionWindow(ArchitectureWindow *_schema,
         const QString _source, const QString _target, QWidget *parent):
         QDialog(parent), ui(new Ui::NewConnectionWindow), source(_source),
         target(_target), schema(_schema){

    ui->setupUi(this);

    fieldsLayout = new QGridLayout(ui->fieldsGroupBox);
    fieldsLayout->setSpacing(2);
    fieldsLayout->setMargin(2);

    parsLayout = new QGridLayout(ui->parsGroupBox);
    parsLayout->setSpacing(2);
    parsLayout->setMargin(2);

    statesLayout = new QGridLayout(ui->statesGroupBox);
    statesLayout->setSpacing(2);
    statesLayout->setMargin(2);

    bool no_block = false;

    if(!schema->blocks.contains(source)){
        ui->sourceLineEdit->setText("no block");
        no_block = true;
    }
    else{
        ui->sourceLineEdit->setText(source);
        ui->sourceLineEdit->setEnabled(false);
    }

    if(!schema->blocks.contains(target)){
        ui->targetLineEdit->setText("no block");
        no_block = true;
    }
    else{
        ui->targetLineEdit->setText(target);
        ui->targetLineEdit->setEnabled(false);
    }

    if(!no_block){
        ui->fileNameLineEdit->setText(source+"_"+target+".con");
        ui->preFirstSpinBox->setValue(0);
        ui->preFirstSpinBox->setMaximum(schema->blocks[source]->getNeuronsSize()-1);
        ui->preLastSpinBox->setMaximum(schema->blocks[source]->getNeuronsSize()-1);
        ui->preLastSpinBox->setValue(schema->blocks[source]->getNeuronsSize());
        ui->postFirstSpinBox->setValue(0);
        ui->postFirstSpinBox->setMaximum(schema->blocks[target]->getNeuronsSize()-1);
        ui->postLastSpinBox->setMaximum(schema->blocks[target]->getNeuronsSize()-1);
        ui->postLastSpinBox->setValue(schema->blocks[target]->getNeuronsSize());
    }

    ui->fullRadioButton->setChecked(true);

    this->updateConnectionModels();
}

void NewConnectionWindow::updateConnectionModels(){

    ui->connectionModelComboBox->clear();

    QString source = schema->blocks[ui->sourceLineEdit->text()]->getType();
    QString target = schema->blocks[ui->targetLineEdit->text()]->getType();

    for(int i = 0; i<BackendData::edges.size(); i++){
        QString model = BackendData::edges[i];
        if(BackendData::node_model[source] == BackendData::edge_input_model[model] &&
           BackendData::node_model[target] == BackendData::edge_output_model[model]){
            ui->connectionModelComboBox->addItem(model);
        }
    }

    this->updateParameters(ui->connectionModelComboBox->currentText());
}


void NewConnectionWindow::updateParameters(QString model){

    if(!BackendData::edges.contains(model)){
        qDebug() << "NewConnectionWindow::updateParameters: ERROR: Model "
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

    ui->topographicRadioButton->setVisible(false);
    ui->pRadioButton->setVisible(false);
    ui->pSpinBox->setVisible(false);
    ui->formulaRadioButton->setVisible(false);
    ui->formulaLineEdit->setVisible(false);
    ui->fileRadioButton->setVisible(false);
    ui->fileNameLineEdit->setVisible(false);

    BackendData::my_2d_list fields = BackendData::edge_fields[model];

    // fields[i][0] <- name
    // fields[i][1] <- text showed as help
    // fields[i][2] <- type (integer, bool, float, string)
    // fields[i][3] <- Default value (also min value if there is min/max)
    // fields[i][4] <- max value (if there is min/max)

    // SOS: Not used right now..!
    bool has_connectivity_field = false;
    for(int i=0;i<fields.size();i++){
        if(fields[i][0] == "connectivity")
            has_connectivity_field = true;
    }

    for(int i=0;i<fields.size();i++){
        if(fields[i][0] != "__probability" &&
           fields[i][0] != "__formula" &&
           fields[i][0] != "__topographic" &&
           fields[i][0] != "__file" &&
           fields[i][0] != "__connectivity"){
            fieldsLabel.append(new QLabel(fields[i][0], ui->fieldsGroupBox));
            fieldsLayout->addWidget(fieldsLabel.last(),fieldsLabel.size()-1,0);
            fieldsLabel.last()->setMinimumWidth(100);
            fieldsLabel.last()->setToolTip(fields[i][1]);
        }

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
            if(fields[i][0] == "__probability"){
                ui->pRadioButton->setVisible(true);
                ui->pSpinBox->setVisible(true);
                ui->pSpinBox->setMinimum(0.0);
                ui->pSpinBox->setMaximum(1.0);
                ui->pSpinBox->setValue(fields[i][3].toDouble());
            }
            else{
                QDoubleSpinBox *temp_widget = new QDoubleSpinBox(ui->fieldsGroupBox);
                temp_widget->setValue(fields[i][3].toDouble());
                temp_widget->setMinimum(fields[i][3].toDouble());
                if(fields[i][4] != "")
                    temp_widget->setMaximum(fields[i][4].toDouble());
                else
                    temp_widget->setMaximum(9999999.0);
                fieldsWidget.append(temp_widget);
            }
        }
        else if(fields[i][2] == "string"){
            if(fields[i][0] == "__formula"){
                ui->formulaRadioButton->setVisible(true);
                ui->formulaLineEdit->setVisible(true);
                ui->formulaLineEdit->setText(fields[i][3]);
            }
            else if(fields[i][0] == "__file"){
                ui->fileRadioButton->setVisible(true);
                ui->fileNameLineEdit->setVisible(true);
                ui->fileNameLineEdit->setText(fields[i][3]);
            }
            else{
                QLineEdit *temp_widget = new QLineEdit(ui->fieldsGroupBox);
                temp_widget->setText(fields[i][3]);
                fieldsWidget.append(temp_widget);
            }
        }
        else if(fields[i][2] == "picklist" && fields[i][0] == "__connectivity"){
            // I could place all types of connectivity here!
            // But then probability for example would not show!
            ui->topographicRadioButton->setVisible(true);
        }
        else{
            qDebug() << "SOS: I cannot deal with the field "
                     << fields[i][2] << "right now.";
            QMessageBox msgBox;
            msgBox.critical(0,"Error", "SOS: I cannot deal with the field "
                            +fields[i][2] +" right now.");
            return;
        }
        if(fields[i][0] != "__probability" &&
           fields[i][0] != "__formula" &&
           fields[i][0] != "__topographic" &&
           fields[i][0] != "__file" &&
           fields[i][0] != "__connectivity"){
            fieldsLayout->addWidget(fieldsWidget.last(),fieldsWidget.size()-1,1);
        }
    }
    if(fieldsLabel.size() > 0){
        ui->fieldsGroupBox->setVisible(true);
    }
    else{
        ui->fieldsGroupBox->setVisible(false);
    }

    // -- PARAMETERS -----------------------------------------------------------
    for(int i=0;i<parsLabel.size();i++){
        parsLayout->removeWidget(parsLabel[i]);
        parsLayout->removeWidget(parsLineEdit[i]);
        delete parsLabel[i];
        delete parsLineEdit[i];
    }
    parsLabel.clear();
    parsLineEdit.clear();

    QList<QString> params = BackendData::edge_params[model];
    QList<QString> params_default = BackendData::edge_params_default[model];

    for(int i=0;i<params.size();i++){
        parsLabel.append(new QLabel(params[i],ui->parsGroupBox));
        parsLayout->addWidget(parsLabel.last(),parsLabel.size()-1,0);
        parsLabel.last()->setMinimumWidth(100);

        parsLineEdit.append(new QLineEdit(ui->parsGroupBox));
        parsLayout->addWidget(parsLineEdit.last(),parsLineEdit.size()-1,1);
        if(params_default[i] == "true" || params_default[i] == "false") // This means empty!
            parsLineEdit.last()->setText("");
        else
            parsLineEdit.last()->setText(params_default[i]);
    }
    if(parsLabel.size() > 0){
        ui->parsGroupBox->setVisible(true);
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
    QList<QString> states = BackendData::edge_states[model];
    QList<QString> states_default = BackendData::edge_states_default[model];

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
}


void NewConnectionWindow::createEdge(){
    Connection *edge = new Connection(source+"-"+target, source, target,
                                     ui->preFirstSpinBox->value(),
                                     ui->preLastSpinBox->value(),
                                     ui->postFirstSpinBox->value(),
                                     ui->postLastSpinBox->value(),
                                     ui->connectionModelComboBox->currentText());

    QString model = ui->connectionModelComboBox->currentText();

    if(BackendData::edge_fields.contains(model)){
        for(int i=0; i<fieldsLabel.size(); i++){
            // Find index of fieldsLabel[i]->text() in backend data:
            int index = -1;
            for(int j=0; j<BackendData::edge_fields[model].size(); j++){
                if(BackendData::edge_fields[model][j][0] == fieldsLabel[i]->text())
                    index = j;
            }
            if(index == -1){
                qDebug() << "NewBlockWindow::getNeurons: Error: field type "
                         << fieldsLabel[i]->text()
                         << "not found in backend data!";
                return;
            }

            qDebug() << "Adding field:"<<BackendData::edge_fields[model][index];
            if(BackendData::edge_fields[model][index][2] == "integer")
                edge->setParam(fieldsLabel[i]->text(), QString::number(
                                ((QSpinBox*)fieldsWidget[i])->value()));
            else if(BackendData::edge_fields[model][index][2] == "float")
                edge->setParam(fieldsLabel[i]->text(), QString::number(
                                ((QDoubleSpinBox*)fieldsWidget[i])->value()));
            else if(BackendData::edge_fields[model][index][2] == "bool")
                edge->setParam(fieldsLabel[i]->text(),((QCheckBox*)
                                fieldsWidget[i])->isChecked() ?"True" : "False");
            if(BackendData::edge_fields[model][index][2] == "string")
                edge->setParam(fieldsLabel[i]->text(),
                                ((QLineEdit*)fieldsWidget[i])->text());
            else{
                qDebug() << "NewBlockWindow::getNeurons: Error: field type "
                         << BackendData::edge_fields[model][index][2]
                         << "not contained!";
            }
        }
    }
    else if(fieldsLabel.size() > 0){
        qDebug() << "NewConnectionWindow::createEdge: Error: model" << model
                 << "not contained in backend data!";
        exit(1);
    }

    for(int i=0; i<parsLineEdit.size(); i++){
        edge->setParam(parsLabel[i]->text(), parsLineEdit[i]->text());
    }

    for(int i=0; i<statesLineEdit.size(); i++){
        edge->setState(statesLabel[i]->text(), statesLineEdit[i]->text());
    }

    if(ui->fullRadioButton->isChecked()){
        edge->setParam("__connectivity", "full");
    }
    else if(ui->topographicRadioButton->isChecked()){
        edge->setParam("__connectivity", "topographic");
    }
    else if(ui->pRadioButton->isChecked()){
        edge->setParam("__connectivity", "probability");
    }
    else if(ui->formulaRadioButton->isChecked()){
        edge->setParam("__connectivity", "formula");
    }
    else if(ui->fileRadioButton->isChecked()){
        edge->setParam("__connectivity", "file");
    }

    if(ui->pSpinBox->isVisible() && ui->pSpinBox->isEnabled()){
        edge->setParam("__probability", QString::number(ui->pSpinBox->value()));
    }
    if(ui->formulaLineEdit->isVisible() && ui->formulaLineEdit->isEnabled()){
        edge->setParam("__formula", ui->formulaLineEdit->text());
    }
    if(ui->fileNameLineEdit->isVisible() && ui->fileNameLineEdit->isEnabled()){
        edge->setParam("__file", ui->fileNameLineEdit->text());
    }

    schema->endConnection(*edge);
    schema->setModified();
}



void NewConnectionWindow::on_addButton_clicked(){
    this->createEdge();
    this->close();
}


void NewConnectionWindow::on_fullRadioButton_clicked(){
    ui->pSpinBox->setEnabled(false);
    ui->formulaLineEdit->setEnabled(false);
    ui->fileNameLineEdit->setEnabled(false);
}

void NewConnectionWindow::on_topographicRadioButton_clicked(){
    ui->pSpinBox->setEnabled(false);
    ui->formulaLineEdit->setEnabled(false);
    ui->fileNameLineEdit->setEnabled(false);
}

void NewConnectionWindow::on_pRadioButton_clicked(){
    ui->pSpinBox->setEnabled(true);
    ui->formulaLineEdit->setEnabled(false);
    ui->fileNameLineEdit->setEnabled(false);
}

void NewConnectionWindow::on_formulaRadioButton_clicked(){
    ui->pSpinBox->setEnabled(true);
    ui->formulaLineEdit->setEnabled(true);
    ui->fileNameLineEdit->setEnabled(false);
}

void NewConnectionWindow::on_fileRadioButton_clicked(){
    ui->pSpinBox->setEnabled(false);
    ui->formulaLineEdit->setEnabled(false);
    ui->fileNameLineEdit->setEnabled(true);
}

void NewConnectionWindow::on_connectionModelComboBox_currentTextChanged(const QString &arg1){
    this->updateParameters(arg1);
}

void NewConnectionWindow::on_preLastSpinBox_valueChanged(int arg1){
    ui->preFirstSpinBox->setMaximum(arg1);
}

void NewConnectionWindow::on_postLastSpinBox_valueChanged(int arg1){
    ui->postFirstSpinBox->setMaximum(arg1);
}

































