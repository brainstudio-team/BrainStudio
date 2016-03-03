#ifndef MUTUALINFORMATION_H
#define MUTUALINFORMATION_H

// PEDRO: TODO: FIXME: OMG how did I get to this point
#include "/usr/lib/jvm/java-7-openjdk-amd64/include/jni.h"

#include "math.h"

#include <QVector>
#include <QDebug>

class MutualInformation {
private:
    JavaVM *jvm;       /* denotes a Java VM */
    JNIEnv *env;       /* pointer to native method interface */
    JavaVMInitArgs vm_args; /* JDK/JRE 6 VM initialization arguments */
    jclass cls;
    bool jvmCreated = false;

public:
    MutualInformation();
    /*
    MutualInformation(){
        JavaVMOption* options = new JavaVMOption[1];
        options[0].optionString = "-Djava.class.path=/usr/lib/java:/home/pmediano/JCjidt/jidt/infodynamics.jar";
        vm_args.version = JNI_VERSION_1_6;
        vm_args.nOptions = 1;
        vm_args.options = options;
        vm_args.ignoreUnrecognized = false;
        // load and initialize a Java VM, return a JNI interface
        // pointer in env
        JNI_CreateJavaVM(&jvm, (void**) &env, (void*) &vm_args);
        delete options;
    } */

    ~MutualInformation() {
        /* We are done. */
        if (jvm != NULL && jvmCreated) {
            jvm->DestroyJavaVM();
            jvmCreated = false;
        }
    }

    double calculate(QList<float> tseriesA, QList<float> tseriesB);

    double calculate_self(QList<float> tseries);

    QVector<unsigned int> getSymbols(QList<float> x);


};

#endif // MUTUALINFORMATION_H
