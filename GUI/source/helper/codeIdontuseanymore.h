#ifndef CODEIDONTUSEANYMORE_H
#define CODEIDONTUSEANYMORE_H

/*void MainWindow::setSimulationParametersFromTab(const int &index){
    return;
    if(index >= MAX_FILES)
        return;
    // Speed
    //if(workTab[index]->getSpeed() > 0 && runThread != NULL){
    //    speedSlider->setValue(workTab[index]->getSpeed());
    //}

    // TimeStep
    dt = workTab[index]->getTimeStep();

    // Simulator
    if(workTab[index]->getSimulator() == "nemo"){
        actionCustom_simulator->setChecked(false);
        actionNemo->setChecked(true);
    }
    else if(workTab[index]->getSimulator() == "custom"){
        actionCustom_simulator->setChecked(true);
        actionNemo->setChecked(false);
    }

    int sim;
    if(workTab[index]->getSimulationMethod() == "EULER")
        sim = EULER;
    else if(workTab[index]->getSimulationMethod() == "RK4")
        sim = RUNGE_KUTTA_4TH;
    else sim = EULER;
    //simMethodComboBox->setCurrentIndex(sim);
    //A->setSimulationMode(sim);
}*/

/*bool MainWindow::restartEverything(){

    // CREATE NEW BLOCK_PROPERTIES
    if(blockProperties != NULL) delete blockProperties;
    blockProperties = new BlockPropertiesWidget(schemaPtr(), A, NULL, propertiesButtomFrame);
    propertiesFrame->setMinimumSize(propertiesFrame->width(), blockProperties->height() + propertiesLabel->height() + 10);
    blockProperties->show();

    // CREATE NEW MAIN_CONTROLS
    if(controls != NULL) delete controls;
    controls = new ControlsWidget(schemaPtr(), A, controlsButtomFrame);

    // CREATE NEW PROGRESS_DIALOG
    if(progressDialog != NULL) delete progressDialog;
    progressDialog = new ProgressDialog(this);

    // DELETE EVERYTHING ELSE
    if(addWindow != NULL){     delete addWindow; addWindow = NULL; }
    if(addConnection != NULL){ delete addConnection; addConnection = NULL; }
    if(learningWindow != NULL){delete learningWindow; learningWindow=NULL; }
    if(cameraWindow != NULL){  delete cameraWindow; cameraWindow = NULL; }
    if(robotWindow != NULL){   delete robotWindow; robotWindow = NULL; }
    if(focusedPlot != NULL){   delete focusedPlot; focusedPlot = NULL; }

    setSimulationParametersFromTab(currentTabIndex);

    // RESTART SIMULATOR
    this->deleteNeuronsFromSchema(true);
    if(A == NULL) return false;
    if(!A->restartNetwork()){
        std::cout <<"Restarting Network failed: I'm deleting the simulator..\n";

        //if(actionNemo->isChecked())
        //    changeSimulation(NEMOSIM);
        //else if(actionCustom_simulator->isChecked())
        //    changeSimulation(CUSTOMSIM);
    }

    // RESET SIMULATION MODE
    //A->setSimulationMode(simMethodComboBox->currentIndex());

    // RESET GUI STUFF
    playButton->setIcon(QIcon(":/new/prefix1/icons/Pause-Disabled-icon48.png"));
    //modeBRadioButton->click();
    updateGeneralPropertiesTab();
    this->resizeWidget(NULL);

    // RECONNECT SIGNALS
    connectSignals(tabWidget->currentIndex());
    return true;
}*/





/*void MainWindow::paintEvent(QPaintEvent * event){
    QPainter painter(this);
    return;

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));

    // STATS
    if(controls->updateDetails() && !noTabYet()){
        QString str;
        infoLabel1->setText(
         (run ? "  run":str) + " " + (A->isInitialized()?" ready":str)
         + " Neurons: "  + QString::number(A->getNeurons())
         + " Synapses: " + QString::number(A->getSynapses())
         + " Spikes: "   + QString::number(A->getSpikes())
         + " dt: "       + QString::number(A->getDt())
         + (actionNemo->isChecked()?" (Nemo simulator)":" (Custom simulator)"));
        infoLabel2->setText(
           " neuron[0]: Input: " + QString::number(A->getNeuronInput(0))
         + " State: " + QString::number(A->getNeuronState(0))
         + " Firing rate: "
         + workTab[tabWidget->currentIndex()]->getFRneuronStr(0,0)
         + " stimWeight: " + QString::number(A->getStimWeight()));
    }
    else{
        infoLabel1->setText("Empty file..");
    }*/

    /*if(training || scriptTraining){
        if(A->getTimeStep() == -1.0)
            this->setWindowTitle( "TRAINING FINISHED! (" +
                                  QString::number(A->getTimeStep(),'f',3) +
                                  "miliseconds)");
        else
            this->setWindowTitle( "Training... ("+
                                  QString::number(A->getTimeStep(),'f',3) +
                                  "ms)");
    }
    else
        this->setWindowTitle("BrainStd: " + FILENAME);
}*/




/*void MainWindow::schemaMouseRelease(){
    if(!A->isInitialized()){
        saveButton->setEnabled(true);
    }
}*/


/*void MainWindow::deleteNeuronsFromSchema(bool newSchema){
    actionCreate_Network->setEnabled(true);
    CreateNetworkButton->setEnabled(true);

    if(!newSchema){
        schemaPtr()->setInitialized(false);
        if(controls->updateSchema()){
           for(int b=0; b<schemaPtr()->blocks.size(); b++){
                schemaPtr()->blocks[b]->setFirstNeuronIdx(0);
                schemaPtr()->blocks[b]->setLastNeuronIdx(0);
                schemaPtr()->blocks[b]->deleteNeurons();
                schemaPtr()->blocks[b]->deleteConnections();
           }
        }
    }

    //updateSchemaCheckBox->setCheckable(true);
    playButton->setEnabled(false);
    //schemaPtr()->update();
}*/


/*
 *  if(saveButton->isEnabled()){
 *  save();
 *  // if STDP was on, create files and save the custom connections
 *  if(A->isInitialized() && A->learningInitialized()){
 *      for(int c=0; c<schemaPtr()->preConnections.size(); c++)
 *          if(schemaPtr()->preConnections[c].learing() &&
 *                      schemaPtr()->preConnections[c].file() != "")
 *              schemaPtr()->preConnections[c].setCustom();
 *      saveCustomConnections(FOLDERNAME);
 *  }
 *  saveButton->setEnabled(false);
 *
 *
 *  saveCustomConnections(FOLDERNAME);
 *
 *bool MainWindow::saveCustomConnections(QString foldername){
    int c;
    unsigned int s;
    for(c=0; c<schemaPtr()->preConnections.size(); c++){
        if(schemaPtr()->preConnections[c].isCustom() &&
                schemaPtr()->preConnections[c].file() != ""){
            QFile data(foldername+"/"+schemaPtr()->preConnections[c].file());
            if(data.open(QFile::WriteOnly | QFile::Truncate)){
                QTextStream out(&data);

                for(s=schemaPtr()->preConnections[c].firstId();
                                    s<schemaPtr()->preConnections[c].lastId(); s++){
                    out << A->getRealSynapseSource(s) - schemaPtr()->blocks[schemaPtr()->preConnections[c].source()]->getFirstNeuronIdx() << " " <<
                           A->getRealSynapseTarget(s) - schemaPtr()->blocks[schemaPtr()->preConnections[c].target()]->getFirstNeuronIdx() << " " <<
                           A->getRealSynapseDelay(s) << " " <<
                           A->getRealSynapseWeight(s) << " " <<
                           (int)A->getRealSynapseLearning(s) << "\n";
                }
            }
        }
    }
    return true;
}*/








/*
void MainWindow::updateConnectionTable(){
    schemaPtr()->postConnections.clear();
    //int b,c,source=0,target=0;
    //
    //for(b=0; b<connectionsListWidget->count(); b++)
    //    if(connectionsListWidget->item(b)->checkState()){
    //        for(c=schemaPtr()->blocks[b]->getFirstNeuronIdx();
    //            c<schemaPtr()->blocks[b]->getLastNeuronIdx(); c++)
    //    }

    int source, target, b, u, count = connectionsListWidget->count();
    float weight;
    int flagSource=1, flagTarget=1;
    QList<int> subtract;
    QList<int> until;
    int totalSub=0, totalUntil=0, subS, subT;
    subtract.append(0);
    until.append(0);


    // Find the size of the active blocks
    schemaPtr()->postConSize = 0;
    for(b=0; b<count; b++){
        if(connectionsListWidget->item(b)->checkState()){
            schemaPtr()->postConSize+=schemaPtr()->blocks[b]->getNeuronsSize();
        }
        // Find how much I have to subtract from each source/target
        else{
            subtract.append(totalSub);
            until.append(totalUntil);
            totalSub+=schemaPtr()->blocks[b]->getNeuronsSize();
        }
        totalUntil+=schemaPtr()->blocks[b]->getNeuronsSize();
    }
    subtract.append(totalSub);
    until.append(totalUntil);

    schemaPtr()->postMaxWeight = 0;
    for(int c=0; c<A->getSynapses(); c++){
        source = A->getRealSynapseSource(c);
        target = A->getRealSynapseTarget(c);
        flagSource = -1;
        flagTarget = -1;

        // Check if the source is included
        for(b=0; (b<count && flagSource==-1); b++)
            if(connectionsListWidget->item(b)->checkState())
                if(source >= schemaPtr()->blocks[b]->getFirstNeuronIdx() &&
                   source <  schemaPtr()->blocks[b]->getLastNeuronIdx())
                    flagSource = b;

        // Check if the target is included
        if(flagSource>-1)
            for(b=0; (b<count && flagTarget==-1); b++)
                if(connectionsListWidget->item(b)->checkState())
                    if(target >= schemaPtr()->blocks[b]->getFirstNeuronIdx() &&
                       target <  schemaPtr()->blocks[b]->getLastNeuronIdx())
                        flagTarget = b;

        // If both source and target belong to the neurons of a checked block
        if(flagSource>-1 && flagTarget>-1){
            subS=subT=0;
            // For all the different intervals (diastimata)
            for(u=1; u<until.size();u++){
                // if source or target belong to [u-1,u), subtract this
                if(source < until[u] && source >= until[u-1])
                    subS=subtract[u];
                if(target < until[u] && target >= until[u-1])
                    subT=subtract[u];
            }
            source-=subS;
            target-=subT;
            weight = A->getRealSynapseWeight(c);
            schemaPtr()->postConnections.append(PostConnection(
                                               source,
                                               target,
                                               weight,
                                               A->getRealSynapseDelay(c),
                                               A->getRealSynapseLearning(c)));
            if(weight > schemaPtr()->postMaxWeight)
                schemaPtr()->postMaxWeight = weight;
        }
    }

    QListWidgetItem* item;
    //if(connectionsDetailsListWidget->count() == 0){//Not now ->I want the list
        connectionsDetailsListWidget->clear();
        for(int c=0; c<schemaPtr()->postConnections.size(); c++){
            item = new QListWidgetItem(QString::number(c+1)
                  +" S:"+QString::number(schemaPtr()->postConnections[c].source())
                  +" T:"+QString::number(schemaPtr()->postConnections[c].target())
                  +" W:"+QString::number(schemaPtr()->postConnections[c].weight())
                  +" D:"+QString::number(schemaPtr()->postConnections[c].delay())
                  +" L:"+QString::number(schemaPtr()->postConnections[c].learing()));
            connectionsDetailsListWidget->addItem(item);
        }
    //}

}*/




/*void MainWindow::fullUpdate(){
    if(blockProperties != NULL)
        blockProperties->update();
}*/




/*bool MainWindow::start(){
    if(A == NULL){
        cout << "Cannot start: A == NULL.\n";
        return false;
    }
    if(A->isInitialized()){
        cout << "Cannot start: A already initialized.\n";
        return false;
    }
    //progressDialog->show();
    emit createNetSignal(controls->updateSchema());
    changesAfterLoadingNeuronsToSchema();
    this->turnSchemaSIMULATION(ON);
    CreateNetworkButton->setIcon(QIcon(":/new/prefix1/icons/build-stop.png"));
    //progressDialog->close();
    //modeCRadioButton->click(); //setSchemaMode(2);
    updateTime();
    if(!noTabYet()) tab()->update();
    return true;
}*/







/*void WorkspaceTab::setHighlighted(int value){
     // -- FOCUS PLOT ----------------------------------------------------------
    if(schema->getHighlighted() >= 0 && focusedPlot != NULL){
        delete focusedPlot;
        focusedPlot = NULL;
    }
    if(value >=0){
        if(controls->modeA() && focusedPlot == NULL){
            focusedPlot = new CurrentGraph(schema->blocks[value]->getName()+
                                        "'s current graph",
                                        schema->blocks[value]->getNeuronsSize(),
                                        0, 200, 5, 5,
                                        plotButtomFrame->width()-10,
                                        150, Qt::red, Qt::blue, 500,
                                        plotButtomFrame);
            focusedPlot->setVisible(true);
        }
        else if(controls->modeC() && focusedPlot == NULL){
            focusedPlot = new RasterPlot( schemaPtr()->blocks[value]->getName() +
                                          "'s raster plot",
                                          schemaPtr()->blocks[value]->getNeuronsSize(),
                                          5, 5, plotButtomFrame->width()-10,
                                 //schema->blocks[value]->getNeuronsSize() + 30,
                                          150, Qt::blue, plotButtomFrame);
            focusedPlot->setVisible(true);
        }
    }

    // The method schema->setHighlighted(value) which runs always, returns
    // the potential source of a new connection!
    if(schema->setHighlighted(value) != -1){
        addConnectionWindow = new NewConnectionWindow(schema,
                                                      schema->getHighlighted(),
                                                      value, this);
        addConnectionWindow->show();
    }

    schema->setHighlighted(value);
}*/

#endif // CODEIDONTUSEANYMORE_H
