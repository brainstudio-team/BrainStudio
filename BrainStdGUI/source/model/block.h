#ifndef BLOCK_H
#define BLOCK_H

#include <QtCore/QPoint>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>

#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <QtWidgets>

#include <iostream>

#include "textblock.h"
#include "source/graphs/graphs.h"
#include "source/helper/firingrate.h"
#include "source/helper/helperClasses.h"
#include "source/helper/backendData.h"

class Block : public QWidget{
    Q_OBJECT

private:
    int i, up_i, up_j;
protected:

    // -- ESSENTIAL PARAMETERS -------------------------------------------------
    // id, type, X, Y, width, height, colour
    QString type;
    QString version;
    int initial_x, initial_y;
    int WINDOW_X, WINDOW_Y;
    QString colour;
    bool spiking; // SOS: Returns true if the block has spikes!

    // -- REST PARAMETERS ------------------------------------------------------
    // Both block (fields) and unit (params) parameters
    QMap<QString,QString> params;
    QMap<QString,QString> states;
    QString param_for_neurons; // Parameter that holds the number of neurons    // ZAF SOS: I do not set it!
                               // Used to save the changes..!


    // Helper variables to show which is the first and which is the last neuron
    // of the network that this block contains.
    // Assumes that all neurons are neighnours..
    int firstNeuronIdx;
    int lastNeuronIdx;

    int stimulusFirst, stimulusLast;
    float stimulusAmount;
    float oscillationPhase, oscillationFrequency, oscillationAmplitude;

    // This is used in arch.window in order to change the QMap when id changes
    QString old_id;


    // -- STATES ---------------------------------------------------------------
    // Main state of the neurons of the system (updated from MainWindow)
    // Used for visualizations
    QVector<float> state;



    // -- EDGES ---------------------------------------------------------------- ZAF SOS: I think that this is only used in draw cons
    QVector<QString> blockConnections;
    QVector<int> blockConnectionSize;
    QVector<float> blockConnectionAverageWeight;
    QVector<int> neuronConnections;
    QVector<float> neuronConnectionWeights;



    // -- REST -----------------------------------------------------------------

    bool highlighted;   // Highlighted in the gui
    int mode;           // Visualization mode
    bool connecting;    // If this block is highlighted for a connection
    BasicGraph *graph;  //

    bool LFPcalculated;
    double LFP;

    // QWIDGET STUFF
    void paintEvent(QPaintEvent * event);
    void contextMenuEvent(QContextMenuEvent *event);
    QLineEdit *idLineEdit, *sizeLineEdit;
    //QComboBox *typeComboBox;
    QVBoxLayout *plotLayout;
    QFrame *plotFrame;

    QAction *newPlotAct;    // New plot
    QAction *propertiesAct; // Properties of block
    QAction *stimulationAct;// Stimulation of block
    QAction *newConAct;     // New connection
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *deleteAct;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

public:
    enum{ modeStatesPixels, modeEdit, modeC, modeRasters, modeStatesPlots,
          modeNetwork};

public:
    Block(QString _id, QString _type, bool _spiking,
          int _x=0, int _y=0, int _w =100, int _h=50, QString _col="#AAAAAA",
          int _size = 1, QWidget *parent=0);
    ~Block();

    FiringRate firingRateBlock;
    QVector<FiringRate> firingRateNeuron;

    QVector<NeuronUnit> neuronsLoc;

    void loadLocationsFromFile();
    void loadLocations(QVector<NeuronUnit> *locations);
    void loadLocations(const QString &filename);

    void id_exists(QString _old_id){ old_id = _old_id; idLineEdit->setText(old_id); }

    // ACCESSORS:
    QMap<QString,QString> getAllParams(){ return params; }
    QMap<QString,QString> getAllStates(){ return states; }
    QString getParam(const QString p_name) {if(params.contains(p_name)) return params[p_name]; else return "not found"; }
    QMap<QString,QString> getParams() { return params; }
    bool    containsParam(QString data){ return params.contains(data); }
    void    printParams(QTextStream &out){
                for(QMap<QString, QString>::iterator i=params.begin(); i != params.end(); i++)
                    out << "\t\t\t<" << i.key() << ">" << i.value() << "</"<< i.key() << ">\n";
            }

    QString getState(const QString s_name) {if(states.contains(s_name)) return states[s_name]; else return "not found"; }
    QMap<QString,QString> getStates() { return states; }
    bool    containsState(QString data){ return states.contains(data); }
    void    printStates(QTextStream &out){
                for(QMap<QString, QString>::iterator i=states.begin(); i != states.end(); i++)
                    out << "\t\t\t<" << i.key() << ">" << i.value() << "</"<< i.key() << ">\n";
            }

    bool    isSpiking() const { return spiking; }

    int     getInitialX() const { return initial_x; }
    int     getInitialY() const { return initial_y; }
    int     getFirstNeuronIdx() const { return firstNeuronIdx; }
    int     getLastNeuronIdx() const { return lastNeuronIdx; }

    bool    getConnecting() const {return connecting; }

    QString getVersion() const { return version; }
    QString getId() const { return idLineEdit->text(); }
    //QString getType() const { return typeComboBox->currentText(); }
    QString getType() const { return type; }
    QString getColour() const { return colour; }
    float   getState(int indx) const { if(indx<state.size())
                                            return state[indx]; else return 0;
                                          }
    int     getNeuronsSize() const {return state.size(); }
    float   getFiringRateNeuron(int indx) const {
                                        if(indx < firingRateNeuron.size())
                                          return firingRateNeuron[indx].getFR();
                                        else return 0.0f;}
    float   getFiringRateBlock() const { return firingRateBlock.getFR();}
    float   frequencyBlock() const { return firingRateBlock.getFR()*1000.0f;}
    QString getConnection(const int &i) const { return blockConnections[i]; }
    int     getConnectionSize(int i) const { return blockConnectionSize[i]; }
    int     getBlockConnectionAverageWeight(int i) const { return blockConnectionAverageWeight[i]; }
    int     getConnectionNo() const { return blockConnections.size(); }
    int     getNeuronConnection(int i) const { return neuronConnections[i]; }
    int     getNeuronConnectionWeight(int i) const { return neuronConnectionWeights[i]; }
    int     getNeuronConnectionNo() const { return neuronConnections.size(); }

    // MODIFIERS:
    void setAllParams(const QMap<QString,QString> &data){ params.clear();
        for(QMap<QString,QString>::const_iterator it=data.constBegin(); it!=data.constEnd(); it++)
              this->setParam(it.key(), it.value());}
    void setAllStates(const QMap<QString,QString> &data){ states.clear();
        for(QMap<QString,QString>::const_iterator it=data.constBegin(); it!=data.constEnd(); it++)
              this->setState(it.key(), it.value());}

    void setParam(const QString &p_name, const QString &value){
        if(p_name == param_for_neurons) this->sizeChanged(value);
        else{ params.insert(p_name, value); emit schemaModified(); }
    }
    void setParam(const QString &p_name, const int &value)   { this->setParam(p_name, QString::number(value)); }
    void setParam(const QString &p_name, const float &value) { this->setParam(p_name, QString::number(value)); }
    void setParam(const QString &p_name, const double &value){ this->setParam(p_name, QString::number(value)); }
    void clearParams(){ params.clear(); emit schemaModified(); }

    void setState(const QString &p_name, const QString &value){ states.insert(p_name, value); emit schemaModified(); }
    void setState(const QString &p_name, const int &value)   { this->setState(p_name, QString::number(value)); }
    void setState(const QString &p_name, const float &value) { this->setState(p_name, QString::number(value)); }
    void setState(const QString &p_name, const double &value){ this->setState(p_name, QString::number(value)); }
    void clearStates(){ states.clear(); emit schemaModified(); }

    void setVersion(const QString &_value){ version = _value; }
    void setType(const QString &_value){ type = _value; }
    void setNeuronMemPot(int indx, float mp){if(indx<state.size())state[indx]=mp;}
    void setSpikeArray(unsigned *spikesArray, const int spikes);
    void setFirstNeuronIdx(int value){ firstNeuronIdx = value; }
    void setLastNeuronIdx(int value){ lastNeuronIdx = value; }
    void setFiringRateDepth(const int &value);

    // -- STIMULUS -------------------------------------------------------------
    void setStimulus(const float &value, const int &first, const int &last){stimulusAmount=value;stimulusFirst=first;stimulusLast=last;}
    void setStimulus(const float &value){ stimulusAmount = value; stimulusFirst=0; stimulusLast=state.size();}
    void setStimulusFirst(const int &value){ stimulusFirst = value; }
    void setStimulusLast(const int &value){ stimulusLast = value; }
    void setStimulusAmount(const float &value){ stimulusAmount = value; }
    void setOscillationPhase(const float &value){ oscillationPhase = value; }
    void setOscillationFrequency(const float &value){ oscillationFrequency = value; }
    void setOscillationAmplitude(const float &value){ oscillationAmplitude = value; }

    int getStimulusFirst(){ return stimulusFirst; }
    int getStimulusLast(){ return stimulusLast; }
    float getStimulusAmount(){ return stimulusAmount; }
    float getOscillationPhase(){ return oscillationPhase; }
    float getOscillationFrequency(){ return oscillationFrequency; }
    float getOscillationAmplitude(){ return oscillationAmplitude; }
    // -------------------------------------------------------------------------


    // SOS: Probably only architecture should call this.
    void setHighlighted(bool value){ highlighted = value; }
    void setMode(int value);
    void setConnecting(const bool& value){ connecting = value; }


    // METHODS:
    void updateRaster();
    void updateMe(); // Whatever it needs to be updated in every itteration..
    bool in(int x2, int y2){    if(x() <= x2 && x2 <= (x()+width()) )
                                    if(y() <= y2 && y2 <= (y()+height()) )
                                        return true;
                                return false;
                           }
    bool containsNeuron(int neuronIdx){
       if(neuronIdx >= firstNeuronIdx && neuronIdx < lastNeuronIdx)return true;
       else return false; }
    void moveX(int _x){ initial_x +=_x; this->moveAll(); } //*/this->move(this->x()+_x,this->y()); }
    void moveY(int _y){ initial_y +=_y; this->moveAll(); } //*/this->move(this->x(),this->y()+_y); }
    void moveXY(int _x, int _y){ initial_x +=_x; initial_y +=_y; /*this->moveAll(); } //*/this->move(this->x()+_x,this->y()+_y); }
    void addNeuron(float value){ state.push_back(value);
                                 firingRateNeuron.push_back(0);
                                 /*spikes.push_back(0) to be deleted;*/
                               }
    void deleteStates(){ state.clear(); firingRateNeuron.clear(); }
    QColor getNeuronColour(int indx);
    QColor getFiringRateNeuronColour(int indx);
    QColor getFiringRateBlockColour(const int &targetBlock);
    QList<float> getWindowedFiringRate();
    void addConnection(const QString &indx, const float &weight){
            //std::cout << "ADD BLOCK(" << indx << ")" << std::endl;
            if(!blockConnections.contains(indx)){
                //std::cout<<"------BLOCK ADDED "<<indx<<std::endl;
                blockConnections.append(indx);
                blockConnectionSize.append(1);
                blockConnectionAverageWeight.append(weight);
            }
            else{
                blockConnectionAverageWeight[blockConnections.indexOf(indx)] =
                  (blockConnectionAverageWeight[blockConnections.indexOf(indx)]*
                    blockConnectionSize[blockConnections.indexOf(indx)]+weight)/
                        (blockConnectionSize[blockConnections.indexOf(indx)]+1);
                blockConnectionSize[blockConnections.indexOf(indx)]++;
            }
        }
    void addNeuronConnection(const int &indx, const float &weight){
            //std::cout << "ADD NEURON(" << indx << ")" << std::endl;
            if(!neuronConnections.contains(indx)){
                //std::cout<<"------NEURON ADDED "<<indx<<std::endl;
                neuronConnections.push_back(indx);
                neuronConnectionWeights.push_back(weight);
            }
        }
    void deleteConnections(){
            neuronConnections.clear();
            neuronConnectionWeights.clear();
            blockConnections.clear();
            blockConnectionSize.clear();
            blockConnectionAverageWeight.clear();
        }

    bool fired(int neuron) const { if(neuron < firingRateNeuron.size())
                                       return firingRateNeuron[neuron].fired();
                                   qDebug() << "Block::fired: Error: Target "
                                               "out of limmit"; return false; }

    void setWINDOW_X(const int &value){ WINDOW_X = value; this->moveAll(); }
    void setWINDOW_Y(const int &value){ WINDOW_Y = value; this->moveAll(); }

    void moveAll(){ this->move(initial_x + WINDOW_X, initial_y + WINDOW_Y); }

    int centreX() const { return x() + width()/2; }
    int centreY() const { return y() +height()/2; }

private slots:
    void newPlotSlot(){ emit newPlotSignal(this->idLineEdit->text()); }
    void stimulationSlot(){ emit stimulationSignal(); }
    void propertiesSlot(){ emit doubleClickSignal(this->idLineEdit->text()); }
    void newConnectionSlot(){ connecting=true; emit newConnectionSignal(this->idLineEdit->text()); }
    void deleteBlockSlot(){emit deleteBlockSignal(idLineEdit->text()); }
    void idChanged(){ emit idChangedSignal(old_id, idLineEdit->text()); old_id = idLineEdit->text(); }
    void typeChanged(int curr_index){ emit schemaModified(); }
    void sizeChanged(const QString &new_size){ state.resize(new_size.toInt());
                                      firingRateNeuron.resize(new_size.toInt());
                                      stimulusLast = new_size.toInt();
                                      params[param_for_neurons] = new_size;
                                      emit schemaModified();
                                        }

signals:
    void schemaModified();
    void newConnectionSignal(QString id); // current id
    void deleteBlockSignal(QString id);
    void idChangedSignal(QString old_id, QString new_id);
    void highlighted_signal(QString id);
    void init_block_motion_signal(int x, int y);
    void init_block_resize_signal(QPoint, QString);
    void newPlotSignal(QString id);
    void doubleClickSignal(QString id);
    void stimulationSignal();

    // COPY/PASTE STUFF
private slots:
    void cut(){ emit signal_cut(this->idLineEdit->text());}
    void copy(){ emit signal_copy(this->idLineEdit->text());}
signals:
    void signal_cut(QString);
    void signal_copy(QString);
};

#endif // BLOCK_H

















