#ifndef WORKSPACETAB_H
#define WORKSPACETAB_H

#include <QtGui>
#include <QThread>
#include <QtWidgets/QMainWindow>

#include "source/windows/xmlwindow.h"
#include "source/windows/experimentwidget.h"
#include "source/windows/pythonwidget.h"
#include "source/windows/architecturewindow.h"
#include "source/windows/plotwindow.h"
#include "source/windows/blockpropertieswidget.h"
#include "source/windows/controlswidget.h"
#include "source/windows/newblockwindow.h"
#include "source/windows/newconnectionwindow.h"

#include "source/simulation/simulator.h"
#include "source/simulation/tcpip.h"

#include "source/helper/userdata.h"


// Because we declaire also workspacetab in Controlswidget because we call stuff
class ControlsWidget;

namespace Ui {
class WorkspaceTab;
}

class WorkspaceTab : public QMainWindow{
    Q_OBJECT

private:
    Ui::WorkspaceTab *ui;

    // -- Windows associated with this tab -------------------------------------
    ArchitectureWindow *schema;
    XmlWindow *xmlWindow;
    QListWidget *actionsList;
    ExperimentWidget *experimentWidget;
    PythonWidget *pythonWidget;

    NewBlockWindow *addBlockWindow;
    NewConnectionWindow *addConnectionWindow;

    QList<PlotWindow*> plotWindows;
    QList<BlockPropertiesWidget*> blockPropertiesWindows;

    // Just pointers (do not get destructed when tab gets)
    ControlsWidget *controls;
    // -------------------------------------------------------------------------

    // Frames from main window
    QFrame *propertiesFrame;
    QFrame *controlsFrame;
    QFrame *activityFrame;


    // Docs for the above windows
    QDockWidget *xmlDock;
    QDockWidget *actionsListDock;
    QDockWidget *experimentDock;
    QDockWidget *pythonDock;
    QList<QDockWidget*> plotWindowsDock;
    QMap<QString,QDockWidget*> blockPropertiesWindowsDock;

    Simulator *snn;
    QString FILENAME;

    void setBackend();

private :
    // Time
    int sec;
    double f_ms, ms;
    QString timeLabelStr;





    bool loading_failed; // Becomes true if parsing the xml file has failed..

private:
    bool loadAll(); // Loads schema and all child (control or docking) widgets
    void refreshActionList();
    void createDockWindows();
    void updateTime();
    bool checkForAction();
    void addControlsWidget();
    void calcLocalFieldPotentials();

    void closeEvent(QCloseEvent * event);
    void resizeEvent(QResizeEvent * event );

public:
    explicit WorkspaceTab(QString filename, QWidget *parent = 0);
    ~WorkspaceTab();

    // PUBLIC METHODS
    bool create_network();
    bool network_is_running();
    bool save();
    bool save(QString path);
    bool exportNetwork();
    bool backup();
    bool loadXML();
    bool saveXML(){ return false; }
    bool play();
    bool pause();
    bool stop();
    void setMode(const int &_mode);
    void setGrid(const bool &on);
    bool changeSimulator(const int &simulator); // setSimulator(QString)????
    bool addConnection();
    bool removeConnection();
    bool addBlock();
    bool removeBlock();
    bool removeAllBlocks();
    void setTimeSpeed(const int &value);
    void disableControls();
    void enableControls();
    bool failed(){ return loading_failed; }

    // ACCESSORS
    //QObject *getXMLWindowPointer() const { return xmlWindow; }
    //QObject *getSchemaPointer() const { return schema; }
    QString filename() const { return FILENAME; }
    bool    xmlVisible() const { return xmlDock->isVisible(); }
    bool    actionsListVisible() const { return actionsListDock->isVisible(); }
    bool    experimentVisible() const { return false; }
    bool    pythonVisible() const { return false; }
    int     getSpeed() const { if(schema != NULL) return schema->getSpeed(); return-1;}

    // MODIFIERS
    void setFilename(const QString &value){ FILENAME = value; }

    void setSpeed(const int &value){ if(schema!=NULL) schema->setSpeed(value); }

private slots:
    void updateBot();
    void receiveProgress(int value, QString stage);
    void networkLoaded(Simulator *snn_input, int *firstId, int *lastId, int idSize);
    void experimentUpdate(QString msg);
    void experimentEndOfCycle(float p, float gp);
    void experimentFinished();
    void schemaDoubleClick(QPoint point);
    void blockDoubleClick(QString id);
    void blockNewPlot(QString id);
    void schemaCreateTextBlock(QPoint point);
    void schemaAddingNewConnection(QString source, QString target);
    void plotWindowDeleteRequest(int index);

    //void on_horizontalScrollBar_valueChanged(int value){ schema->setWINDOW_X(value); }
    //void on_verticalScrollBar_valueChanged(int value){ schema->setWINDOW_Y(value); }

    void loadTempSchema_xmlSlot();
    void schemaModifiedSlot();

public slots:
    void toggleXML();
    void toggleActionsList();
    void toggleExperiment();
    void togglePython();

    void buildPython(QString simulator);
    void showMessage(const QString &text, int timeout);
    void simulationErrorSlot(QString msg){ emit simulationErrorSignal(msg); }

signals:
    void workTabSchemaLoaded();
    void workTabNetworkLoaded();
    void timeSignal(QString time);
    void infoSignal(QList<float> data);
    void toggleXMLsignal();             // ZAF: Will be used to change the button on MainWindow
    void toggleActionsListSignal();     // ZAF: Will be used to change the button on MainWindow
    void toggleExperimentSignal();      // ZAF: Will be used to change the button on MainWindow
    void togglePythonSignal();          // ZAF: Will be used to change the button on MainWindow
    void schemaModifiedSignal();
    void simulationErrorSignal(QString msg);

public slots:
    void cut(){schema->cut();}
    void copy(){schema->copy();}
    void paste(){schema->paste();}
    void select_all(){schema->select_all();}

    void keyUP(){schema->keyUP();}
    void keyDOWN(){schema->keyDOWN();}
    void keyLEFT(){schema->keyLEFT();}
    void keyRIGHT(){schema->keyRIGHT();}
};

#endif // WORKSPACETAB_H








































