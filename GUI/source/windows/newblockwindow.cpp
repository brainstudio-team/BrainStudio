#include "newblockwindow.h"
#include "ui_newblockwindow.h"

NewBlockWindow::NewBlockWindow(ArchitectureWindow *_schema, QPoint _pos,
                               QWidget *parent):QDialog(parent),
                               ui(new Ui::NewBlockWindow), pos(_pos){
    schema = _schema;
    ui->setupUi(this);

    maxX = 100.0f;

    QString col = "#";
    for(int i=0;i<6;i++){
        col += QString::number(2 + rand()%8);
    }
    ui->colourLineEdit->setText(col);

    parsLayout = new QGridLayout(ui->parsGroupBox);
    parsLayout->setSpacing(2);
    parsLayout->setMargin(2);

    statesLayout = new QGridLayout(ui->statesGroupBox);
    statesLayout->setSpacing(2);
    statesLayout->setMargin(2);

    fieldsLayout = new QGridLayout(ui->fieldsGroupBox);
    fieldsLayout->setSpacing(2);
    fieldsLayout->setMargin(2);

    updateFunctionValues();

    QString start = "";
    int i=0;
    while(schema->blocks.contains("New block"+start)){
        start = " " + QString::number(i++);
    }
    ui->nameLineEdit->setText("New block"+start);
    this->setWindowTitle("Add a new block");
}

void NewBlockWindow::updateFunctionValues(){

    ui->neuronModelComboBox->clear();

    for(int i = 0; i<BackendData::nodes.size(); i++){
        ui->neuronModelComboBox->addItem(BackendData::nodes[i]);
    }


    this->changeParameters(ui->neuronModelComboBox->currentText());
}

NewBlockWindow::~NewBlockWindow(){
    delete ui;
}

void NewBlockWindow::paintEvent(QPaintEvent * event){
    (void)event;
    QPainter painter(this);
    QColor c1;

    int size_x = ui->widthSlider->value();
    int size_y = ui->heightSlider->value();

    if(size_x > ui->lblBlock->maximumWidth())
        size_x = ui->lblBlock->maximumWidth();
    if(size_y > ui->lblBlock->maximumHeight())
        size_y = ui->lblBlock->maximumHeight();

    if(size_x < ui->lblBlock->minimumWidth())
        size_x = ui->lblBlock->minimumWidth();
    if(size_y < ui->lblBlock->minimumHeight())
        size_y = ui->lblBlock->minimumHeight();

    QPixmap pixmap(size_x+3, size_y+3);
    pixmap.fill(QColor("transparent"));

    QPainter painter2(&pixmap);


    if(ui->colourLineEdit->text().length() == 7)
        c1.setNamedColor(ui->colourLineEdit->text());
    else
        c1.setNamedColor("#ffffff");
    painter2.setBrush(QBrush(c1));
    painter2.drawRect(0, 0,size_x, size_y);
    painter2.drawText(3, 30, "("+QString::number(pos.x()) +", "
                      + QString::number(pos.y()) + ")");
    painter2.drawText(3, 15, "("+QString::number(ui->widthSlider->value())+", "+
                      QString::number(ui->heightSlider->value()) + ")");

    ui->lblBlock->resize( size_x + 3, size_y + 3);
    ui->lblBlock->setPixmap(pixmap);


    if(ui->spatiallyEmbeddedCheckBox->isChecked()){
        int i,j;
        for(i=0; i<ui->spinBoxNeuronsX->value(); i++){
            for(j=0; j<ui->spinBoxNeuronsY->value(); j++){
                painter.drawEllipse(QPoint(SIZEX+10+i*10,550-j*10),3,3);
            }
        }
    }

}

void NewBlockWindow::changeParameters(QString model){

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
    for(int i = 0; i < parsLabel.size(); i++){
        parsLayout->removeWidget(parsLabel[i]);
        parsLayout->removeWidget(parsLineEdit[i]);
        delete parsLabel[i];
        delete parsLineEdit[i];
    }
    parsLabel.clear();
    parsLineEdit.clear();

    QList<QString> params = BackendData::node_params[model];
    QMap<QString, QList<QString> > params_default = BackendData::node_params_default[model];

    for(int i = 0; i < params.size(); i++){
        parsLabel.append(new QLabel(params[i],ui->parsGroupBox));
        parsLayout->addWidget(parsLabel.last(),parsLabel.size()-1,0);
        parsLabel.last()->setMinimumWidth(100);

        parsLineEdit.append(new QLineEdit(ui->parsGroupBox));
        parsLayout->addWidget(parsLineEdit.last(),parsLineEdit.size()-1,1);

        // By default, start with the first parameter set (which will be the
        // same that is shown in the parameterSetComboBox
        QString paramSetName = params_default.keys()[0];
        if(params_default[paramSetName][i] == "true" || params_default[paramSetName][i] == "false") // This means empty!
            parsLineEdit.last()->setText("");
        else
            parsLineEdit.last()->setText(params_default[paramSetName][i]);
    }
    if(parsLabel.size() > 0){
        ui->parsGroupBox->setVisible(true);
    }
    else{
        ui->parsGroupBox->setVisible(false);
    }


    // -- STATES ---------------------------------------------------------------
    for(int i = 0; i < statesLabel.size(); i++){
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
}

void NewBlockWindow::on_parameterSetComboBox_currentTextChanged(const QString &paramSetName) {

    if (!paramSetName.isEmpty()) {
        QString model = ui->neuronModelComboBox->currentText();
        QList<QString> params = BackendData::node_params[model];
        QMap<QString, QList<QString> > params_default = BackendData::node_params_default[model];

        for (int i = 0; i < params.size(); i++) {
            if(params_default[paramSetName][i] == "true" || params_default[paramSetName][i] == "false") // This means empty!
                parsLineEdit[i]->setText("");
            else
                parsLineEdit[i]->setText(params_default[paramSetName][i]);
        }
    }
}

int NewBlockWindow::getNeurons(){
    QString model = ui->neuronModelComboBox->currentText();
    if(!BackendData::node_units_field.contains(model)){
        qDebug() << "NewBlockWindow::getNeurons: Error: model" << model
                 << "not contained!";
        return 0;
    }
    for(int i=0; i<fieldsLabel.size(); i++){
        if(fieldsLabel[i]->text() == BackendData::node_units_field[model]){
            if(BackendData::node_fields[model][i][2] == "integer")
                return ((QSpinBox*)fieldsWidget[i])->value();
            else if(BackendData::node_fields[model][i][2] == "float")
                return (int)((QDoubleSpinBox*)fieldsWidget[i])->value();
            else if(BackendData::node_fields[model][i][2] == "bool")
                return (int)((QCheckBox*)fieldsWidget[i])->isChecked();
            else{
                qDebug() << "NewBlockWindow::getNeurons: Error: field type not contained!";
                return 0;
            }
        }
    }
    qDebug() << "NewBlockWindow::getNeurons: Error: field not contained!";
    return 0;
}


void NewBlockWindow::on_neuronModelComboBox_currentTextChanged(const QString &model){

    if (!model.isEmpty()) {
        QMap<QString, QList<QString> > params_default = BackendData::node_params_default[model];

        this->changeParameters(model);

        // Once we know which model has been selected, we can clear the parameter
        // combo box and populate it with the default values for this model.
        ui->parameterSetComboBox->clear();
        ui->parameterSetComboBox->insertItems(0, params_default.keys());

    }

}


void NewBlockWindow::on_addButton_clicked(){
    bool locations = false;

    if(ui->spatiallyEmbeddedCheckBox->isChecked())
        locations = true;

    this->createBlock(locations);

    this->close();
}

void NewBlockWindow::on_neuronModelComboBox_currentIndexChanged(int index) {
   /* Here I need to load it from the backend!
    ui->parameterSetComboBox->clear();
    switch(index){
    case SIMPLE:
        ui->parameterSetComboBox->addItem("Excitatory neurons");
        ui->parameterSetComboBox->addItem("Inhibitory neurons");
        ui->parameterSetComboBox->addItem("Custom neurons");
        break;
    }*/
}

void NewBlockWindow::setCustom(int category){
    (void)category;
    // .... here too
}

void NewBlockWindow::on_spatiallyEmbeddedCheckBox_clicked(bool checked){
    if(checked){
        ui->lblNeuronsX->setEnabled(true);
        ui->lblNeuronsY->setEnabled(true);
        ui->spinBoxNeuronsX->setEnabled(true);
        ui->spinBoxNeuronsY->setEnabled(true);
        this->resize(SIZEX+20, SIZEY);

    }
    else{
        ui->lblNeuronsX->setEnabled(false);
        ui->lblNeuronsY->setEnabled(false);
        ui->spinBoxNeuronsX->setEnabled(false);
        ui->spinBoxNeuronsY->setEnabled(false);
        this->resize(SIZEX, SIZEY);
    }
}

void NewBlockWindow::on_spinBoxNeuronsX_valueChanged(int arg1){
    (void)arg1;
    this->resize(SIZEX+20+10*ui->spinBoxNeuronsX->value(),SIZEY);
    this->update();
}


void NewBlockWindow::on_spinBoxNeuronsY_valueChanged(int arg1){
    (void)arg1;
    this->update();
}



void NewBlockWindow::on_neuronsSpinBox_valueChanged(int arg1){
    if(ui->spinBoxNeuronsX->value() * ui->spinBoxNeuronsY->value() < arg1){
        ui->spinBoxNeuronsX->setValue(1);
        ui->spinBoxNeuronsY->setValue(arg1);
    }
}


void NewBlockWindow::on_exitButton_clicked(){
    this->close();
}


void NewBlockWindow::createBlock(bool locations){
    int i, j;

    // -- Create locations, add them in the vector 'neuronLoc' and save them ---
    if(locations){
        QFile fileLoc(/*FOLDERNAME + "/" + */ui->nameLineEdit->text() + ".loc");
        if (!fileLoc.open(QIODevice::WriteOnly)) {
            std::cerr << "Cannot open fileLoc for writing: "
                 << qPrintable(fileLoc.errorString()) << std::endl;
            return;
        }
        QTextStream outLoc(&fileLoc);

        for(i=0; i<ui->spinBoxNeuronsX->value(); i++){
            for(j=0; j<ui->spinBoxNeuronsY->value(); j++){
                neuronLoc.append(NeuronUnit((float)i,(float)j, 0.0f));
                outLoc << QString::number((float)i, 'f', 7) << " "
                       << QString::number((float)j, 'f', 7) << " "
                       << QString::number(0.0f, 'f', 7) << "\n";
            }
        }
        fileLoc.close();
    }

    // -- Add block (including locations) to schema ----------------------------
    bool spiking = false;
    if(!BackendData::node_model.contains(ui->neuronModelComboBox->currentText())){
        qDebug() << "NewBlockWindow::createBlock: Error: node model not recognized!";
        return;
    }
    if(BackendData::node_model[ui->neuronModelComboBox->currentText()] == "spike")
        spiking = true;
    else if(BackendData::node_model[ui->neuronModelComboBox->currentText()] == "rate")
        spiking = false;
    else{
        qDebug() << "NewBlockWindow::createBlock: Error: node model not recognized!";
        return;
    }

    Block *block = new Block(ui->nameLineEdit->text(),
                             ui->neuronModelComboBox->currentText(),
                             spiking,
                             pos.x()-(ui->widthSlider->value()/2) - 5,
                             pos.y()-(ui->heightSlider->value()/2) - 5,
                             ui->widthSlider->value() + 10,
                             ui->heightSlider->value() + 10,
                             ui->colourLineEdit->text(),
                             "Default",
                             this->getNeurons(),
                             this->schema);

    QString model = ui->neuronModelComboBox->currentText();
    if(!BackendData::node_units_field.contains(model)){
        qDebug() << "NewBlockWindow::getNeurons: Error: model" << model
                 << "not contained!";
        exit(1);
    }

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
            qDebug() << "NewBlockWindow::getNeurons:"
                     << "Error: field type not contained!";
        }
    }

    for(int i=0; i<parsLineEdit.size(); i++){
        block->setParam(parsLabel[i]->text(), parsLineEdit[i]->text());
    }

    for(int i=0; i<statesLineEdit.size(); i++){
        block->setState(statesLabel[i]->text(), statesLineEdit[i]->text());
    }

    block->setParam("SpatiallyEmbedded",ui->spatiallyEmbeddedCheckBox->isChecked() ? "1" : "0");

    schema->addBlock(block);


    schema->setModified();
}

void NewBlockWindow::on_nameLineEdit_textChanged(const QString &arg1){
    if(schema->blocks.contains(arg1)){
        ui->warningLabel->setText("<b>Warning:</b><br/>"
                                  "This name is used by another block.<br/>"
                                  "Please change it before you finish.");
        ui->addButton->setEnabled(false);
    }
    else{
        ui->warningLabel->setText("");//ui->warningLabel->setText("Ready to add.");
        ui->addButton->setEnabled(true);
    }
}

































