#include "workspacetab.h"
#include "ui_workspacetab.h"

WorkspaceTab::WorkspaceTab(QString filename, QWidget *parent):
                           QMainWindow(parent),
                           ui(new Ui::WorkspaceTab){
    ui->setupUi(this);

    FILENAME = filename;

    xmlWindow = NULL;
    xmlDock = NULL;
    actionsList = NULL;
    actionsListDock = NULL;
    experimentWidget = NULL;
    experimentDock = NULL;

    pythonWidget = NULL;
    pythonDock = NULL;

    snn = NULL;
    //blockProperties = NULL;
    addBlockWindow = NULL;
    addConnectionWindow = NULL;

    loading_failed = false;

    schema = new ArchitectureWindow(this);
    setCentralWidget(schema);

    this->createDockWindows();


    connect(schema, SIGNAL(schemaDoubleClick(QPoint)),
            this,   SLOT(schemaDoubleClick(QPoint)));
    connect(schema, SIGNAL(blockDoubleClick(QString)),
            this,   SLOT(blockDoubleClick(QString)));
    connect(schema, SIGNAL(blockNewPlot(QString)),
            this,   SLOT(blockNewPlot(QString)));
    connect(schema, SIGNAL(schemaCreateTextBlock(QPoint)),
            this,   SLOT(schemaCreateTextBlock(QPoint)));
    connect(schema, SIGNAL(modifiedSignal()),
            this,   SLOT(schemaModifiedSlot()));
    connect(schema, SIGNAL(schemaAddingNewConnection(QString,QString)),
            this,   SLOT(schemaAddingNewConnection(QString,QString)));
    connect(schema, SIGNAL(showMessageSignal(QString,int)),
            this,   SLOT(showMessage(QString,int)));


    controls = new ControlsWidget(this->schema, this);
    controls->show();

    // I do this first so the simulator will be the one defined in schema!
    loadAll();

    this->setBackend();

    // -------------------------------------------------------------------------
    //tcpip_init = new TCPIP_Init(this);
    //tcpip_init->connect_to_download_data();
    // //connect(snn, SIGNAL(step_done()), this, SLOT(updateBot()));
    // -------------------------------------------------------------------------


    updateTime();
    setMode(Block::modeEdit); // This is also the default in ControlsWidget
}

WorkspaceTab::~WorkspaceTab(){
    if(snn != NULL)
        delete snn;
    delete ui;
}

void WorkspaceTab::setBackend(){

    //client = new TCPIP_Sim(this);
    //client->sendmsg("connected!!!!! oleee!");

    QString path;
    if(FILENAME.contains('\\') || FILENAME.contains('/')) path = FILENAME;
    else path = QDir::toNativeSeparators(UserData::workspace_path+"/"+FILENAME);

    snn = new TCPIP_Sim(this, path);
    schema->updateSimulationPointer(snn);
    QObject::connect(snn,  SIGNAL(simulation_loaded(Simulator*,int*,int*,int)),
                     this, SLOT(networkLoaded(Simulator*,int*,int*,int)),
                     Qt::QueuedConnection);
    connect(snn, SIGNAL(step_done()), this, SLOT(updateBot()));
    connect(snn, SIGNAL(error_signal(QString)),
            this, SLOT(simulationErrorSlot(QString)));
    //client->sendmsg("connected!!!!! oleee!");
}

void WorkspaceTab::resizeEvent(QResizeEvent * event ){
    controls->move(0, this->height()-controls->height()-55);
}

void WorkspaceTab::closeEvent(QCloseEvent * event){
    (void)event;
    if(schema->isSavable()){
        QMessageBox msgBox;
        msgBox.setText(FILENAME + " has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save |
                                  QMessageBox::Discard |
                                  QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Discard);
        switch(msgBox.exec()){
        case QMessageBox::Save:
            schema->save_brn(FILENAME);
            break;
        case QMessageBox::Discard:
            //exit(1);
            break;
        case QMessageBox::Cancel:
            break;
        default:
            // should never be reached
            break;
        }
    }
    else{
        exit(1);
    }
}

bool WorkspaceTab::loadAll(){
    QString path;
    if(FILENAME.contains('\\') || FILENAME.contains('/')) path = FILENAME;
    else path = QDir::toNativeSeparators(UserData::workspace_path+"/"+FILENAME);

    // LOAD SCHEMA
    if(!schema->load_brn(path)){
        qDebug() << "WorkspaceTab::loadAll: Error: BRN could not be loaded!";
        loading_failed = true;
        return false;
    }

    // LOAD XML FILE TO EDITOR
    if(!this->loadXML()){
        qDebug() << "WorkspaceTab::loadAll: Error: XML could not be loaded!";
        loading_failed = true;
        return false;
    }
    // LOAD ACTIONS FROM SCHEMA
    this->refreshActionList();
    // LOAD CONTROLS WIDGET
    addControlsWidget();
    // INFORM MainWindow THAT EVERYTHING IS LOADED
    emit workTabSchemaLoaded();
    return true;
}

bool WorkspaceTab::loadXML(){
    if(xmlWindow == NULL){
        qDebug() << "WorkspaceTab::loadXML: Error: xmlWindow is NULL!";
        return false;
    }
    QString path;
    if(FILENAME.contains('\\') || FILENAME.contains('/')) path = FILENAME;
    else path = QDir::toNativeSeparators(UserData::workspace_path+"/"+FILENAME);

    if(!xmlWindow->loadFile(path)){
        qDebug() <<"WorkspaceTab::loadXML: Error: Could not load the xml file!";
        return false;
    }
    return true;
}

void WorkspaceTab::refreshActionList(){
    if(actionsList != NULL){
        actionsList->clear();
        QStringList list;
        QString str;

        list << "Type\tTime\tNode\tFirst/Last\tDetails";
        for(int i=0;i<schema->actions.size(); i++){
            str = schema->actions[i].type + "\t";
            str+= QString::number(schema->actions[i].time) + "\t";

            if(schema->actions[i].type == "adjust"){
                str+= schema->actions[i].node + "\t";
                str+= QString::number(schema->actions[i].firstNeuron) + " - " +
                      QString::number(schema->actions[i].lastNeuron) + "\t";
                str+= schema->actions[i].parameter + ": ";
                str+= QString::number(schema->actions[i].new_value) + "\t";
            }

            if(schema->actions[i].type == "stimulate"){
                str+= schema->actions[i].node + "\t";
                str+= QString::number(schema->actions[i].firstNeuron) + " - " +
                      QString::number(schema->actions[i].lastNeuron) + "\t";
                str+= "I:"+QString::number(schema->actions[i].new_value)+",";
                str+= " f:"+QString::number(schema->actions[i].frequency) +",";
                str+= " p:"+QString::number(schema->actions[i].phase) +"\t";
            }
            list << str;
        }
        actionsList->addItems(list);
        QFont f = actionsList->item(0)->font();
        f.setBold(true);
        actionsList->item(0)->setFont(f);
        actionsList->item(0)->setBackground(Qt::gray);
        actionsList->setMaximumHeight(20 + schema->actions.size()*20 +
                                      20 + schema->actionsDone.size()*20);
    }
    else
        qDebug() << "WorkspaceTab: ActionList is (NULL)";
}

/* Called by the constructor
 */
void WorkspaceTab::createDockWindows(){
    // LOAD ACTIONS LIST
    actionsListDock = new QDockWidget(tr("List of actions"), this);
    actionsList = new QListWidget(actionsListDock);
    actionsList->setMinimumWidth(400);
    this->refreshActionList();
    actionsListDock->setWidget(actionsList);
    addDockWidget(Qt::RightDockWidgetArea, actionsListDock);

    // LOAD XML WINDOW
    xmlDock = new QDockWidget(tr("XML code"), this);
    xmlDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    if(xmlWindow != NULL){
        qDebug()<<"WorksapceTab::createDockWindows: Error: xmlWindow not NULL!";
        return; //delete xmlWindow; xmlWindow = NULL;
    }
    xmlWindow = new XmlWindow(xmlDock);
    xmlWindow->setWindowFlags(Qt::Widget);
    xmlWindow->move(20,20);

    connect(xmlWindow, SIGNAL(buildTemp_xmlSignal()),
            this,        SLOT(loadTempSchema_xmlSlot()));
    connect(xmlWindow, SIGNAL(f3_xmlSignal()),
            this,        SLOT(toggleActionsList()));
    connect(xmlWindow, SIGNAL(f4_xmlSignal()),
            this,        SLOT(toggleXML()));
    connect(xmlWindow, SIGNAL(f6_xmlSignal()),
            this,        SLOT(toggleExperiment()));
    connect(xmlWindow, SIGNAL(f7_xmlSignal()),
            this,        SLOT(togglePython()));

    //xmlWindow->resize(ui->xmlOutFrame->width(), ui->xmlOutFrame->height());
    //xmlWindow->setVisible(true);
    xmlDock->setWidget(xmlWindow);
    addDockWidget(Qt::RightDockWidgetArea, xmlDock);
    //viewMenu->addAction(dock->toggleViewAction());

    // LOAD EXPERIMENT WIDGET
    experimentDock = new QDockWidget(tr("Experiment controls"), this);
    experimentWidget = new ExperimentWidget(experimentDock);
    experimentDock->setWidget(experimentWidget);
    addDockWidget(Qt::RightDockWidgetArea, experimentDock);
    connect(experimentWidget, SIGNAL(f3_xmlSignal()),
            this,             SLOT(toggleActionsList()));
    connect(experimentWidget, SIGNAL(f4_xmlSignal()),
            this,             SLOT(toggleXML()));
    connect(experimentWidget, SIGNAL(f6_xmlSignal()),
            this,             SLOT(toggleExperiment()));
    connect(experimentWidget, SIGNAL(f7_xmlSignal()),
            this,             SLOT(togglePython()));
    // To disable it
    toggleExperiment();

    // LOAD PYTHON WIDGET
    pythonDock = new QDockWidget(tr("Extract to python"), this);
    pythonWidget = new PythonWidget(pythonDock);
    pythonDock->setWidget(pythonWidget);
    addDockWidget(Qt::RightDockWidgetArea, pythonDock);
    connect(pythonWidget, SIGNAL(f3_xmlSignal()),
            this,             SLOT(toggleActionsList()));
    connect(pythonWidget, SIGNAL(f4_xmlSignal()),
            this,             SLOT(toggleXML()));
    connect(pythonWidget, SIGNAL(f6_xmlSignal()),
            this,             SLOT(toggleExperiment()));
    connect(pythonWidget, SIGNAL(f7_xmlSignal()),
            this,             SLOT(togglePython()));
    connect(pythonWidget, SIGNAL(buildPython_signal(QString)),
            this,             SLOT(buildPython(QString)));

    // To disable it
    togglePython();
}

/* This method checks if ....
 *
 */
bool WorkspaceTab::checkForAction(){
    // If simulation is not initialized return
    if(snn == NULL)
        return false;
    if(!snn->isInitialized())
        return false;
    // If there are no actions return
    if(schema->actions.empty())
        return false;
    // If the first action is not due yet return
    if(schema->actions[0].time > snn->getTimeStep())
        return false;

    // Else, execute the first action!
    Action temp = schema->actions[0];
    schema->actionsDone.push_back(temp);
    schema->actions.pop_front();
    this->refreshActionList();

    if(temp.type == "stop"){
        // TODO: Finish it...
        return true;
    }

    // If we don't have this node return
    if(!schema->blocks.contains(temp.node)){
        qDebug() << "WorkspaceTab::checkForAction: Error: Node "
                 << temp.node << " does not exist!";
        return false;
    }


    if(temp.type == "adjust"){
        // TODO: Finish it...
    }

    else if(temp.type == "stimulate"){
        int first, last;

        // If there is no information about the first/last neurons in actions
        if(temp.firstNeuron == temp.lastNeuron){
            first = schema->blocks[temp.node]->getFirstNeuronIdx();
            last  = schema->blocks[temp.node]->getLastNeuronIdx();
        }

        if(temp.frequency>0.0){
            Stimulus new_stimulus(first, last, temp.new_value, temp.amplitude,
                                  temp.frequency, temp.phase);
            schema->blocks[temp.node]->setStimulus(new_stimulus);
        }
        else{
            Stimulus new_stimulus(first, last, temp.new_value);
            schema->blocks[temp.node]->setStimulus(new_stimulus);
        }
    }

    return true;
}


void WorkspaceTab::setMode(const int &_mode){
    switch(_mode){
    case Block::modeStatesPixels:  schema->setModeStatesPlots(); break;
    case Block::modeEdit:          schema->setModeEdit(); break;
    case Block::modeC:             schema->setModeC(); break;
    case Block::modeRasters:       schema->setModeRasters(); break;
    case Block::modeStatesPlots:   schema->setModeStatesPlots(); break;
    case Block::modeNetwork:       schema->setModeNetwork(); break;
    default:
        qDebug() << "WorkspaceTab::setMode: Error: This mode is not supported!";
    }
}

void WorkspaceTab::setGrid(const bool &on){
    schema->setGrid(on);
    schema->update();
}


bool WorkspaceTab::changeSimulator(const int &simulator){ // ????
    if(snn != NULL){
        qDebug() <<"WorkspaceTab::changeSimulator: snn not null - deleting it!";
        delete snn;
    }
}

// -------------------------------------------------------------------------- //
//                          NETWORK CONTROLS                                  //
// -------------------------------------------------------------------------- //
bool WorkspaceTab::play(){
    if(snn == NULL){
        qDebug() << "WorkspaceTab::play: Error: snn is NULL!";
        return false;
    }
    else if(!snn->isInitialized()){
        qDebug() << "WorkspaceTab::play: Error: snn is not initialized!";
        return false;
    }

    snn->play();
    qDebug() << "WorkspaceTab::play: tcpip backend started..";

    return true;
}

bool WorkspaceTab::pause(){
    snn->pause();
    return true;
}

bool WorkspaceTab::stop(){
    snn->stop();

    //blockProperties->setSimulatorPointer(NULL);

    // Delete first and last id in the blocks of schema:
    for(int c=0; c<schema->connections.size(); c++){
        schema->connections[c].setFirstId(0);
        schema->connections[c].setLastId(0);
    }

    // Reseting the actions of the schema!
    schema->restartActions();
    this->refreshActionList();

    this->updateTime();
    return true;
}

void WorkspaceTab::setTimeSpeed(const int &value){
    schema->setSpeed(value);

    //??
}

/* it creates a SNN based on the current schema! SOS: To delete - doesn't apply any more!
 * SOS: The worker only reads from schema so in case that I disable editing the
 * schema while loading snn, that's fine. The neural network on the onther hand
 * needs to be created on another place and then when it's ready to take the
 * pointer to snn.
*/
bool WorkspaceTab::create_network(){
    // Experiment mode
    snn->startInitializing();
    return true;
}

bool WorkspaceTab::network_is_running(){
    if(snn != NULL && snn->isInitialized()) return true;
    return false;
}

bool WorkspaceTab::save(){
    return this->save("");
}

bool WorkspaceTab::save(QString path = ""){
    if(path == ""){
        // If the user has not specified a name yet, return false which triggers
        // save_as
        if(FILENAME.contains("untitled"))
            return false;
        if(FILENAME.contains('\\') || FILENAME.contains('/')) path = FILENAME;
        else path = QDir::toNativeSeparators(UserData::workspace_path+"/"+FILENAME);
    }
    else{
        // Change the filename
        FILENAME = path.split(QDir::toNativeSeparators("/"),
                              QString::SkipEmptyParts).last();
    }
    return schema->save_brn(path);
}

bool WorkspaceTab::exportNetwork(){
    if(snn == NULL){
        qDebug() << "WorkspaceTab::saveNetwork: Error: snn is null!";
        return false;
    }
    else if(!snn->isInitialized()){
        qDebug() << "WorkspaceTab::saveNetwork: Error: snn not initialized!";
        return false;
    }

    QString path;
    if(FILENAME.contains('\\') || FILENAME.contains('/')) path = FILENAME;
    else path = QDir::toNativeSeparators(UserData::workspace_path+"/"+FILENAME);
    if(snn->exportNetwork(path)){
        ui->statusBar->showMessage("Network (neurons + synapses) was exported!");
        return true;
    }
    return false;
}

bool WorkspaceTab::backup(){
    if(schema->backup(UserData::workspace_path)){
        ui->statusBar->showMessage("Back up was successful!");
        return true;
    }
    ui->statusBar->showMessage("Back up failed!");
    return false;
}

void WorkspaceTab::receiveProgress(int value, QString stage){
    ui->statusBar->showMessage(QString::number(value) + "% - " + stage);
    qDebug() << value << "% - "  << stage;
}

void WorkspaceTab::networkLoaded(Simulator *snn_input, int *firstId,
                                 int *lastId, int idSize){
    snn = snn_input;
    //blockProperties->setSimulatorPointer(snn_input);
    if(firstId != NULL && lastId != NULL){
        for(int c=0; c<idSize; c++){
            schema->connections[c].setFirstId(firstId[c]);
            schema->connections[c].setLastId(lastId[c]);
        }
        delete [] firstId;
        delete [] lastId;
    }
    emit workTabNetworkLoaded();

    //if(!A->learningInitialized())
    //    learningCheckBox->setEnabled(false);

    //if(!updateSchemaCheckBox->isChecked())
    //    updateSchemaCheckBox->setCheckable(false);
}
// -------------------------------------------------------------------------- //
//                          REAL-TIME UPDATES                                 //
// -------------------------------------------------------------------------- //
void WorkspaceTab::updateTime(){
    if(snn != NULL && snn->isInitialized()){
        // fmod: modulo for floating point numbers
        // modf: Breaks into fractional and integral parts
        sec = (int)snn->getTimeStep()/1000;
        f_ms, ms;
        f_ms = modf(fmod(snn->getTimeStep(),1000.0), &ms);
        timeLabelStr = QString("%1:").arg((sec/60),2,10,QChar('0'))+
                       QString("%1:").arg(sec,2,10,QChar('0'))+
                       QString("%1.").arg((int)ms,3,10,QChar('0'))+
                       QString("%1").arg((int)(f_ms*1000.0),3,10,QChar('0'));
    }
    else timeLabelStr = "00:00:000.000";

    emit timeSignal(timeLabelStr);
}


void WorkspaceTab::updateBot(){
    if(snn == NULL || !snn->isInitialized()){
        qDebug() << "WorkspaceTab::updateBot: Error: "
                    "snn not initialized or null!";
        return;
    }
    this->checkForAction();
    snn->step(this->getSpeed());

#ifdef RUNTIME_LOGS
    qDebug() << "New Step: "
             << "t: " << snn->getTimeStep() << "\t"
             << "ts: "<< snn->getTimeStepSize() << "\n"
             << "u: " << snn->getRealNeuronStateU(0) << "\t"
             << "v: " << snn->getRealNeuronStateV(0);
#endif
    updateTime();

    ArchitectureWindow::BlockIter bl;

    if(schema->updateSchemaIsOn()){
        for(bl = schema->blocks.begin(); bl != schema->blocks.end(); bl++)
            bl.value()->updateMe();

        if(schema->modeStatesPixels()){
            // Update the blocks
            bl = schema->blocks.begin(); // block b1
            for(int i=0; i<snn->getNeurons(); i++){
                if( i >= bl.value()->getLastNeuronIdx())
                    bl++;
                bl.value()->setNeuronMemPot(i - bl.value()->getFirstNeuronIdx(),
                                            snn->getNeuronState(i));
            }
        }
        else if(schema->modeC()){
            schema->update_f_rates(snn->getSpikesArray(), snn->getSpikes());
        }
        else if(schema->modeRasters()){
            schema->update_f_rates(snn->getSpikesArray(), snn->getSpikes());

            // If spiking update raster if rate-based update rates!
            for(bl = schema->blocks.begin(); bl != schema->blocks.end(); bl++){
                if(bl.value()->isSpiking()){
                    bl.value()->updateRaster();
                }
                else{
                    for(int i=bl.value()->getFirstNeuronIdx();
                                         i<bl.value()->getLastNeuronIdx(); i++){
                        bl.value()->setNeuronMemPot(
                                            i - bl.value()->getFirstNeuronIdx(),
                                            snn->getNeuronState(i));
                    }
                }
            }
        }
        else if(schema->modeStatesPlots()){
            bl = schema->blocks.begin(); // block b1
            for(int i=0; i<snn->getNeurons(); i++){
                if( i >= bl.value()->getLastNeuronIdx() )
                    bl++;
                bl.value()->setNeuronMemPot(i - bl.value()->getFirstNeuronIdx(),
                                            snn->getNeuronState(i));
            }
        }
    }

    // PLOT WINDOWS:
    this->calcLocalFieldPotentials();
    for(int i = 0; i<plotWindows.size(); i++)
        plotWindows[i]->updateActivePlot();

    // HERE I SHOULD UPDATE ALL WINDOWS AND WIDGETS!
    schema->update();

    int h = 0;
    if(schema->getHighlighted() != "")
        h = schema->blocks[schema->getHighlighted()]->getFirstNeuronIdx();
    QList<float> data;
    data.append(snn->getRealParameter(h,0));
    data.append(snn->getRealParameter(h,1));
    data.append(snn->getRealParameter(h,2));
    data.append(snn->getRealParameter(h,3));
    data.append(snn->getRealParameter(h,4));
    data.append(snn->getRealParameter(h,5));
    data.append(snn->getRealParameter(h,6));
    data.append(snn->getRealParameter(h,7));
    data.append(snn->getRealParameter(h,8));
    data.append(snn->getRealParameter(h,9));
    data.append(snn->getRealParameter(h,10));
    data.append(snn->getRealParameter(h,11));
    data.append(snn->getRealParameter(h,12));
    data.append(snn->getRealParameter(h,13));
    data.append(snn->getRealParameter(h,14));
    data.append(snn->getRealParameter(h,15));
    data.append(snn->getRealParameter(h,16));
    data.append(snn->getRealParameter(h,17));

    data.append(snn->getRealNeuronState(h,0));
    data.append(snn->getRealNeuronState(h,1));
    data.append(snn->getRealNeuronState(h,2));
    data.append(snn->getRealNeuronState(h,3));

    emit infoSignal(data);
}




void WorkspaceTab::calcLocalFieldPotentials(){
  //for(i=0; schema->LFPvalue.size(); i++)
    //  schema->LFPvalue[i] = 1; // ZAF: normally it's 0
    //
    for(int i=0; i<schema->LFPindex.size(); i++){
        schema->LFPvalue[schema->LFPindex[i]] = 0;
        schema->LFPvalue[schema->LFPindex[i]] = snn->getLocalFieldPotential(
                    schema->blocks[schema->LFPindex[i]]->getFirstNeuronIdx(),
                    schema->blocks[schema->LFPindex[i]]->getLastNeuronIdx());
    }
}





bool WorkspaceTab::addBlock(){
    if(snn != NULL && snn->isInitialized()){
        qDebug() << "WorkspaceTab::addBlock: Error: snn is initialized";
        return false;
    }
    qDebug() << "WorkspaceTab::addBlock: Not implemented yet!";
    this->update();
    return false;
}

bool WorkspaceTab::removeAllBlocks(){
    if(snn != NULL && snn->isInitialized()){
        qDebug() << "WorkspaceTab::removeAllBlocks: Error: snn is initialized";
        return false;
    }
    schema->connections.clear();

    QMap<QString,Block*>::iterator it = schema->blocks.begin();
    while(it != schema->blocks.end()){
        delete schema->blocks[it.key()];
        it++;
    }
    schema->blocks.clear();
    this->update();
    schema->update();
    emit schemaModifiedSignal();
}


bool WorkspaceTab::removeBlock(){
    if(snn != NULL && snn->isInitialized()){
        qDebug() << "WorkspaceTab::removeBlock: Error: snn is initialized";
        return false;
    }

    if(schema->blocks.contains(schema->getHighlighted())){
        // Remove block
        schema->blocks.remove(schema->getHighlighted());
        // Remove connections

        schema->deleteRedundantConnections();
    }
    this->update();
    emit schemaModifiedSignal();
}



void WorkspaceTab::schemaAddingNewConnection(QString source, QString target){
    addConnectionWindow = new NewConnectionWindow(schema, source, target, this);
    addConnectionWindow->show();
}


void WorkspaceTab::plotWindowDeleteRequest(int index){
    if(index >= plotWindows.size()){
        qDebug() << "WorkspaceTab::deletePlotWindow:"
                    " Error: Index of plot ("<< index <<") does not exist!";
        return;
    }
    disconnect(plotWindows[index], SIGNAL(plotWindowDeleteRequest(int)),
               this,               SLOT(plotWindowDeleteRequest(int)));
    // Remove dock widget from the layout, close the child widget, change the
    // internal indices and delete the parent (dock widget).
    removeDockWidget(plotWindowsDock[index]);
    plotWindows[index]->close();
    plotWindows.removeAt(index);
    for(int i=0; i<plotWindows.size(); i++)
        plotWindows[i]->setMyIndex(i);
    delete plotWindowsDock[index];
    plotWindowsDock.removeAt(index);
    qDebug() << "WorkspaceTab: Plot window " << index << " deleted!";
}



void WorkspaceTab::schemaDoubleClick(QPoint point){
    // ADD NEW BLOCK:
    addBlockWindow = new NewBlockWindow(schema, point, this);
    addBlockWindow->show();
}

void WorkspaceTab::blockDoubleClick(QString id){
    if(id == ""){
        qDebug() << "WorkspaceTab("+FILENAME+")::blockDoubleClick:"
                    "Nothing highlighted..";
        return;
    }
    else if( !schema->blocks.contains(id)){
        qDebug() << "WorkspaceTab("+FILENAME+")::blockDoubleClick: Error:"
                    "Index of highlighted block out of range..";
        exit(1);
    }

    if(!blockPropertiesWindowsDock.contains(id)){
        QString title = id;
        blockPropertiesWindowsDock[id] = new QDockWidget(title, this);
        blockPropertiesWindows.append(new BlockPropertiesWidget(schema->blocks[id],
                                      snn, blockPropertiesWindowsDock.last()));
        //blockPropertiesWindows.last()->show();
        blockPropertiesWindowsDock[id]->setWidget(blockPropertiesWindows.last());
        //blockPropertiesWindowsDock[id]->show();
        addDockWidget(Qt::LeftDockWidgetArea, blockPropertiesWindowsDock[id]);
    }
}

void WorkspaceTab::blockNewPlot(QString id){
    if(id == ""){
        qDebug() << "WorkspaceTab("+FILENAME+")::blockNewPlot:"
                    "Nothing highlighted..";
        return;
    }
    else if( !schema->blocks.contains(id)){
        qDebug() << "WorkspaceTab("+FILENAME+")::blockNewPlot: Error:"
                    "Index of highlighted block out of range..";
        exit(1);
    }

    // ADD NEW PLOT WINDOW:
    QString title = id;
    plotWindowsDock.append(new QDockWidget(title, this));
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    int index = plotWindows.size();
    plotWindows.append(new PlotWindow(id, schema, snn, index,
                                      plotWindowsDock.last()));
    schema->addLFPcounter(id);
    connect(plotWindows.last(), SIGNAL(plotWindowDeleteRequest(int)),
            this,               SLOT(plotWindowDeleteRequest(int)));
    //plotWindows.last()->show();
    plotWindows.last()->setWindowFlags(Qt::Widget);
    plotWindowsDock.last()->setWidget(plotWindows.last());
    //plotWindowsDock.last()->show();
    addDockWidget(Qt::RightDockWidgetArea, plotWindowsDock.last());
}

void WorkspaceTab::schemaCreateTextBlock(QPoint point){
    // ADD NEW TEXT BLOCK:
    if(schema->getHighlighted() != ""){                                         // ZAF SOS: Why???
        TextBlock *block = new TextBlock("Text", 12, point.x(), point.y(),
                                         200, 100, 000000, schema);
        schema->textBlocks.append(*block);
    }

    // EDIT TEXT IN EXISTING TEXT BLOCK:
    //else{
    // ...
    //}
}

// CREATE NEW MAIN_CONTROLS
void WorkspaceTab::addControlsWidget(){
    /*if(controls != NULL){
        qDebug() <<"WorkspaceTab::addControlsWidget: Error: Controls not NULL!";
        return;
        //delete controls;
    }*/
}

void WorkspaceTab::disableControls(){
}

void WorkspaceTab::enableControls(){
}


void WorkspaceTab::loadTempSchema_xmlSlot(){
    // LOAD SCHEMA
    if(schema == NULL){
        qDebug() << "WorkspaceTab::loadTempSchema_xmlSlot: Error: "
                    "schema is NULL!";
        return;
    }
    if(!schema->load_brn(QDir::toNativeSeparators(
                             UserData::only_path+"/temp.brn")))//TEMP_FILE
        qDebug() << "WorkspaceTab::loadTempSchema_xmlSlot: Error: "
                    "Cannot load temp.brn!";
    schema->update();
    emit schemaModifiedSignal();
}

void WorkspaceTab::toggleXML(){
    if(xmlWindow == NULL)
        qDebug() << "WorkspaceTab::toggleXML: xmlWindow is NULL!";
    if(xmlDock == NULL){
       qDebug()<<"WorkspaceTab::toggleXML: Error: xmlDock is NULL!";
       return;
    }
    if(xmlDock->isHidden())
        xmlDock->show();
    else
        xmlDock->hide();
    emit toggleXMLsignal();
}

void WorkspaceTab::toggleActionsList(){
    if(actionsList == NULL)
        qDebug() << "WorkspaceTab::toggleActionsList: actionsList is NULL!";
    if(actionsListDock == NULL){
       qDebug()<<"WorkspaceTab::toggleActionsList: Error: ac.listDock is NULL!";
       return;
    }
    if(actionsListDock->isHidden())
        actionsListDock->show();
    else
        actionsListDock->hide();
    emit toggleActionsListSignal();
}

void WorkspaceTab::toggleExperiment(){
    if(experimentWidget == NULL)
        qDebug() << "WorkspaceTab::toggleExperiment: experimentWidget is NULL!";
    if(experimentDock == NULL){
       qDebug()<<"WorkspaceTab::toggleExperiment: Error: experimeDock is NULL!";
       return;
    }
    if(experimentDock->isHidden())
        experimentDock->show();
    else
        experimentDock->hide();
    emit toggleExperimentSignal();
}

void WorkspaceTab::togglePython(){
    if(pythonWidget == NULL)
        qDebug() << "WorkspaceTab::togglePython: pythonWidget is NULL!";
    if(pythonDock == NULL){
       qDebug()<< "WorkspaceTab::togglePython: Error: pythonDock is NULL!";
       return;
    }
    if(pythonDock->isHidden())
        pythonDock->show();
    else
        pythonDock->hide();
    emit togglePythonSignal();
}

// This is the standard procedure after every modification of the schema!
// I save the current version on a temp file and load this temp file in
// the XML viewer.
void WorkspaceTab::schemaModifiedSlot(){
    if(xmlWindow != NULL){
        schema->save_brn(QDir::toNativeSeparators(
                             UserData::only_path+"/temp.brn"));
        xmlWindow->loadFile(QDir::toNativeSeparators(
                                UserData::only_path+"/temp.brn"));
    }
    //qDebug() << "HERE I WILL CHANGE XML IF DIDN'T SEND COMMAND. IF YES, I WILL DEACTIVATE A BOOLEAN";
    emit schemaModifiedSignal();
}

////////////////////////////////////////////////////////////////////////////////
///                            TO CHANGE                                     ///
////////////////////////////////////////////////////////////////////////////////

void WorkspaceTab::experimentUpdate(QString msg){
    experimentWidget->write(msg);

}

void WorkspaceTab::experimentEndOfCycle(float p, float gp){
    experimentWidget->setProgress(p);
    experimentWidget->setProgressGeneral(gp);
}

void WorkspaceTab::experimentFinished(){
    experimentWidget->write("Experiment completed!");
    experimentWidget->setProgress(100);
    qDebug() << "Experiment completed!";
}








////////////////////////////////////////////////////////////////////////////////
///                               NEW                                        ///
////////////////////////////////////////////////////////////////////////////////
void WorkspaceTab::buildPython(QString simulator){
    qDebug() << "WorkspaceTab::buildPython: Process starts with"
             << simulator << "simulator!";
    if(simulator == "brian"){
        //schema->savePythonBrian();
    }
}

// Will be used by everything!
void WorkspaceTab::showMessage(const QString &text, int timeout = 2000){
    ui->statusBar->showMessage(text, timeout);
}
















