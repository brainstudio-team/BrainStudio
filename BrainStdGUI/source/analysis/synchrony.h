#ifndef SYNCHRONY_H
#define SYNCHRONY_H

#include "math.h"

#include <QVector>
#include <QDebug>

class Synchrony {
private:
    const static unsigned int LFILT = 128;  // Must be even
    QVector<double> hilbert_kernel;

public:
    Synchrony();

    double calculate(QList<float> tseriesA, QList<float> tseriesB);

    double calculate_self(QList<float> tseries);

    QVector<double> FiringRateToPhase(QList<float> tseries);

    // void Convolution(double *source, double *target, double *filt, int npt, int lfilt);
    void Convolution(QList<float> source, QVector<double> target, QVector<double> filt, int npt, int lfilt);


};

#endif // SYNCHRONY_H
