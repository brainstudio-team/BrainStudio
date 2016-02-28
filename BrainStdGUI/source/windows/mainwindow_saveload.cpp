#include "mainwindow.h"


// Connect Slots/Signals
void MainWindow::connectTabSignals(const QString &key){
    if(!workTab.contains(key))
        return;
    connect(workTab[key], SIGNAL(workTabSchemaLoaded()),
            this,             SLOT(workTabSchemaLoaded()));
    connect(workTab[key], SIGNAL(workTabNetworkLoaded()),
            this,             SLOT(workTabNetworkLoaded()));
    connect(workTab[key], SIGNAL(timeSignal(QString)),
            this,             SLOT(workTabGetTime(QString)));
    connect(workTab[key], SIGNAL(infoSignal(QList<float>)),
            this,             SLOT(workTabGetInfo(QList<float>)));
    connect(workTab[key], SIGNAL(schemaModifiedSignal()),
            this,             SLOT(workTabSchemaModifiedSlot()));
    connect(workTab[key], SIGNAL(simulationErrorSignal(QString)),
            this,             SLOT(error_workspaceTab_slot(QString)));
}

// Disconnect Slots/Signals
void MainWindow::disconnectTabSignals(const QString &key){
    if(!workTab.contains(key))
        return;
    disconnect(workTab[key], SIGNAL(workTabSchemaLoaded()),
               this,             SLOT(workTabSchemaLoaded()));
    disconnect(workTab[key], SIGNAL(workTabNetworkLoaded()),
               this,             SLOT(workTabNetworkLoaded()));
    disconnect(workTab[key], SIGNAL(timeSignal(QString)),
               this,             SLOT(workTabGetTime(QString)));
    disconnect(workTab[key], SIGNAL(infoSignal(QList<float>)),
               this,             SLOT(workTabGetInfo(QList<float>)));
    disconnect(workTab[key], SIGNAL(schemaModifiedSignal()),
               this,             SLOT(workTabSchemaModifiedSlot()));
    disconnect(workTab[key], SIGNAL(simulationErrorSignal(QString)),
               this,             SLOT(error_workspaceTab_slot(QString)));
}

// This is called when I press one key in the numpad
bool MainWindow::selectTabWithKey(const unsigned &number){
    if(number >= MAX_FILES){
        qDebug() << "MainWindow::loadTab: Warning: The maximum number of tabs "
                    "has been reached!";
        return false;
    }

    // If we are calling the same tab, do nothing
    if(number == tabWidget->currentIndex()){
        return true;
    }
    // If we are calling a different tab that already exists
    else if(number < tabWidget->count()){
        // Just go to that tab
        tab()->disableControls();
        tabWidget->setCurrentIndex(number);
        tab()->enableControls();
    }
    // If we are calling a different tab that has nothing open the corresponding
    // schemaXXX.brn file from the workspace.
    // This will become the last opened files.
    else{
        QString file = "schema" + QString::number(number + 1) + ".brn";
        if(!loadNewTab(file)){
            qDebug() << "MainWindow::loadTab: Error: Failed to open tab!";
            return false;
        }
    }
    return true;
}

void MainWindow::on_tabWidget_currentChanged(int index){
    QMap<QString,WorkspaceTab*>::iterator i;
    for(i=workTab.begin(); i != workTab.end(); i++)
        i.value()->disableControls();

    tab()->enableControls();
}


bool MainWindow::loadNewTab(const QString &filename){

    QString test_file = filename;
    if(test_file.size() > 0 && test_file[0] != '/')
        test_file = UserData::workspace_path + "/" + test_file;
    if(!QFile::exists(test_file)){
        qDebug() << "MainWindow::loadTab: File" << filename
                 << "does not exist!";
        return false;
    }

    if(!noTabYet())
        tab()->disableControls();

    qDebug() << "MainWindow::loadTab: Creating a new tab:" << filename;
    FILENAME = filename;

    // CREATE NEW TAB
    this->removeTab(filename);

    workTab[filename] = new WorkspaceTab(FILENAME, this);
    if(workTab[filename]->failed()){
        qDebug() << "MainWindow::loadTab: Creating a new tab:" << filename;
        return false;
    }

    tabWidget->addTab(workTab[filename], filename);
    tabWidget->setCurrentWidget(workTab[filename]);
    connectTabSignals(filename);

    return true;
}

void MainWindow::removeTab(const QString &filename){
    if(workTab.contains(filename)){
        disconnectTabSignals(filename);
        delete workTab[filename];
        workTab.remove(filename);
    }
}








































//
//
//                      T O    D E L E T E    ! ! !
//
//
void MainWindow::on_actionActivated_triggered(bool checked){
}
























