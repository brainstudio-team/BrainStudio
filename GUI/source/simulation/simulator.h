/*
Interface for different classes of backends that might used in BrainStudio.
Although the default functionality provides only a tool to connect to
other backends-programs via tcpip, this interface can be used to create
an embedded backend.
*/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "source/helper/helperClasses.h"
#include "source/helper/userdata.h"
#include "source/helper/stimulus.h"

#include <iostream>
#include <QWidget>
#include <QMap>

enum {SYN_DEFAULT, SYN_SIMPLE_CHEMICHAL};

class Simulator : public QObject{
    Q_OBJECT

protected:
    double randBig(const double &var){
        return var*(((double)(rand()%1000))/1000.0);
    }
    float randBig(const float &var){
        return var*(((float)(rand()%1000))/1000.0f);
    }

// -- TCPIP stuff --------------------------------------------------------------
signals:
    void backend_connected();
    void simulation_loaded(Simulator*,int*,int*,int);
    void step_done();
    void error_signal(QString msg);


public:
    virtual void stop(){ std::cout << "Not in this simulation\n"; }
    virtual void pause(){ std::cout << "Not in this simulation\n"; }
    virtual void play(){ std::cout << "Not in this simulation\n"; }

    //virtual void loadXML(QString path){std::cout<<"Not in this simulation\n";}

// -----------------------------------------------------------------------------

protected:
    double timeStep;
    double dt;
    bool initialized;
    unsigned *spikesArray; // Spikes
    QVector<float> neuronsState;  // Values of main variable (state of neurons)
    size_t spikes;
    int synapticModel;

    // STIMULATION
    QMap<QString,Stimulus> stimuli;

    // Requests - Requesting states and parameters that will return after a step
    QMap<QString, QList< QString > > requested_states;
    QMap<QString, QList< QString > > requested_parameters;


    // ACTIONS
protected:
    QList<QString> action_buffer;
public:
    void add_action(const QString &action){ action_buffer.append(action); }

public:
    explicit Simulator(double _dt = 1.0, QObject *parent = 0):
        QObject(parent),dt(_dt){spikesArray=NULL; restartNetwork();}
    virtual ~Simulator(){
    }

    virtual bool restartNetwork(){
        synapticModel = SYN_DEFAULT;
        spikes = 0;
        timeStep = 0.0;
        // SOS: I think that nemo deletes this!!
        // if(spikesArray != NULL) delete [] spikesArray;
        spikesArray = NULL;
        stimuli.clear();
        initialized = false;
        return false;
    }

    void restart_requesting(){
        requested_states.clear();
        requested_parameters.clear();
    }

    void request_state(QString node_name, QString state_name){
        requested_states[state_name].append(node_name);
    }

    // LOCAL FIELD POTENTIAL
    virtual double getLocalFieldPotential(const int &first, const int &last){ (void)first; (void)last; return 0.0;}

    // SIMULATION
    virtual void initializeFromFile(QString filename){ (void)filename; std::cout << "Not in this simulation\n"; }
    virtual void step(const int &speed) = 0;
    virtual void trainingStep(){ std::cout << "Not in this simulation\n"; }
    virtual void scriptTrainingStep(){ std::cout << "Not in this simulation\n"; }

    // ACCESSORS
    double getTimeStep(){ return timeStep; }
    double getDt(){ return dt; }
    int getSpikes(){ if(initialized) return (int)spikes; return 0; }
    unsigned getSpikesArray(int index){ if(initialized) return spikesArray[index]; return 0; }
    unsigned* getSpikesArray(){ if(initialized) return spikesArray; return NULL; }


    // VIRTUUAL ACCESSORS
    virtual int   getNeurons() = 0;
    virtual int   getSynapses() = 0;
    virtual int   getSynapses(int indx){ (void)indx; return 0; }
    virtual int   getSynapse(int neuron, int synapseIndx){ (void)neuron; (void)synapseIndx; return 0; }
    virtual float getSynapseWeight(const unsigned &neuron, const unsigned &synapseIndx){ (void)neuron; (void)synapseIndx; return 0.0f;}


    virtual float getNeuronState(unsigned int index) = 0; // This is used by the gui
    virtual bool  isInitialized(){ return initialized; }
    virtual bool  isInitializing(){ return false; }
    virtual bool  learningInitialized(){ return false; }

    virtual float getNeuronLocX(int idx){ (void)idx; return 0.0f; }
    virtual float getNeuronLocY(int idx){ (void)idx; return 0.0f; }
    virtual float getNeuronLocZ(int idx){ (void)idx; return 0.0f; }

    virtual double getNeuronInput(const unsigned &idx){ (void)idx; return -7777.0; }
    virtual double getNeuronI(const unsigned &idx){ (void)idx; return -7777.0; }
    virtual double getNeuronStimulus(const unsigned &idx){ (void)idx; return -7777.0; }

    // GET REAL NEURON PARAMETERS
    virtual float getRealParameter(const unsigned &index, const unsigned &param){(void)index; (void)param; return 0.0f; }
    virtual float getRealNeuronStateU(const unsigned &index){ (void)index; return 0.0f;}
    virtual float getRealNeuronStateV(const unsigned &index){ (void)index; return 0.0f;}
    virtual float getRealNeuronState(const unsigned &index, const unsigned &param){ (void)index; (void)param; return 0.0f; }

    // GET REAL SYNAPSE PARAMETERS
    virtual unsigned int  getRealSynapseSource(const unsigned int &index){ (void)index; return 0; }
    virtual unsigned int  getRealSynapseTarget(const unsigned int &index){ (void)index; return 0; }
    virtual double getRealSynapseDelay(const unsigned int &index){ (void)index; return 0; }
    virtual double getRealSynapseWeight(const unsigned int &index){ (void)index; return 0.0f; }
    virtual unsigned char getRealSynapseLearning(const unsigned int &index){ (void)index; return ' ';}


    virtual bool exportNetwork(QString filename){ (void)filename; std::cout << "Not in this simulation\n"; return false; }

    virtual void addForcedSpikesPatternA(QVector<unsigned int> data){ (void)data; std::cout << "Not in this simulation\n"; }
    virtual void clearForcedSpikesA(int patternIndex){ (void)patternIndex; std::cout << "Not in this simulation\n"; }
    virtual void clearForcedSpikesA(){ std::cout << "Not in this simulation\n"; }
    virtual void addForcedSpikesPatternB(QVector<unsigned int> data){ (void)data; std::cout << "Not in this simulation\n"; }
    virtual void clearForcedSpikesB(int patternIndex){ (void)patternIndex; std::cout << "Not in this simulation\n"; }
    virtual void clearForcedSpikesB(){ std::cout << "Not in this simulation\n"; }
    virtual void addRandomGenerator(QList<int> data){ (void)data; std::cout << "Not in this simulation\n"; }
    virtual void setDuration(int value){ (void)value; std::cout << "Not in this simulation\n"; }
    virtual void setHowLong(int value){ (void)value; std::cout << "Not in this simulation\n"; }
    virtual void setEvent(int _howLong, int _pre, int _post, int stimFirst,int stimLast, int stimValue){ (void)_howLong; (void)_pre; (void)_post; (void)stimFirst; (void)stimLast; (void)stimValue; std::cout << "Not in this simulation\n"; }

    virtual void clearValues(){ std::cout << "Not in this simulation\n"; }
    virtual void clearParameters(){ std::cout << "Not in this simulation\n"; }
    virtual void startInitializing(){ std::cout << "Not in this simulation\n"; }
    virtual void continueInitializing(){ std::cout << "Not in this simulation\n"; }
    virtual void finishInitialization(){ initialized = true; }
    virtual void applyLearning(bool on){ std::cout << on << " Not in this simulation\n"; }
    virtual void setD1(const int &idx, const double &value){std::cout << idx << "," << value << " No dopamine in this simulation\n"; }
    virtual void setD2(const int &idx, const double &value){std::cout << idx << "," << value << " No dopamine in this simulation\n"; }
    virtual void setSynapticModel(const int &type){ synapticModel = type; }

    // -- STIMULATION ----------------------------------------------------------
public slots :
    virtual void setStimulus(QString blockName, int neuronIndex, float value) {
        if (stimuli.contains(blockName)) {
            stimuli[blockName].baseline = value;
            stimuli[blockName].firstNeuron = neuronIndex;
            stimuli[blockName].lastNeuron = neuronIndex;
        } else {
            // stimuli[blockName] = Stimulus(neuronIndex, neuronIndex, value);
            stimuli.insert(blockName, Stimulus(neuronIndex, neuronIndex, value));
        }
    }

    virtual void setStimulus(QString blockName, int firstNeuron, int lastNeuron, float value) {
        if (stimuli.contains(blockName)) {
            stimuli[blockName].baseline = value;
            stimuli[blockName].firstNeuron = firstNeuron;
            stimuli[blockName].lastNeuron = lastNeuron;
        } else {
            // stimuli[blockName] = Stimulus(firstNeuron, lastNeuron, value);
            stimuli.insert(blockName, Stimulus(firstNeuron, lastNeuron, value));
        }
    }

    virtual void setStimulus(QString blockName, int firstNeuron, int lastNeuron,
                             double baseline, double amplitude, double frequency, double phase) {
        if (stimuli.contains(blockName)) {
            stimuli[blockName].firstNeuron = firstNeuron;
            stimuli[blockName].lastNeuron = lastNeuron;
            stimuli[blockName].baseline = baseline;
            stimuli[blockName].amplitude = amplitude;
            stimuli[blockName].frequency = frequency;
            stimuli[blockName].phase = phase;
        } else {
            // stimuli[blockName] = Stimulus(firstNeuron, lastNeuron, baseline, amplitude, frequency, phase);
            stimuli.insert(blockName, Stimulus(firstNeuron, lastNeuron, baseline, amplitude, frequency, phase));
        }

    }

    virtual void clearAllStimuli() {
        stimuli.clear();
    }

    virtual void clearStimulus(QString blockName) {
        stimuli.remove(blockName);
    }

};

#endif // SIMULATOR_H
