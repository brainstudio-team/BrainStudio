#include "synchrony.h"

Synchrony::Synchrony() {
    hilbert_kernel = QVector<double>(LFILT + 1);
    for (int i = 1; i <= LFILT; i++)
        hilbert_kernel[i] = 1/((i-LFILT/2)-0.5)/M_PI;

}


double Synchrony::calculate(QList<float> tseriesA, QList<float> tseriesB){

    int npt = tseriesA.size();
    if (npt != tseriesB.size()) {
        qDebug() << "ERROR in synchrony calculation. Inputs must have the same length";
        return 0;
    }

    if (npt < 3) {
        return 0;
    }

    double sumA = 0.0, sumAA = 0.0, sumB = 0.0, sumBB = 0.0, sumAB = 0.0;
    for (int i = 0; i < npt; i++) {
        sumA += tseriesA[i];
        sumB += tseriesB[i];
        sumAA += tseriesA[i]*tseriesA[i];
        sumBB += tseriesB[i]*tseriesB[i];
        sumAB += tseriesA[i]*tseriesB[i];
    }

    double crosscov = (sumAB - sumB*sumA/npt)/npt;
    double varA = (sumAA - sumA*sumA/npt)/npt;
    double varB = (sumBB - sumB*sumB/npt)/npt;

    double corrcoef = crosscov / (sqrt(varA)*sqrt(varB));

    qDebug() << "varA = " << varA << ", varB = " << varB << ", varAB = " << crosscov << ", corr = " << corrcoef;

    return fabs(corrcoef);

    /*
    if (npt <= LFILT) {
        return 0.0;
    }

    QVector<double> phaseA = FiringRateToPhase(tseriesA);
    QVector<double> phaseB = FiringRateToPhase(tseriesB);

    double sync = 0.0;
    for (int i = 0; i < phaseA.size(); i++) {
        double sumCos = 0.5*(cos(phaseA[i]) + cos(phaseB[i]));
        double sumSin = 0.5*(sin(phaseA[i]) + sin(phaseB[i]));
        sync += (1.0/npt) * sqrt(sumCos*sumCos + sumSin*sumSin);
    }

    return sync;
    */
}

double Synchrony::calculate_self(QList<float> tseries) {
    // Don't draw self-connections when calculating synchrony
    return 0.0;
}

QVector<double> Synchrony::FiringRateToPhase(QList<float> x) {

    double xt, xht;
    int npt = x.size();

    QVector<double> xh = QVector<double>(npt, 0.0);
    QVector<double> ampl = QVector<double>(npt, 0.0);
    QVector<double> phase = QVector<double>(npt, 0.0);

    // Hilbert transform
    Convolution(x, xh, hilbert_kernel, npt, LFILT);

    // Ampl and phase
    for (int i=LFILT/2+1; i<=npt-LFILT/2; i++) {
        xt = x[i];
        xht = xh[i];
        ampl[i] = sqrt(xt*xt+xht*xht);
        phase[i] = atan2(xht ,xt);
    }

    return phase;

}


// void Synchrony::Convolution(double *source, double *target, double *filt, int npt, int lfilt) {
void Synchrony::Convolution(QList<float> source, QVector<double> target, QVector<double> filt, int npt, int lfilt) {

    int i, l;
    double yt;

    for (l=1; l<=npt-lfilt+1; l++) {
        yt = 0.0;
        for (i=1; i<=lfilt; i++)
            yt = yt+source[l+i-1]*filt[lfilt+1-i];
        target[l] = yt;
    }

    // shifting lfilt/1+1/2 points
    for (i=1; i<=npt-lfilt; i++) {
        target[i] = 0.5*(target[i]+target[i+1]);
    }
    for (i=npt-lfilt; i>=1; i--)
        target[i+lfilt/2]=target[i];

    // writing zeros
    for (i=1; i<=lfilt/2; i++) {
        target[i] = 0.0;
        target[npt+1-i] = 0.0;
    }
}

