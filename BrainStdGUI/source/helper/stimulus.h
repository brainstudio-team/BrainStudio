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
    double value, baseline, amplitude, frequency, phase;

    Stimulus() {}

    Stimulus(const int &_first, const int &_last, const double &_val) :
            firstNeuron(_first),
            lastNeuron(_last),
            value(_val),
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
            value = other.value;
            stim_type = STIMULATION_RANGE;

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

    QString toString(int timeStep) const {

        QString s = " stim "
                    + QString::number(firstNeuron) + "-"
                    + QString::number(lastNeuron) + " ";

        if (stim_type == STIMULATION_RANGE) {
            s += QString::number(value)+' ';
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

