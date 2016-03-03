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

    QString path = filename;
    if(!path.contains('\\') && !path.contains('/'))
        path = UserData::workspace_path + QDir::toNativeSeparators("/") + path;
    path = QDir::toNativeSeparators(path);

    if(!QFile::exists(path)){
        QString error = "File " + path + " does not exist!";
        qDebug() << "MainWindow::loadNewTab:" << error;
        QMessageBox::critical(this, "Error in loading", error, QMessageBox::Ok);
        return false;
    }

    if(!noTabYet())
        tab()->disableControls();


    qDebug() << "MainWindow::loadTab: Creating a new tab:" << filename <<
                "path:" << path;

    QString brn_filename = path.split(QDir::toNativeSeparators("/"),
                                      QString::SkipEmptyParts).last();

    // REMOVE PREV TAB
    this->removeTab(path);

    // CREATE NEW TAB

    workTab[path] = new WorkspaceTab(path, this);
    if(workTab[path]->failed()){
        qDebug() << "MainWindow::loadTab: Creating a new tab:" << brn_filename;
        return false;
    }

    tabWidget->addTab(workTab[path], brn_filename);
    tabWidget->setCurrentWidget(workTab[path]);
    connectTabSignals(path);

    return true;
}

void MainWindow::removeTab(const QString &path){
    if(workTab.contains(path)){
        disconnectTabSignals(path);
        delete workTab[path];
        workTab.remove(path);
    }
}

