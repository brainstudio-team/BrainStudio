/*
Interface for different classes of backends that might used in BrainStd.
Although the default functionality provides only a tool to connect to
other backends-programs via tcpip, this interface can be used to create
an embedded backend.
*/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "source/helper/helperClasses.h"
#include "source/helper/userdata.h"

#include <iostream>
#include <QWidget>

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


    // OSCILLATIONS
    int osc, s; // helper variables
    QVector<OscillationSet> oscillation;

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
        oscillation.clear();
        initialized = false;
        return false;
    }

    // LOCAL FIELD POTENTIAL
    virtual double getLocalFieldPotential(const int &first, const int &last){return 0.0;}

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
    virtual float getSynapseWeight(const unsigned &neuron, const unsigned &synapseIndx){return 0.0f;}


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
    virtual float getRealNeuronStateU(const unsigned &index){return 0.0f;}
    virtual float getRealNeuronStateV(const unsigned &index){return 0.0f;}
    virtual float getRealNeuronState(const unsigned &index, const unsigned &param){ (void)index; (void)param; return 0.0f; }

    // GET REAL SYNAPSE PARAMETERS
    virtual unsigned int  getRealSynapseSource(const unsigned int &index){ (void)index; return 0; }
    virtual unsigned int  getRealSynapseTarget(const unsigned int &index){ (void)index; return 0; }
    virtual double getRealSynapseDelay(const unsigned int &index){ (void)index; return 0; }
    virtual double getRealSynapseWeight(const unsigned int &index){ (void)index; return 0.0f; }
    virtual unsigned char getRealSynapseLearning(const unsigned int &index){ (void)index; return ' ';}


    virtual bool save(QString filename){ (void)filename; std::cout << "Not in this simulation\n"; }

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
    virtual void setStimulus(int neuronIndex, float value) = 0;
    virtual void setStimulus(int firstNeur, int lastNeur, float value) = 0;
    virtual void clearStimulus() = 0;
    virtual void clearStimulus(const int &firstNeuron, const int &lastNeuron){ (void)firstNeuron; (void)lastNeuron; }
    // OSCILLATIONS:
    virtual void setOscillation(const int &firstNeuron, const int &lastNeuron, const double &maxStimulus, const double &_freq, const int &_delay, const double &_phase){
        stopOscillation(firstNeuron, lastNeuron);
        setStimulus(firstNeuron, lastNeuron, 0.0f); // To make sure that ids exist and are continues (needed for update oscillations) ZAF: To check!
        oscillation.append(OscillationSet());
        oscillation[oscillation.size()-1].initialize(firstNeuron, lastNeuron, _freq, _delay, maxStimulus, _phase);
    }

    virtual void stopOscillation(const int &firstNeuron, const int &lastNeuron){
        // Find if we already have set oscillations for these neurons  - Zaf: good one :) -
        for(int i=0; i<oscillation.size(); i++){
            // If the new set covers another one
            if(firstNeuron <= oscillation[i].firstNeuron &&
                    lastNeuron >= oscillation[i].lastNeuron){
                //clearStimulus(firstNeuron, lastNeuron);
                //setStimulus(firstNeuron, lastNeuron, 0.0f);
                oscillation.remove(i);
            }
            // If ..
            else if(firstNeuron > oscillation[i].firstNeuron &&
                    firstNeuron < oscillation[i].lastNeuron &&
                    lastNeuron > oscillation[i].lastNeuron){
                //clearStimulus(firstNeuron, lastNeuron);
                //setStimulus(firstNeuron, lastNeuron, 0.0f);
                oscillation[i].lastNeuron = firstNeuron;
            }
            // If ..
            else if(firstNeuron < oscillation[i].firstNeuron &&
                    lastNeuron > oscillation[i].firstNeuron &&
                    lastNeuron < oscillation[i].lastNeuron){
                //clearStimulus(firstNeuron, lastNeuron);
                //setStimulus(firstNeuron, lastNeuron, 0.0f);
                oscillation[i].firstNeuron = lastNeuron;
            }
            // If the new set is inside another one
            else if(firstNeuron > oscillation[i].firstNeuron &&
                    firstNeuron < oscillation[i].lastNeuron &&
                    lastNeuron < oscillation[i].lastNeuron){
                //clearStimulus(firstNeuron, lastNeuron);
                //setStimulus(firstNeuron, lastNeuron, 0.0f);
                oscillation.remove(i);
            }
        }
    }
    virtual void stopAllOscillations(){ oscillation.clear(); }                  // ZAF: not correct because the stimulusIds remain there!
    virtual double getStimWeight(){ if(!oscillation.isEmpty())                  // ZAF: for debugging I guess..!
                                       return oscillation[0].stimWeight;
                                    else return -1;
                                  }
    virtual void updateOscillations(){
        for(osc=0; osc<oscillation.size(); osc++){
          for(s=oscillation[osc].firstNeuron; s<oscillation[osc].lastNeuron; s++)
             setStimulus(s, oscillation[osc].setStimWeight(timeStep));
       }
     }
};

#endif // SIMULATOR_H
