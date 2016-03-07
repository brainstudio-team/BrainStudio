#include "mutualinformation.h"

MutualInformation::MutualInformation(){
    jvmCreated = false;
    JavaVMOption* options = new JavaVMOption[1];
    options[0].optionString = "-Djava.class.path=/usr/lib/java:/home/pmediano/JCjidt/jidt/infodynamics.jar";
    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;
    /* load and initialize a Java VM, return a JNI interface
     * pointer in env */
    // JNI_CreateJavaVM((JavaVM **) &jvm, (void **) &env, (void *) &vm_args);
    // jvmCreated = true;
    delete options;


}

QVector<unsigned int> MutualInformation::getSymbols(QList<float> x) {

    unsigned int tau = 2;

    QVector<unsigned int> s = QVector<unsigned int>(x.size() - 2*tau);

    for (int i = 0; i < x.size() - 2*tau; i++) {
        if ((x[i] < x[i+tau]) && (x[i+tau] < x[i+2*tau])) {
            s[i] = 0;
        } else if ((x[i] < x[i+tau]) && (x[i+tau] > x[i+2*tau]) && (x[i] < x[i+2*tau])) {
            s[i] = 1;
        } else if ((x[i] < x[i+tau]) && (x[i] > x[i+2*tau])) {
            s[i] = 2;
        } else if ((x[i] > x[i+tau]) && (x[i] < x[i+2*tau])) {
            s[i] = 3;
        } else if ((x[i] > x[i+tau]) && (x[i+tau] < x[i+2*tau]) && (x[i] > x[i+2*tau])) {
            s[i] = 4;
        } else if ((x[i] > x[i+tau]) && (x[i+tau] > x[i+2*tau])) {
            s[i] = 5;
        }
    }

    return s;
}

double MutualInformation::calculate(QList<float> tseriesA, QList<float> tseriesB){

    int npt = tseriesA.size();
    if (npt != tseriesB.size()) {
        qDebug() << "ERROR in synchrony calculation. Inputs must have the same length";
        return 0;
    }

    QVector<unsigned int> symbolA, symbolB;
    symbolA = getSymbols(tseriesA);
    symbolB = getSymbols(tseriesB);

    QVector<unsigned int> counts = QVector<unsigned int>(36, 0);
    QVector<unsigned int> countsA = QVector<unsigned int>(6, 0);
    QVector<unsigned int> countsB = QVector<unsigned int>(6, 0);

    for (int i = 0; i < symbolA.size(); i++) {
        counts[symbolA[i]*6 + symbolB[i]]++;
        countsA[symbolA[i]]++;
        countsB[symbolB[i]]++;
    }

    int obs = symbolA.size();

    double mi = 0.0;
    for (int i = 0; i < 6; i++) {
        double probA = (double) countsA[i] / (double) obs;
        for (int j = 0; j < 6; j++) {
            double probB = (double) countsB[j] / (double) obs;
            double jointProb = (double) counts[6*i + j] / (double) obs;
            if (probA * probB * jointProb > 0)
                mi += jointProb * log(jointProb / (probA * probB));
        }
    }

    if (mi < 0) mi = 0;

    // Normalize by the maximum MI
    qDebug() << "MI = " << mi;
    return mi/log(6);




    /*
    // Initialise calculator
    jmethodID init_mid = env->GetMethodID(cls, "initialize", "(I,I)V");
    env->CallVoidMethod(cls, init_mid, 1, 1);

    // Set observations
    jmethodID setObs_mid = env->GetMethodID(cls, "setObservations", "(D[],D[])V");

    jdouble *dataA = (jdouble*) malloc(tseriesA.size() * sizeof(jdouble));
    for (int i = 0; i < tseriesA.size(); i++) dataA[i] = (jdouble) tseriesA[i];

    jdouble *dataB = (jdouble*) malloc(tseriesB.size() * sizeof(jdouble));
    for (int i = 0; i < tseriesB.size(); i++) dataB[i] = (jdouble) tseriesB[i];

    env->CallVoidMethod(cls, setObs_mid, dataA, dataB);

    // maybe use tseriesA.pointer ?

    // Calculate
    jmethodID calc_mid = env->GetMethodID(cls, "computeAverageLocalOfObservations", "(V)D");
    jdouble res = env->CallDoubleMethod(cls, calc_mid);

    return (double) res;*/

}

double MutualInformation::calculate_self(QList<float> tseries) {
    // In the future we might implement entropy, but for now
    // set edge transparent

    return 0.0;
}
