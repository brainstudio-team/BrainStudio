#include "mainwindow.h"

MainWindow::MainWindow(QString filename, QWidget *parent) : QMainWindow(parent){
    setupUi(this);

    this->setWindowIcon(QIcon(":/new/prefix1/icons/brainstudio-logo.png"));

    top_frame_separator = 800;
    welcomeWindow = NULL;
    aboutWindow = NULL;
    propertiesDialog = NULL;

    if(UserData::workspace_path == "empty" && filename==""){
        welcomeWindow = new WelcomeWindow();
        connect(welcomeWindow, SIGNAL(allDone()), this, SLOT(init()));
        welcomeWindow->show();
    }
    else
        this->init(filename);
}

MainWindow::~MainWindow() {
    backend_process->kill();
    /*if(welcomeWindow != NULL)
        delete welcomeWindow;*/
}

void MainWindow::init(QString givenfilewithpath=""){
    qDebug() << "MainWindow::init: workspace_path:" << UserData::workspace_path;

    // -- GUI ------------------------------------------------------------------
    this->resize(1460, 980);
    this->move(0,0);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    this->tabWidget->setTabsClosable(true);
    this->connect(tabWidget, SIGNAL(tabCloseRequested(int)),
                  this,      SLOT(removeTab(int)));
    this->tabWidget->tabBar()->setMaximumWidth(top_frame_separator);

    this->mainFrame->layout()->removeWidget(this->topFrame);
    this->mainFrame->layout()->removeWidget(this->topFrameBackground);
    this->mainFrame->layout()->removeWidget(this->backend_frame);

    this->saveButton->setEnabled(false);
    this->playButton->setEnabled(false);
    //this->learningCheckBox->setVisible(false);

    this->IPlineEdit->setText(UserData::tcpip_hostname);
    this->PORTlineEdit->setText(QString::number(UserData::tcpip_port));

    // -- VARIABLES ------------------------------------------------------------
    run = false;

    // -------------------------------------------------------------------------
    tcpip_init = new TCPIP_Init(this);
    //tcpip_init->connect_to_download_data();
    connect(tcpip_init, SIGNAL(nodes_downloaded(QString)),
            this, SLOT(nodes_downloaded(QString)));
    connect(tcpip_init, SIGNAL(edges_downloaded(QString)),
            this, SLOT(edges_downloaded(QString)));

    // To download backend data.
    this->on_PORTlineEdit_editingFinished();
    // -------------------------------------------------------------------------

    //qDebug() << "==================== BACK-END ==========================";
    backend_process = new QProcess(this);
    QCoreApplication::processEvents();
    connect(backend_process, SIGNAL(readyReadStandardError()),
            this, SLOT(backend_error()));
    connect(backend_process, SIGNAL(readyReadStandardOutput()),
            this, SLOT(backend_output()));
    this->backend_restart();
    //qDebug() << "========================================================";

    QFileInfo givenFile(givenfilewithpath);
    QFileInfo defaultFile(UserData::workspace_path+"/network1.brn");

    // If file is given try to load this file:
    if(givenFile.exists() && givenFile.isFile()){
        this->loadNewTab(givenfilewithpath);
    }
    // Else, try to load the default file
    else if(defaultFile.exists() && defaultFile.isFile()){
        this->loadNewTab("network1.brn");
    }
    // If not show a new file
    else{
        this->onNew();
    }

    this->showMaximized();
}

bool MainWindow::noTabYet() const {
    if(tabWidget->currentIndex() < 0)
        return true;
    if(tab() == NULL)
        return true;
    return false;
}

void MainWindow::keyPressEvent(QKeyEvent * event){
    if(event->key()==Qt::Key_Escape)    onExit();
    else if(event->key()==Qt::Key_W)    onExit();
    else if(event->key()==Qt::Key_T)    on_tcpipButton_clicked();
    else if(event->key()==Qt::Key_1)    selectTabWithKey(0);
    else if(event->key()==Qt::Key_2)    selectTabWithKey(1);
    else if(event->key()==Qt::Key_3)    selectTabWithKey(2);
    else if(event->key()==Qt::Key_4)    selectTabWithKey(3);
    else if(event->key()==Qt::Key_5)    selectTabWithKey(4);
    else if(event->key()==Qt::Key_6)    selectTabWithKey(5);
    else if(event->key()==Qt::Key_7)    selectTabWithKey(6);
    else if(event->key()==Qt::Key_8)    selectTabWithKey(7);
    else if(event->key()==Qt::Key_9)    selectTabWithKey(8);
    else if(event->key()==Qt::Key_F3)   { this->onActionsListToggle(); }
    else if(event->key()==Qt::Key_F4)   { this->onXmlToggle(); }
    else if(event->key()==Qt::Key_F6)   { this->onExperimentToggle(); }
    else if(event->key()==Qt::Key_F7)   { this->onPythonToggle(); }
    else if(event->key()==Qt::Key_Space)on_playButton_clicked();
    // MOVE BLOCKS WITH ARROWS
    else if(event->key()==Qt::Key_Up)   { if(!noTabYet()) tab()->keyUP(); }
    else if(event->key()==Qt::Key_Down) { if(!noTabYet()) tab()->keyDOWN(); }
    else if(event->key()==Qt::Key_Left) { if(!noTabYet()) tab()->keyLEFT(); }
    else if(event->key()==Qt::Key_Right){ if(!noTabYet()) tab()->keyRIGHT(); }
}

void MainWindow::resizeEvent(QResizeEvent * event){
    this->topFrame->resize(this->width()-top_frame_separator, 41);
    this->topFrame->move(top_frame_separator, 0);
    this->topFrameBackground->resize(top_frame_separator, 41);
    this->topFrameBackground->move(0, 0);

    this->backend_frame->resize(500, 500);
    this->backend_frame->move(this->width() -
                                   this->backend_frame->width(), 41);
    this->backend_frame->setVisible(false);
}


bool MainWindow::play(){
    if(!noTabYet() && tab()->play()){
        this->playButton->
                setIcon(QIcon(":/new/prefix1/icons/Pause-Disabled-icon48.png"));
        return true;
    }
    qDebug() << "MainWindow::play: failed.";
    return false;
}

bool MainWindow::pause(){
    if(!noTabYet() && tab()->pause()){
        playButton->setIcon(QIcon(":/new/prefix1/icons/Play-icon.png"));
        return true;
    }
    qDebug() << "MainWindow::pause: failed.";
    return false;
}

bool MainWindow::stop(){
    if(!noTabYet() && (this->pause() || true) && tab()->stop()){
        run = false;
        CreateNetworkButton->setIcon(QIcon(":/new/prefix1/icons/build.png"));
        IPlineEdit->setEnabled(true);
        PORTlineEdit->setEnabled(true);
        action_create_network->setText("Create network");
        playButton->setEnabled(false);
        return true;
    }
    qDebug() << "MainWindow::stop: failed.";
    return false;
}



// *******************************SLOTS************************************** //
void MainWindow::workTabSchemaLoaded(){
    qDebug() << "MainWindow::workTabSchemaLoaded: This function is empty!";
}

void MainWindow::workTabNetworkLoaded(){
    CreateNetworkButton->setIcon(QIcon(":/new/prefix1/icons/stop.png"));
    IPlineEdit->setEnabled(false);
    PORTlineEdit->setEnabled(false);
    action_create_network->setText("Stop simulation");
    playButton->setEnabled(true);
}

void MainWindow::workTabSchemaModifiedSlot(){
    this->saveButton->setEnabled(true);
}

void MainWindow::workTabGetTime(QString time){
    this->timeLabel->setText(time);
}

void MainWindow::workTabGetInfo(QList<float> data){
}
// -------------------------------------------------------------------------- //



// *****************************GUI SLOTS************************************ //
// ACTIONS
void MainWindow::on_action_create_network_triggered(){
    this->onCreateNetwork();
}
void MainWindow::on_action_new_triggered(){
    this->onNew();
}
void MainWindow::on_action_load_triggered(){
    this->onOpen();
}
void MainWindow::on_action_save_triggered(){
    this->onSave();
}
void MainWindow::on_action_save_as_triggered(){
    this->onSaveAs();
}
void MainWindow::on_action_export_network_triggered(){
    this->onExportNetwork();
}
void MainWindow::on_action_backup_triggered(){
    this->onBackup();
}
void MainWindow::on_action_exit_triggered(){
    this->onExit();
}
void MainWindow::on_action_about_triggered(){
    //QSound::play("hp.wav");
    aboutWindow = new AboutWindow(this);
    aboutWindow->show();
}
void MainWindow::on_action_properties_triggered(){
    propertiesDialog = new PropertiesDialog(this);
    propertiesDialog->show();
}
void MainWindow::on_action_grid_toggled(bool arg1){
    this->onSetGrid(arg1);
}
void MainWindow::on_actionXML_viewer_triggered(){
    this->onXmlToggle();
}
void MainWindow::on_xmlButton_clicked(){
    this->onXmlToggle();
}
void MainWindow::on_actionActions_list_triggered(){
    this->onActionsListToggle();
}
void MainWindow::on_actionsListButton_clicked(){
    this->onActionsListToggle();
}

void MainWindow::on_removeBlockButton_clicked(){
    if(!noTabYet() && tab()->removeBlock()){
        action_create_network->setEnabled(true);    // ????
        CreateNetworkButton->setEnabled(true);
    }
}
void MainWindow::on_IPlineEdit_editingFinished(){
    IPlineEdit->setStyleSheet("background: #fff;");
    PORTlineEdit->setStyleSheet("background: #fff;");
    tcpipButton->setText("  Connecting..");
    UserData::tcpip_hostname = this->IPlineEdit->text();
    UserData::save();
    // -------------------------------------------------------------------------
    tcpip_init->connect_to_download_data();
}
void MainWindow::on_PORTlineEdit_editingFinished(){
    IPlineEdit->setStyleSheet("background: #fff;");
    PORTlineEdit->setStyleSheet("background: #fff;");
    tcpipButton->setText("  Connecting..");
    UserData::tcpip_port = (int)this->PORTlineEdit->text().toShort();
    UserData::save();
    tcpip_init->connect_to_download_data();
}


// BUTTONS
void MainWindow::on_playButton_clicked(){
    this->onPlay();
}
void MainWindow::on_CreateNetworkButton_clicked(){
    this->onCreateNetwork();
}
void MainWindow::on_saveButton_clicked(){
    this->onSave();
}
void MainWindow::on_openFolderButton_clicked(){
    this->onOpenFolder();
}
// -------------------------------------------------------------------------- //

// ******************************METHODS************************************* //
void MainWindow::onExit(){
    //if(saveButton->isEnabled())
    //else
    //exit(1);
    this->close(); // So it can call the destructors!
}
void MainWindow::onNew(){
    QString untitled_path = UserData::only_path + "/" + "untitled" +
                            QString::number(tabWidget->count()+1)+".brn";

    QFile data(untitled_path);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream out(&data);
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
               "<schema speed=\"1\">\n"
               "\t<actions>\n"
               "\t</actions>\n"
               "\t<nodes>\n"
               "\t</nodes>\n"
               "\t<edges>\n"
               "\t</edges>\n"
               "\t<textBlocks>\n"
               "\t</textBlocks>\n"
               "</schema>\n";
    }
    loadNewTab(untitled_path);
}

void MainWindow::onOpen(){
    QString path =
                 QFileDialog::getOpenFileName(this,"Open a network",
                                              UserData::workspace_path,"*.brn");
    if(path == ""){
        // Open was cancelled
        return;
    }
    QStringList list1 = path.split(QDir::toNativeSeparators("/"),
                                   QString::SkipEmptyParts);
    QString filename, FOLDERNAME;

    if(list1.size()>2){
        FOLDERNAME = path.left(path.size()-list1.last().size());
        filename  = list1[list1.size()-1];
    }
    else if(list1.size()>1){
        filename  = list1[list1.size()-1];
    }
    else{
        qDebug() << "MainWindow::open: Error: Path not recognized!";
    }

    if(UserData::workspace_path+"/" != FOLDERNAME){
        qDebug() << "MainWindow::onOpen: Path is different!";
        qDebug() << "The reason if it's not working could be the path."
                 << "\nPath: " << path
                 << "\nFOLDERNAME: " << FOLDERNAME
                 << "\nWorkspacePath: " << UserData::workspace_path
                 << "\nfilename: " << filename;
        // ZAF SOS: Display a message asking them if they want to change the
        //          workspace_path..
        loadNewTab(path);
    }
    else{
        loadNewTab(filename);
    }
}

bool MainWindow::onSave(){
    if(!noTabYet()){
        if(!tab()->save()){
            return this->onSaveAs();
        }
        if(tab()->loadXML()){
            this->saveButton->setEnabled(false);
            return true;
        }
    }
    return false;
}

bool MainWindow::onSaveAs(){
    if(!noTabYet()){
        QString path = QFileDialog::getSaveFileName(this, "Save brn network",
                                             UserData::workspace_path, "*.brn");
        if(path == ""){
            // Save was cancelled
            return false;
        }
        else if(path.right(4) != ".brn"){
            path += ".brn";
        }
        if(tab()->save(path) && tab()->loadXML()){
            this->saveButton->setEnabled(false);
            return true;
        }
    }
    return false;
}

bool MainWindow::onExportNetwork(){
    if(!noTabYet() && tab()->exportNetwork()){
        return true;
    }
    return false;
}

bool MainWindow::onBackup(){
    if(!noTabYet() && tab()->backup()){
        return true;
    }
    return false;
}

bool MainWindow::onOpenFolder(){
    QString command;
    QString path = QDir::toNativeSeparators(UserData::workspace_path);
    qDebug() << "MainWindow::onOpenFolder: Opening " << path;
#ifdef WIN32
    command = "explorer.exe " + path;
#elif linux
    command = "xdg-open " + path;
#endif
    system(command.toStdString().c_str());

    return true;
}

bool MainWindow::onCreateNetwork(){
    this->onSave();
    if(!noTabYet()){
        if(!tab()->network_is_running())
            return tab()->create_network();
        else
            return this->stop();
    }
    return false;
}

void MainWindow::onPlay(){
    if(!noTabYet() && !tab()->network_is_running()){
        tab()->create_network();
        return; // returning because we need to wait for the network creation
    }

    if(!run) run = this->play();
    else run = !this->pause();
}

bool MainWindow::onSetGrid(const bool &on){
    if(noTabYet())
        return false;
    tab()->setGrid(on);
    return true;
}

void MainWindow::onXmlToggle(){
    if(noTabYet()){
        qDebug() << "MainWindow::onXmlToggle: Error: No tab yet!";
        return;
    }

    if(!tab()->xmlVisible()){
        xmlButton->setIcon(QIcon(":/new/prefix1/icons/xml-tool-icon.png"));
        actionXML_viewer->setChecked(true);
    }
    else{
        xmlButton->setIcon(QIcon(":/new/prefix1/icons/xml-tool-icon - gray.png"));
        actionXML_viewer->setChecked(false);
    }
    tab()->toggleXML();
}

void MainWindow::onActionsListToggle(){
    if(noTabYet()){
        qDebug() << "MainWindow::onActionsListToggle: Error: No tab yet!";
        return;
    }

    if(!tab()->actionsListVisible()){
        actionsListButton->setIcon(QIcon(":/new/prefix1/icons/"
                                         "actions-list.png"));
        actionActions_list->setChecked(true);
    }
    else{
        actionsListButton->setIcon(QIcon(":/new/prefix1/icons/"
                                         "actions-list-icon.png"));
        actionActions_list->setChecked(true);
    }
    tab()->toggleActionsList();
}

void MainWindow::onExperimentToggle(){
    if(noTabYet()){
        qDebug() << "MainWindow::onExperimentToggle: Error: No tab yet!";
        return;
    }

    /*if(!tab()->ExperimentVisible())
        ExperimentButton->setIcon(QIcon(":/new/prefix1/icons/"
                                        "actions-list.png"));
    else
        ExperimentButton->setIcon(QIcon(":/new/prefix1/icons/"
                                        "actions-list-icon.png"));*/
    tab()->toggleExperiment();
}

void MainWindow::onPythonToggle(){
    if(noTabYet()){
        qDebug() << "MainWindow::onPythonToggle: Error: No tab yet!";
        return;
    }

    /*if(!tab()->PythonVisible())
        pythonButton->setIcon(QIcon(":/new/prefix1/icons/"
                                    "actions-list.png"));
    else
        pythonButton->setIcon(QIcon(":/new/prefix1/icons/"
                                    "actions-list-icon.png"));*/
    tab()->togglePython();
}
// -------------------------------------------------------------------------- //






void MainWindow::on_rtNeuronSlider_sliderMoved(int position){
}


void MainWindow::nodes_downloaded(QString data){
    BackendData::load_nodes(data);
    tcpip_init->getEdges();
}


void MainWindow::edges_downloaded(QString data){
    BackendData::load_edges(data);
    IPlineEdit->setStyleSheet("background: #ccc;");
    PORTlineEdit->setStyleSheet("background: #ccc;");
    tcpipButton->setText("  Connected!");

    tcpip_init->disconnect();
}


void MainWindow::error_workspaceTab_slot(QString msg){
    this->stop();
    QMessageBox msgBox;
    msgBox.critical(0,"Error", msg);
}











void MainWindow::on_actionRun_triggered(){
    this->play();
}

void MainWindow::on_actionPause_triggered(){
    this->pause();
}

void MainWindow::on_actionStop_Un_load_network_triggered(){
    this->stop();
}

void MainWindow::on_actionClear_screen_triggered(){
    if(!noTabYet()){
        tab()->removeAllBlocks();
    }
}

void MainWindow::on_tcpipButton_clicked(){
    if(this->backend_frame->isVisible())
        this->backend_frame->setVisible(false);
    else
        this->backend_frame->setVisible(true);
}
















void MainWindow::backend_restart(){
    // If there is another instance running, kill it..
    backend_process->kill();
    this->backend_write("Starting backend");
    backend_process->setWorkingDirectory(UserData::backend_path);
    QString program = "python BrainStdBE.py -port " + QString::number(UserData::tcpip_port);
    backend_process->start(program);
}

void MainWindow::backend_write(const QString &text){
    this->backend_terminal->append(text);
    this->backend_terminal->verticalScrollBar()->setValue(
                this->backend_terminal->verticalScrollBar()->maximum());
}

void MainWindow::backend_error(){
    QByteArray data = backend_process->readAllStandardError();
    //qDebug() << "BackendError:" << data;
    this->backend_write(data);
    //textEdit_verboseOutput->append(QString(data));
}

void MainWindow::backend_output(){
    QByteArray data = backend_process->readAllStandardOutput();
    this->backend_write(data);
    //qDebug() << "Backend:" << data;
    //textEdit_verboseOutput->append(QString(data));
}

void MainWindow::on_backendComboBox_activated(int index){
    if(index == 0){
        this->backend_restart();
        this->backend_stackedWidget->setCurrentIndex(0);
    }
    else if(index == 1){
        backend_process->kill();
        this->backend_write("Backend killed");
        this->backend_stackedWidget->setCurrentIndex(1);
    }
}

void MainWindow::on_pushButton_clicked(){
    this->backend_stackedWidget->setCurrentIndex(0);
}
