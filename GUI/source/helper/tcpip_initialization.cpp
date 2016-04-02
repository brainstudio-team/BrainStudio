#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>

#include "tcpip_initialization.h"
#include "source/helper/userdata.h"

TCPIP_Init::TCPIP_Init(QObject *parent) : QObject(parent){
    tcpSocket = new QTcpSocket(this);

    getting_nodes = getting_edges = false;

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(handleMessage()));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
}


void TCPIP_Init::connected(){
    qDebug() << "We are connected";
    //tcpSocket->write("connected"); // No, cause I send another msg in loading!

    //emit backend_connected();
    this->getNodes();
}
/* Method: disconnected()
 */
void TCPIP_Init::disconnected(){
    qDebug() << "Disconnected";
}


void TCPIP_Init::handleMessage(){
    qDebug() << "AA: Bytes available:" << tcpSocket->bytesAvailable();
    QByteArray aa = tcpSocket->readAll();
    //qDebug() << aa; // this also prints the same stuff as below.
    QString reply = QString(aa);
    qDebug() << "Server (size=" << reply.size() << "):" << reply;


    // ZAF SOS: I've just noticed that this happens and this is a hacky fix..
    if(reply.left(4) == "OK!!"){
        reply.remove(0,4);
    }


    if(getting_nodes){
        qDebug() << "Appending to nodes";
        nodes += reply;
    }
    else if(getting_edges){
        qDebug() << "Appending to edges";
        edges += reply;
    }
    /*else if(reply == "OK!!"){
        tcpSocket->write(reply.toUtf8());
    }
    else */
    else if(reply.size() >= 6 && reply.left(6) == "spikes"){
        QStringList list = reply.split(' ');

        if(list.size() > 1){
            /*QString str_spikes = list.at(1);
            QStringList list_spikes = str_spikes.split(",");

            for(int i=0; i<list_spikes.size(); i++){
                spikesArray[i] = list_spikes[i].toInt();
            }
            spikes = (size_t)list_spikes.size();*/
        }

        //emit step_done(); ZAF???????
    }
    else if(reply.size() >= 5 && reply.left(5) == "nodes"){
        qDebug() << "STARTED GETTING THE NODES (size=" << reply.size() << "):";
        getting_nodes = true;
        nodes = reply;
    }
    else if(reply.size() >= 5 && reply.left(5) == "edges"){
        qDebug() << "STARTED GETTING THE EDGES (size=" << reply.size() << "):";
        getting_edges = true;
        edges = reply;
    }


    // If downloading of either nodes or edges finished, deal with the data!
    if(getting_nodes && reply.right(9) == "nodes end"){
        qDebug() << "WE GOT THE NODES!";
        getting_nodes = false;

        // Remove first/last words
        nodes.remove(0, 6); // "nodes "
        nodes.remove(nodes.size()-9, nodes.size()-1); // "nodes end"

        emit nodes_downloaded(nodes);
    }
    else if(getting_edges && reply.right(9) == "edges end"){
        qDebug() << "WE GOT THE EDGES!";
        getting_edges = false;

        // Remove first/last words
        edges.remove(0, 6); // "edges "
        edges.remove(edges.size()-9, edges.size()-1); // "edges end"

        emit edges_downloaded(edges);
    }
}

void TCPIP_Init::disconnect(){
    tcpSocket->disconnectFromHost();
}

void TCPIP_Init::getNodes(){
    qDebug() << "Asking for nodes..";
    tcpSocket->write("get nodes");
}

void TCPIP_Init::getEdges(){
    qDebug() << "Asking for edges..";
    tcpSocket->write("get edges");
}

void TCPIP_Init::connect_to_download_data(){
    qDebug() << "Connecting.." <<UserData::tcpip_hostname<<UserData::tcpip_port;
    tcpSocket->abort();
    tcpSocket->connectToHost(UserData::tcpip_hostname, UserData::tcpip_port);
}















































void TCPIP_Init::displayError(QAbstractSocket::SocketError socketError)
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














