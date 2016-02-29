#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>

#include "tcpip.h"


TCPIP_Sim::TCPIP_Sim(QObject *parent, QString simulation_file):
                           Simulator(1.0, parent),brn_path(simulation_file){
    timeStep = 0.0;
    all_neurons = 0;
    tcpSocket = new QTcpSocket(this);
    brn_file = NULL;
    spikesArray = NULL;
    stimuli.clear();

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(handleMessage()));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
}


void TCPIP_Sim::connected(){
    qDebug() << "We are connected";
    //tcpSocket->write("connected"); // No, cause I send another msg in loading!
    initialized = true;
    emit backend_connected();
    this->loadXML();
}

/* Method: disconnected()
 */
void TCPIP_Sim::disconnected(){
    qDebug() << "Disconnected";
    initialized = false;
}

void TCPIP_Sim::handleMessage(){
    qDebug() << "Bytes available:" << tcpSocket->bytesAvailable();
    QByteArray aa = tcpSocket->readAll();
    qDebug() << aa;
    QString reply = QString(aa);
    qDebug() << "Server:" << reply;

    /*if(reply == "OK!!"){
        tcpSocket->write(reply.toUtf8());
    }
    else */
    if(reply.size() >= 4 && reply.left(4) == "done"){
        //reply = reply.split(' ').at(1);
        QStringList list = reply.split(' ');
        reply = list.at(1);
        timeStep = reply.toDouble();

        if(list.size() > 2){
            if(list.at(2) == "spikes" && list.size() > 3){
                QString str_spikes = list.at(3);
                QStringList list_spikes = str_spikes.split(",");

                for(int i=0; i<list_spikes.size(); i++){
                    spikesArray[i] = list_spikes[i].toInt();
                }
                spikes = (size_t)list_spikes.size();
            }
            if(list.at(5) == "rates" && list.size() > 8){
                QString str_rates_ind = list.at(6);
                QString str_rates_val = list.at(7);
                QStringList list_rates_ind = str_rates_ind.split(",");
                QStringList list_rates_val = str_rates_val.split(",");

                for(int i=0; i<list_rates_ind.size(); i++){
                    neuronsState[list_rates_ind[i].toInt()] = list_rates_val[i].toFloat();
                }
            }
        }

        emit step_done();
    }
    else if(reply.size() >= 6 && reply.left(6) == "spikes"){
        QStringList list = reply.split(' ');

        if(list.size() > 1){
            QString str_spikes = list.at(1);
            QStringList list_spikes = str_spikes.split(",");

            for(int i=0; i<list_spikes.size(); i++){
                spikesArray[i] = list_spikes[i].toInt();
            }
            spikes = (size_t)list_spikes.size();
        }

        //emit step_done(); ZAF???????
    }
    else if(reply == "give me"){
        // File -> read next line
        if(brn_file != NULL ){
            if(!brn_file->atEnd()) {
                QByteArray line = brn_file->readLine();
                // Send it
                tcpSocket->write(line);
            }
            else{
                tcpSocket->write("end");
                delete brn_file;
                brn_file = NULL;
            }
        }
    }
    else if(reply.size() >= 5 && reply.left(5) == "nodes"){
        qDebug() << "AA" << reply;
        tcpSocket->write("get edges");
    }
    else if(reply.size() >= 5 && reply.left(5) == "edges"){
        qDebug() << "BB" << reply;
    }
    else if( (reply.size() >= 11 && reply.left(11) == "file loaded") ||
             (reply.size() >= 15 && reply.left(15) == "already running") ){
        QStringList list = reply.split(' ');
        if(list.size() > 2)
            all_neurons = unsigned(list.at(2).toInt());

        // Initialize the array that is going to be used to pass information
        // for firing rates and rasters..
        if(spikesArray != NULL)
            delete [] spikesArray;
        spikesArray = new unsigned[all_neurons];

        // Initialize the array that is going to be used to pass information
        // about the neurons state (main variable)
        neuronsState.clear();
        neuronsState.resize(all_neurons);

        emit simulation_loaded(this,NULL,NULL,0); // ???
    }
    else if(reply.size() >= 5 && reply.left(5) == "error"){
        QStringList list = reply.split('"');
        // display error
        QString msg = list.at(7)+"\n\n";
        msg+=list.at(3);
        stop();
        emit error_signal(msg);
    }


    //qDebug() << "Bytes available:" << tcpSocket->bytesAvailable();
}

void TCPIP_Sim::startInitializing(){
    qDebug() << "Connecting..";
    tcpSocket->abort();
    tcpSocket->connectToHost(UserData::tcpip_hostname, UserData::tcpip_port);
}

void TCPIP_Sim::getNodes(){
    qDebug() << "Asking for nodes..";
    tcpSocket->write("get nodes");
}

void TCPIP_Sim::getEdges(){
    qDebug() << "Asking for edges..";
    tcpSocket->write("get edges");
}

void TCPIP_Sim::stop(){
    qDebug() << "Disconnecting..";
    tcpSocket->abort();
}

void TCPIP_Sim::step(const int &speed){
    QString msg = "step " + QString::number(timeStep) + " spikes all ";
    msg += "speed " + QString::number(speed);

    if (!stimuli.isEmpty()) {
        QMap<QString, Stimulus>::const_iterator it;
        for(it=stimuli.constBegin(); it != stimuli.constEnd(); it++){
            msg += it.value().toString(timeStep);
        }
    }

    tcpSocket->write(msg.toUtf8());
}

void TCPIP_Sim::play(){
    qDebug() << "Starting..";
    tcpSocket->write("play ");
}
void TCPIP_Sim::pause(){
    qDebug() << "Pausing..";
    tcpSocket->write("pause ");
}

void TCPIP_Sim::loadXML(){
    qDebug() << "Sending brn file..";

    brn_file = new QFile(brn_path);

    if (!brn_file->open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "TCPIP_Sim: Error: Cannot open brn file in:" << brn_path;
        return;
    }

    /*while (!brn_file->atEnd()) {
        QByteArray line = brn_file->readLine();
        process_line(line);
    }*/
    tcpSocket->write("sending file");
}


void TCPIP_Sim::displayError(QAbstractSocket::SocketError socketError)
{
    /*switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }*/
}
