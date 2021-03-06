#ifndef ARCHITECTUREWINDOW_H
#define ARCHITECTUREWINDOW_H

#include <QWidget>
#include <QtEvents>
#include <QList>
#include <QPainter>
#include <QColor>
#include <QMapIterator>

#include <math.h>

//#include <stdlib.h>
//#include <qapplication.h>
#include <qprocess.h>
#include <qstringlist.h>

#include "source/windows/stimulationwidget.h"
#include "source/windows/rightpanel.h"
#include "source/simulation/simulator.h"

#include "source/model/block.h"
#include "source/model/connection.h"

#include "source/analysis/mutualinformation.h"
#include "source/analysis/synchrony.h"
#include "source/analysis/effectiveconnectivitycalculator.h"

class StimulationWidget;
class RightPanel;

class Controls{
public:
    explicit Controls(const int _speed = 100):
                      speed(_speed),
                      updateSchema(true),
                      savable(false), network(NET_NONE){}

    enum {NET_NONE, NET_MI, NET_TE, NET_SYNC};

    // From XML file:
    int speed;

    // From GUI:
    bool updateSchema;
    bool savable;
    int network;
};

class Action{
public:
    Action(const int &_time=0, const QString &_type="", const QString &_node="",
           const double &_baseline=0.0, const double &_amplitude=0.0,
           const int&_first=0, const int&_last=0,
           const QString &_parameter="",
           const double &_frequency=0.0, const int &_phase=0):
                time(_time), type(_type), node(_node),
                new_value(_baseline), amplitude(amplitude),
                firstNeuron(_first), lastNeuron(_last),
                parameter(_parameter), frequency(_frequency), phase(_phase){}
    // Used in all actions
    int time;
    QString type;
    QString node;

    // Used in adjust and stimulate
    double new_value; // Either stimulus or value of a parameter
    int firstNeuron;
    int lastNeuron;

    // Used in stimulate
    double phase;
    double frequency;
    double amplitude;

    QString parameter;
};

namespace Ui {
class ArchitectureWindow;
}

class ArchitectureWindow : public QWidget{
    Q_OBJECT
private:
    int k,radius;
    float frSUM;
    QColor c1;
    float freq;
    QString oscMode;
    bool _showActions;
    int x1, y1, x2, y2, xA, yA, xB, yB;
    bool resize_bottom, resize_right, resize_bottomright;
    Ui::ArchitectureWindow *ui;

protected:
    StimulationWidget *stimWidget;
    RightPanel *rightPanel;
    Simulator *A;

    MutualInformation *mi;
    Synchrony *sync;
    QMap<QString, EffectiveConnectivityCalculator> ec_calculator;

    bool grid;
    int GRID_SIZE;
    int GRID_WIDTH;
    int GRID_offset_X;
    int GRID_offset_Y;
    int WINDOW_X;   // The value that schema's X coordinate is translated
    int WINDOW_Y;   // The value that schema's Y coordinate is translated
    int mode;
    bool details, show_connections;
    QPoint connectingCursor;
    QString highlighted;
    int zoom;
    bool under_modification;

    QRect selection_box;

    // Variables needed to form new connections and visualize the process ------
    bool connecting; // We are during a new connection
    QString con_target; // The source of this new connection
    QString con_source; // Potential target (for visualization)

    int xStart, yStart; // The initial coordinates when moving a node
    int xRelease, yRelease; // Where the mouse was during the last release

    bool SIMULATION;

    void loadFirstLastNeurons();

    void drawConnections(QPainter &painter);
    void drawConnection(QPainter &painter, const QString &bl1, const QString &bl2);
    void drawLeftToRight(QPainter &painter, const QString &bl1, const QString &bl2);
    void drawRightToLeft(QPainter &painter, const QString &bl1, const QString &bl2);
    void drawTopToDown(QPainter &painter, const QString &bl1, const QString &bl2);
    void drawDownToTop(QPainter &painter, const QString &bl1, const QString &bl2);

    void drawConnection(QPainter &painter, const QString &bl1, const QPoint &tar);
    void drawLeftToTarget(QPainter &painter, const QString &bl1, const QPoint &tar);
    void drawRightToTarget(QPainter &painter, const QString &bl1,const QPoint &tar);
    void drawTopToTarget(QPainter &painter, const QString &bl1, const QPoint &tar);
    void drawDownToTarget(QPainter &painter, const QString &bl1, const QPoint &tar);


    void paintEvent(QPaintEvent * event);
    void resizeEvent(QResizeEvent * event );
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void wheelEvent(QWheelEvent *event);

    QAction *newBlockAct;
    QAction *newTextBlockAct;
    QAction *pasteAct;

    QPoint menuPos;


private: // BRN PARSER HELPER FUNCTIONS
    QMap<QString, QString> parseInternalElement(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml, QMap<QString, QString>& map) const;

    void addNodes(QList< QMap<QString,QString> >& data);
    void addEdges(QList< QMap<QString,QString> >& data);
    void addTextBlocks(QList< QMap<QString,QString> >& data);
    void addActions(QList< QMap<QString,QString> >& data);

    void helper1(QString name, QList< QMap<QString,QString> > &structure,
                                                         QXmlStreamReader &xml);
    void parse_actions(QList< QMap<QString,QString> > &structure,
                                                         QXmlStreamReader &xml);

public:

    bool load_brn(const QString filename);
    bool save_brn(const QString filename);
    bool backup(QString foldername);


    //bool savePythonBrian();
    bool checkValues(int _model, int _type, float *A);

public:
    explicit ArchitectureWindow(QWidget *parent = 0);
    ~ArchitectureWindow();

    void updateSimulationPointer(Simulator *_A){
        A = _A;
    }


    // Widget stuff
    void setModeStatesPixels(){mode = Block::modeStatesPixels; setBlockMode(Block::modeStatesPixels); update();}
    void setModeEdit(){    mode = Block::modeEdit; setBlockMode(Block::modeEdit); update();}
    void setModeDefault(){ mode = Block::modeDefault; setBlockMode(Block::modeDefault); update();}
    void setModeC(){mode = Block::modeC; setBlockMode(Block::modeC); update();}
    void setModeRasters(){ mode = Block::modeRasters; setBlockMode(Block::modeRasters); update();}
    void setModeStatesPlots(){mode = Block::modeStatesPlots; setBlockMode(Block::modeStatesPlots); update();}
    void setModeStatesPhases(){mode = Block::modeStatesPhases; setBlockMode(Block::modeStatesPhases); update();}
    void setModeNetwork(){mode = Block::modeNetwork; setBlockMode(Block::modeNetwork); update();}
    void setBlockMode(int value);
    void setDetails(bool value){ details = value; update(); }
    void setZoom(int value){ zoom = value; update(); }
    void setNetworkMode(const int &mode);
    void setWINDOW_X(int value){ WINDOW_X = value; update(); }
    void setWINDOW_Y(int value){ WINDOW_Y = value; update(); }
    void setGrid(bool on){ grid = on; }

    void showActions(){ _showActions = true; }
    void hideActions(){ _showActions = false; }

    void addBlock(Block *block);
    bool addBlock(QString _name, QString _type, int _x, int _y,
                  int _w, int _h, QString _col, QString _vis, int _neuronsNo);

    void addConnection(Connection temp){
        connections.append(temp);
    }

    void clear(){
        for(BlockIter bl = blocks.begin(); bl!=blocks.end(); bl++)
            delete bl.value();
        this->blocks.clear();
        this->connections.clear();
        this->textBlocks.clear();
        this->actions.clear();
        this->LFPindex.clear();
        this->LFPvalue.clear();
    }

    // Architecture stuff
public:
    typedef QMap<QString,Block*> BlockMap;
    typedef BlockMap::iterator BlockIter;

    QMap<QString,Block*> blocks;
    QList<Connection> connections;
    QList<TextBlock> textBlocks;
    QVector<Action> actions;
    QVector<Action> actionsDone;

    // Used to keep track of how many edges each node has, so we know in which
    // order to illustrate them.
    QMap<QString, int > edges_stack_a;
    QMap<QString, int > edges_stack_b;
    QMap<QString, int > edges_stack_rec;

    // Simulation stuff
protected:
    Controls controls;

public:
    int     getSpeed() const { return controls.speed; }
    bool    updateSchemaIsOn() const { return controls.updateSchema; }
    bool    isSavable() const { return controls.savable; }
    QString getHighlighted() const { return highlighted; }

    void setSpeed(const int &value){ controls.speed = value; }
    void setModified(){ under_modification = false; emit modifiedSignal(); }
    void setUpdateSchema(const bool &value){ controls.updateSchema = value; }

    bool modeStatesPixels(){    if(mode == Block::modeStatesPixels) return true; return false; }
    bool modeEdit(){            if(mode == Block::modeEdit) return true; return false; }
    bool modeDefault(){         if(mode == Block::modeDefault) return true; return false; }
    bool modeC(){               if(mode == Block::modeC) return true; return false; }
    bool modeRasters(){         if(mode == Block::modeRasters) return true; return false; }
    bool modeStatesPlots(){     if(mode == Block::modeStatesPlots) return true; return false; }
    bool modeStatesPhases(){    if(mode == Block::modeStatesPhases) return true; return false; }
    bool modeNetwork(){         if(mode == Block::modeNetwork) return true; return false; }

public:

    // ???
    QList<QString> LFPindex;
    QMap<QString,double> LFPvalue;

    void setFiringRateDepth(const int &value);
    QString in(int x2, int y2); // Returns "" if not found
    void update_f_rates( unsigned int *spikesArray, int spikes);
    void update_effective_connectivity();

    bool isEmpty(){ return blocks.isEmpty(); }

    bool addLFPcounter(const QString &index);
    //void calcLocalFieldPotentials();

    void turnSIMULATION(bool on);

    void endConnection(Connection edge);

    void restartActions();

private slots:
    void schemaDoubleClickSlot(){ emit schemaDoubleClick(menuPos); }
    void blockDoubleClickSlot(QString id){ emit blockDoubleClick(id); }
    void blockNewPlotSlot(QString id){ emit blockNewPlot(id); }
    void schemaCreateTextBlockSlot(){ emit schemaCreateTextBlock(menuPos); }
    void schemaModifiedSlot();
    void startConnectionSlot(QString id);

    void on_horizontalScrollBar_valueChanged(int value);
    void on_verticalScrollBar_valueChanged(int value);

    void init_block_motion_slot(int x, int y);
    void init_block_resize_slot(QPoint point, QString direction);
    void blockIdChangedSlot(QString old_id, QString new_id);

public slots:
    void deleteRedundantConnections();
    void deleteBlockSlot(const QString &id);
    void deleteAllBlocks();

    void mouseMoveFromPlotSlot(QMouseEvent *event, QPoint pos){
        QMouseEvent a(QEvent::MouseMove, event->pos() + pos, Qt::NoButton,
                                 event->buttons(), event->modifiers());
        this->mouseMoveEvent(&a);
    }

public slots:
    bool setHighlighted(QString id); // SOS: returns the source of a connection!
    bool setGroupHighlighted(QString id);
    void turnOnStimulation();
    void turnOffStimulation();

signals:
    void schemaDoubleClick(QPoint point);
    void blockDoubleClick(QString id);
    void blockNewPlot(QString id);
    void schemaCreateTextBlock(QPoint point);
    void schemaAddingNewConnection(QString source, QString target);
    void modifiedSignal();
    void showMessageSignal(const QString &text, int timeout);

    // COPY/PASTE STUFF
private:
    QList<QString> clipboard;
public slots:
    void cut();
    void copy();
    void paste();
    void select_all();

    void keyUP();
    void keyDOWN();
    void keyLEFT();
    void keyRIGHT();
};

#endif // ARCHITECTUREWINDOW_H
