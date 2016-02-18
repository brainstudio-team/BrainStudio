#ifndef HELPERCLASSES_H
#define HELPERCLASSES_H

#include <cmath>
#include <cstdlib>
#include "modelValues.h"
#ifdef LOGS
    #include <iostream>
#endif

/* --------------------------------------------------------------------------
 * DESCRIPTION OF CLASS: Function
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * -------------------------------------------------------------------------- */
class Function{
protected:
    double e;
    double _t;
    double _a, _b, _c, _d;
    double _G, _L;
    double _maxD, _minD;

public:
    Function(){ e = 2.71828182846; _t = 0;
                turnTo(0);
              }

    // -------------- Exponential ----------------------------------------------
    // f(t) = a / ( b + e^(t/d-c)^2 )
    double outputExp(const double &t){
        _t = local_global_filter(_a/(_b + pow(e, (t/_d-_c)*(t/_d-_c) )));
        return limited();
    }
    // -------------- Simple Like Report ---------------------------------------
    double outputSimple(const double &t){
        _t = local_global_filter(1.0/(1.0+pow(abs((t-_c)/_a),2.0*_b)));
        return limited();
    }

    // -------------- Humphries etal 2009 --------------------------------------
    double outputHumphries(const double &value){
        if(_a == 0.0 || _d == 0.0) return 0.0;
        _t = local_global_filter(_a*pow(_d*value,-_b)*pow(e,-(_d*value)*_c));
        return limited();
    }

    double outputChoose(const double &value, const int &method){
        switch(method){
        case 0:  return outputSimple(value);
        case 1:  return outputExp(value);
        case 2:  return outputHumphries(value);
        default: return 0.0;
        }
    }

    double local_global_filter(const double &value){
        if(_G < _L) return _L*_G+_L*(_L-_G)*value;
        else return _G;
    }

    void turnTo(const int &method){
        switch(method){
        case 0: // Simple
            _a = Values::Weight_a;
            _b = Values::Weight_b;
            _c = Values::Weight_c;
            _d = Values::Weight_d;
            _L = Values::Weight_L;
            _G = Values::Weight_G;
            _minD = Values::Weight_minD;
            _maxD = Values::Weight_maxD;
            return;
        case 1: // Exponential
            _a = Values::Prob_exp_a;
            _b = Values::Prob_exp_b;
            _c = Values::Prob_exp_c;
            _d = Values::Prob_exp_d;
            _L = Values::Prob_exp_L;
            _G = Values::Prob_exp_G;
            _minD = Values::Prob_exp_minD;
            _maxD = Values::Prob_exp_maxD;
            return;
        case 2: // Humphries
            _a = Values::Hum_a;
            _b = Values::Hum_b;
            _c = Values::Hum_c;
            _d = Values::Hum_d;
            _L = Values::Hum_L;
            _G = Values::Hum_G;
            _minD = Values::Hum_minD;
            _maxD = Values::Hum_maxD;
            return;
        default:
            return;
        }
    }

    double limited(){
        #ifdef LOGS
            std::cout << "_t = " << _t << "\n";
        #endif
        if(_t > _maxD ) return _maxD;
        else if ( _t < _minD ) return _minD;
        else return _t;
    }

    double a(){ return _a; }
    double b(){ return _b; }
    double c(){ return _c; }
    double d(){ return _d; }
    double maxDistance(){ return _maxD; }
    double minDistance(){ return _minD; }
    double globalFactor(){ return _G; }
    double localFactor(){ return _L; }

    void setA(double value){ _a = value; }
    void setB(double value){ _b = value; }
    void setC(double value){ _c = value; }
    void setD(double value){ _d = value; }
    void setMaxDistance(double value){ _maxD = value; }
    void setMinDistance(double value){ _minD = value; }
    void setGlobalFactor(double value){ _G = value; }
    void setLocalFactor(double value){ _L = value; }
};

/* --------------------------------------------------------------------------
 * DESCRIPTION OF CLASS: OscillationSet
 *
 * Variables: first/last neurons, A, phase,
 *
 * This class includes a method (setStimWeight) which calculates the weight of
 * a stimulus according to the following oscillatory function:
 *
 *                    / A*sin(2*pi*frequency*((time+delay)/1000.0)-phase);
 *          weight = <
 *                    \ 0   if the above is < 0
 *
 *
 * -------------------------------------------------------------------------- */
class OscillationSet{
public:
    OscillationSet():firstNeuron(0),lastNeuron(0),frequency(0.0),delay(0),
        A(0.0),phase(0.0){}

    void initialize(const int &first, const int &last, const double &freq,
                    const double &_delay, const double _A = 1.0,
                    const double _phase = 0.0){
                firstNeuron = first;
                lastNeuron = last;
                frequency = freq;
                delay = _delay;
                A = _A;
                phase = _phase;
                stimWeight = 1.0;
    }
    int firstNeuron, lastNeuron;
    double A;  // The amplitude (max displacement from the equilibrium position)
    double phase;       // The phase of the oscillation
    double frequency;   // The frequency of the oscillation
    double delay;       // The delay of the oscillation
    double stimWeight;  // The output value

    // Methods
    double setStimWeight(double time){
        // Set the stimulus weight
        stimWeight = A*sin(2.0*M_PI*frequency*((time+delay)/1000.0)-phase);
        //Could also be:(A*sin(2.0*M_PI*frequency*(timeStep/1000.0)-phase)+A)/2.0f;
        if(stimWeight < 0)
            stimWeight = 0;
        return stimWeight;
    }
};

/* -------------------------------------------------------------------------- *\
 * DESCRIPTION OF CLASS: PostNeuron
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
\* -------------------------------------------------------------------------- */
class NeuronUnit{
protected:
    float _x;
    float _y;
    float _z;

public:
    NeuronUnit(float x=0.0f, float y=0.0f, float z=0.0f):_x(x), _y(y), _z(z){}
    ~NeuronUnit(){}

    float x(){ return _x; }
    float y(){ return _y; }
    float z(){ return _z; }

    void setX(float value){ _x = value; }
    void setY(float value){ _y = value; }
    void setZ(float value){ _z = value; }
};

#endif // HELPERCLASSES_H

