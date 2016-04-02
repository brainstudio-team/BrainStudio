#ifndef TRANSFERENTROPYCALCULATOR_H
#define TRANSFERENTROPYCALCULATOR_H

//SpikeStream includes
//#include "SpikeStreamTypes.h"

//Qt includes
#include <QtCore/QDebug>
#include <QtCore/QHash>
#include <QtCore/QVector>

//Other includes
#include <cmath>
#include <iostream>

using namespace std;

namespace spikestream{

class TransferEntropyCalculator{

public:
    TransferEntropyCalculator(unsigned k_param, unsigned l_param, unsigned timeWindow);
    ~TransferEntropyCalculator();

    QString getBitString(unsigned num);

    // ACCESSORS
    QHash<unsigned, double>& getI_k_probs() { return I_k_probs; }
    QHash<unsigned, double>& getI_k_plus_1_probs() { return I_k_plus_1_probs; }
    QHash<unsigned, double>& getI_k_J_l_probs() { return I_k_J_l_probs; }
    QHash<unsigned, double>& getI_k_plus_1_J_l_probs() { return I_k_plus_1_J_1_probs; }

    double getTransferEntropy(unsigned startTimeStep, QVector<unsigned>& iVector, QVector<unsigned>& jVector);

    void load_I_k_probabilities(unsigned startTimeStep, QVector<unsigned>& iVector);
    void load_I_k_plus_1_probabilities(unsigned startTimeStep, QVector<unsigned>& iVector);
    void load_I_k_J_l_probabilities(unsigned startTimeStep, QVector<unsigned>& iVector, QVector<unsigned>& jVector);
    void load_I_k_plus_1_J_l_probabilities(unsigned startTimeStep, QVector<unsigned>& iVector, QVector<unsigned>& jSVector);

private:
    //=============================  VARIABLES  ============================
    /*! k parameter of analysis */
    unsigned k_param;

    /*! l (L) parameter of analysis */
    unsigned l_param;

    /*! time window of analysis */
    unsigned timeWindow;

    /*! Map of probabilities of sequences of length k in I */
    QHash<unsigned, double> I_k_probs;

    /*! Map of probabilities of sequences of length k+1 in I */
    QHash<unsigned, double> I_k_plus_1_probs;

    /*! Map of combined probabilities of sequences of length k in I and sequences of length l in J */
    QHash<unsigned, double> I_k_J_l_probs;

    /*! Map of combined probabilities of sequences of length k+1 in I and sequences of length l in J */
    QHash<unsigned, double> I_k_plus_1_J_1_probs;


    //=============================  METHODS  ==============================
    void printMap(QHash<unsigned, double>& map, QString name);

};

}

#endif//TRANSFERENTROPYCALCULATOR_H
