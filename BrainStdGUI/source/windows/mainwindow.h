#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>

#include <QtCore/QPointer>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtCore/QTime>
#include <QtCore/QTextStream>

#include <QtMultimedia/QSound>

#include "source/helper/userdata.h"
#include "source/helper/backendData.h"
#include "source/helper/tcpip_initialization.h"

// Created here:
#include "workspacetab.h"
#include "welcomewindow.h"
#include "aboutwindow.h"
#include "propertiesdialog.h"

// Pointers:

#include "ui_mainwindow.h"

enum {OFF, ON};

class WorkspaceTab;

class MainWindow : public QMainWindow, public Ui::MainWindow{
    Q_OBJECT
private:
    WelcomeWindow *welcomeWindow;
    AboutWindow *aboutWindow;
    PropertiesDialog *propertiesDialog;

    WorkspaceTab* tab() const { return (WorkspaceTab*)tabWidget->currentWidget(); }
    bool noTabYet() const;

    TCPIP_Init *tcpip_init;

protected:
    QPointer<QVBoxLayout> _layout;

    bool run;

    QMap<QString, WorkspaceTab*> workTab;

    int top_frame_separator;

    bool saveCustomConnections(QString foldername);

    void connectTabSignals(const QString &key);
    void disconnectTabSignals(const QString &key);
    void updateConnectionTable();


    void keyPressEvent(QKeyEvent * event);
    void resizeEvent(QResizeEvent * event);

public:
    explicit MainWindow(QString filename="", QWidget *parent = 0);
    ~MainWindow();

signals:
    void createNetFromFileSignal(QString _filename, bool _updateSchemaCheckBox);


public slots:
    // I don't know why but this has to be here in the header file otherwise
    // it is not recognized and connected.
    void removeTab(int index){
        // Find the key
        QString keyToRemove;
        QMap<QString, WorkspaceTab*>::const_iterator it;
        for(it = workTab.constBegin(); it != workTab.constEnd(); it++) {
            if (tabWidget->indexOf(it.value()) == index) {
                keyToRemove = it.key();
                break;
            }
        }
        this->removeTab(keyToRemove);
    }

private slots:
    void init(QString givenfilewithpath);
    //void newFile(){}
    void onNew();
    void onOpen();
    bool onSave();
    bool onSaveAs();
    bool onExportNetwork();
    bool onBackup();
    bool onOpenFolder();
    bool onCreateNetwork();
    void onPlay();
    bool onSetGrid(const bool &on);
    bool selectTabWithKey(const unsigned &number);
    bool loadNewTab(const QString &filename);
    void removeTab(const QString &path);
    void about(){}
    void onXmlToggle();
    void onActionsListToggle();
    void onExperimentToggle();
    void onPythonToggle();
    void onExit();

    bool play();
    bool pause();
    bool stop();

    friend class NewBlockWindow;

    void on_action_new_triggered();
    void on_action_save_triggered();
    void on_action_save_as_triggered();
    void on_action_load_triggered();
    void on_action_exit_triggered();
    void on_action_export_network_triggered();
    void on_action_backup_triggered();
    void on_action_about_triggered();
    void on_action_properties_triggered();
    void on_action_create_network_triggered();
    void on_action_grid_toggled(bool arg1);
    void on_actionXML_viewer_triggered();
    void on_actionActions_list_triggered();

    void on_saveButton_clicked();
    void on_playButton_clicked();
    void on_removeBlockButton_clicked();
    void on_CreateNetworkButton_clicked();
    void on_IPlineEdit_editingFinished();
    void on_PORTlineEdit_editingFinished();

    void on_learningCheckBox_clicked(bool checked){ /*A->applyLearning(checked);*/ }
    void on_xmlButton_clicked();
    void on_actionsListButton_clicked();

private slots:
    void workTabSchemaLoaded();
    void workTabNetworkLoaded();
    void workTabSchemaModifiedSlot();
    void workTabGetTime(QString time);
    void workTabGetInfo(QList<float> data);

    void on_openFolderButton_clicked();
    void on_rtNeuronSlider_sliderMoved(int position);
    void on_tabWidget_currentChanged(int index);

    void nodes_downloaded(QString data);
    void edges_downloaded(QString data);
    void error_workspaceTab_slot(QString msg);
    void on_actionRun_triggered();
    void on_actionPause_triggered();
    void on_actionStop_Un_load_network_triggered();
    void on_actionClear_screen_triggered();
    void on_tcpipButton_clicked();


    // -- Backends -------------------------------------------------------------
    void on_backendComboBox_activated(int index);

    void on_pushButton_clicked();

private:
    QProcess *backend_process;

    void backend_restart();
    void backend_write(const QString &text);

private slots:
    void backend_error();
    void backend_output();
    // -------------------------------------------------------------------------
};

#endif // MAINWINDOW_H









