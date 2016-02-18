#include "source/helper/userdata.h"
#include "source/helper/backendData.h"
#include "windows/mainwindow.h"

#include <QtGui>

QString UserData::workspace_path = "empty";
QString UserData::backend_path = "empty";
QString UserData::path = "";
QString UserData::tcpip_hostname = "localhost";
int     UserData::tcpip_port = 10000;

QString  BackendData::path = "";

BackendData::my_list BackendData::nodes;
QMap<QString, BackendData::my_list > BackendData::node_params;
QMap<QString, BackendData::my_list > BackendData::node_states;
QMap<QString, BackendData::my_list > BackendData::node_params_default;
QMap<QString, BackendData::my_list > BackendData::node_states_default;
QMap<QString, BackendData::my_2d_list > BackendData::node_fields;
QMap<QString, QString > BackendData::node_units_field;
QMap<QString, QString > BackendData::node_input_field;
QMap<QString, QString > BackendData::node_output_field;
QMap<QString, QString > BackendData::node_model;

BackendData::my_list BackendData::edges;
QMap<QString, BackendData::my_list > BackendData::edge_params;
QMap<QString, BackendData::my_list > BackendData::edge_states;
QMap<QString, BackendData::my_list > BackendData::edge_params_default;
QMap<QString, BackendData::my_list > BackendData::edge_states_default;
QMap<QString, BackendData::my_2d_list > BackendData::edge_fields;
QMap<QString, QString > BackendData::edge_input_model;
QMap<QString, QString > BackendData::edge_output_model;


#include <QMessageBox>

int main(int argc, char **argv){
    QApplication app(argc, argv);

    UserData::init();
    BackendData::init();


    QString file_path = "";
    if(argc > 1) {
        file_path = QString(argv[1]);
    }

    MainWindow mainWindow(file_path);
    (void)mainWindow;

    /*if(argc > 1) {
        QString aa = "Arguments: ";
        for(int i=0; i< argc; i++){
            aa += QString(argv[i]) +", ";
        }
        QMessageBox::information(&mainWindow, "Arguments", aa);
    }*/

    // This will not return until the last window is closed.
    return app.exec();
}
