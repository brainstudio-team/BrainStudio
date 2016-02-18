#ifndef TCPIP_INIT_H
#define TCPIP_INIT_H

#include <QWidget>
#include <QTcpSocket>
#include <QStringList>

class TCPIP_Init : public QObject{
    Q_OBJECT

public:
    TCPIP_Init(QObject *parent = 0);
    ~TCPIP_Init(){}

    QString nodes, edges;
    bool getting_nodes, getting_edges;

private slots:
    void handleMessage();
    void connected();
    void disconnected();
    void displayError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *tcpSocket;

public:
    void getNodes();
    void getEdges();

    void connect_to_download_data();
    void disconnect();

signals:
    void nodes_downloaded(QString);
    void edges_downloaded(QString);
};

#endif
