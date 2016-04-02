#ifndef TCPIP_H
#define TCPIP_H
#include "simulator.h"

#include <QWidget>
//#include <QMutex>
#include <QTcpSocket>
#include <QFile>
#include <QStringList>

class TCPIP_Sim : public Simulator{
    Q_OBJECT

public:
    TCPIP_Sim(QObject *parent = 0, QString simulation_file = "");
    ~TCPIP_Sim(){ if(spikesArray != NULL) delete [] spikesArray; }

private slots:
    void handleMessage();
    void connected();
    void disconnected();
    void displayError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *tcpSocket;
    //QMutex mutex;
    QFile *brn_file; // File that is going to be sent to the backend
    QString brn_path;
    unsigned all_neurons;

public:
    void step(const int &speed);
    void startInitializing();
    void stop();
    void play();
    void pause();
    void loadXML();
    void getNodes();
    void getEdges();

    int   getNeurons(){return 0;}
    int   getSynapses(){return 0;}

    float getNeuronState(unsigned int index){ return neuronsState[index]; }

};

#endif
