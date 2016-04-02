#ifndef STIMULUS_H
#define STIMULUS_H

#include "math.h"

#include "QString"

class Stimulus {

    enum Type {STIMULATION_RANGE, OSCILLATION};

private:
    Type stim_type;

public:

    int firstNeuron, lastNeuron;
    double baseline, amplitude, frequency, phase;

    Stimulus(){Stimulus(0,0);}

    // Constructor for zero stimulus
    Stimulus(int first, int last) : firstNeuron(first), lastNeuron(last),
                 baseline(0.0), amplitude(0.0), frequency(0.0), phase(0.0),
                 stim_type(STIMULATION_RANGE){}

    Stimulus(const int &_first, const int &_last, const double &_val) :
            firstNeuron(_first),
            lastNeuron(_last),
            baseline(_val),
            stim_type(STIMULATION_RANGE) {}

    Stimulus(const int &_first, const int &_last, const double &_baseline,
             const double &_amplitude, const double &_frequency,
             const double &_phase) :
                firstNeuron(_first),
                lastNeuron(_last),
                baseline(_baseline),
                amplitude(_amplitude),
                frequency(_frequency),
                phase(_phase),
                stim_type(OSCILLATION) {}

    Stimulus(const Stimulus &other) {
        switch (other.getType()) {
        case STIMULATION_RANGE:
            firstNeuron = other.firstNeuron;
            lastNeuron = other.lastNeuron;
            baseline = other.baseline;
            stim_type = STIMULATION_RANGE;
            break;

        case OSCILLATION:
            firstNeuron = other.firstNeuron;
            lastNeuron = other.lastNeuron;
            baseline = other.baseline;
            amplitude = other.amplitude;
            frequency = other.frequency;
            phase = other.phase;
            stim_type = OSCILLATION;
        }
    }

    int size(){ return lastNeuron - firstNeuron; }

    QString toString(int timeStep) const {

        QString s = " stim "
                    + QString::number(firstNeuron) + "-"
                    + QString::number(lastNeuron) + " ";

        if (stim_type == STIMULATION_RANGE) {
            s += QString::number(baseline)+' ';
        }
        else if (stim_type == OSCILLATION) {
            double val = baseline + amplitude*sin(2.0*M_PI*frequency*timeStep/1000.0 - phase);
            s += QString::number(val)+' ';
        }
        else{
            qDebug() << "Stimulus::toString: This shouldn't happen!";
        }

        return s;
    }

    Type getType() const {
        return stim_type;
    }
};

#endif // STIMULUS_H

