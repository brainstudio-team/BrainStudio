#ifndef BACKENDDATA_H
#define BACKENDDATA_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QMapIterator>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "userdata.h"

#define BackendDataNodes    "backend_nodes.ini"
#define BackendDataEdges    "backend_edges.ini"


class BackendData{
public:
    typedef QList<QString> my_list;
    typedef QList< QList<QString> > my_2d_list;

    // -- NODES ----------------------------------------------------------------
    static my_list nodes;
    // Parameter/state names
    static QMap<QString, my_list > node_params;
    static QMap<QString, my_list > node_states;
    // Outmost map lists over different models, inner map over different
    // parameter sets for the same model
    static QMap<QString, QMap<QString, my_list> > node_params_default;
    static QMap<QString, my_list > node_states_default;
    // Fields: Parameters for the node as a whole
    static QMap<QString, my_2d_list > node_fields;
    static QMap<QString, QString > node_units_field; // specifies the name of field that defines the number of neurons
    static QMap<QString, QString > node_input_field;
    static QMap<QString, QString > node_output_field;
    static QMap<QString, QString > node_model; // spike, rate, etc..

    // -- EDGES ----------------------------------------------------------------
    static my_list edges;
    static QMap<QString, my_list > edge_params;
    static QMap<QString, my_list > edge_states;
    static QMap<QString, my_list > edge_params_default;
    static QMap<QString, my_list > edge_states_default;

    static QMap<QString, my_2d_list > edge_fields;
    static QMap<QString, QString > edge_input_model;
    static QMap<QString, QString > edge_output_model;


    static QString path;            // The application settings path.


     static void init(){
        path= QStandardPaths::writableLocation(QStandardPaths::DataLocation)
                                                         + "/" + UserDataFolder;
        QDir dir(path);
        if(!dir.exists())
            dir.mkpath(".");

        path = path + "/";// + BackendDataName;

        if(!QFile::exists(path+BackendDataNodes) ||
                !QFile::exists(path+BackendDataEdges)){
            //qDebug() << "Creating file for backend data.";
            //save();
        }
        else{
            qDebug() << "Loading Backend data.";
            BackendData::reload();
        }
    }

    static void load_nodes(QString data){
        qDebug() << "--------------------------------------------------------------------------------";

        if(data.size() < 6)
            return;

        BackendData::clear_nodes();

        qDebug() << data;

        // Parse the string
        QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());

        // Get JSON object
        QJsonObject json = doc.object();

        for (QJsonObject::iterator it = json.begin(); it != json.end(); it++) {
            QJsonObject configs = it.value().toObject();

            nodes.append(it.key());

            foreach (const QJsonValue & value, configs["params"].toArray()) {
                node_params[it.key()].append(value.toString());
            }
            foreach (const QJsonValue & value, configs["states"].toArray()) {
                node_states[it.key()].append(value.toString());
            }

            // Get all default parameter sets, including names and values
            foreach (QString k, configs["default_params"].toObject().keys()) {
                qDebug() << "New obj = " << k;
                QJsonArray parameterSet = configs["default_params"].toObject()[k].toArray();
                foreach (QJsonValue s, parameterSet) {
                    node_params_default[it.key()][k].append(s.toString());
                }
            }

            foreach (const QJsonValue & value,
                     configs["default_states"].toArray()) {
                node_states_default[it.key()].append(value.toString());
            }
            foreach (const QJsonValue & value,
                     configs["fields"].toArray()) {
                my_list temp_list;
                foreach (const QJsonValue & value2, value.toArray()) {
                    temp_list.append(value2.toString());
                }
                node_fields[it.key()].append(temp_list);
            }

            node_units_field[it.key()] = configs["units_field"].toString();
            node_input_field[it.key()] = configs["input_field"].toString();
            node_output_field[it.key()] = configs["output_field"].toString();
            node_model[it.key()] = configs["model"].toString();
        }
        for(int i = 0; i<nodes.size(); i++){
            qDebug() << nodes[i] << node_params[nodes[i]]
                     << "\n\t\t" << node_states[nodes[i]]
                     << "\n\t\t" << node_fields[nodes[i]]
                     << "\n\t\t" << node_units_field[nodes[i]]
                     << "\n\t\t" << node_input_field[nodes[i]]
                     << "\n\t\t" << node_output_field[nodes[i]]
                     << "\n\t\t" << node_model[nodes[i]];
        }
        qDebug() << "--------------------------------------------------------------------------------";

        BackendData::save_locally(path+BackendDataNodes, data);
    }


    static void load_edges(QString data){
        if(data.size() < 6)
            return;

        BackendData::clear_edges();

        // Parse the string
        QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());

        // Get JSON object
        QJsonObject json = doc.object();

        for (QJsonObject::iterator it = json.begin(); it != json.end(); it++) {
             QJsonObject configs = it.value().toObject();

            edges.append(it.key());

            foreach (const QJsonValue & value, configs["params"].toArray()) {
                edge_params[it.key()].append(value.toString());
            }
            foreach (const QJsonValue & value, configs["states"].toArray()) {
                edge_states[it.key()].append(value.toString());
            }
            foreach (const QJsonValue & value,
                     configs["default_params"].toArray()) {
                edge_params_default[it.key()].append(value.toString());
            }
            foreach (const QJsonValue & value,
                     configs["default_states"].toArray()) {
                edge_states_default[it.key()].append(value.toString());
            }
            foreach (const QJsonValue & value,
                     configs["fields"].toArray()) {
                my_list temp_list;
                foreach (const QJsonValue & value2, value.toArray()) {
                    temp_list.append(value2.toString());
                }
                edge_fields[it.key()].append(temp_list);
            }

            edge_input_model[it.key()] = configs["input_model"].toString();
            edge_output_model[it.key()] = configs["output_model"].toString();
        }
        for(int i = 0; i<edges.size(); i++){
            qDebug() << edges[i] << edge_params[edges[i]]
                     << "\n\t\t" << edge_states[edges[i]]
                     << "\n\t\t" << edge_fields[edges[i]]
                     << "\n\t\t" << edge_input_model[edges[i]]
                     << "\n\t\t" << edge_output_model[edges[i]];
        }
        BackendData::save_locally(path+BackendDataEdges, data);
    }

    static void save_locally(const QString &filename, const QString &data){
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        out << data;
        file.close();
    }


    static bool load_from_server(){
        return false;
    }

    static bool load_nodes_locally(){
        BackendData::clear_nodes();
        qDebug() <<"Loading backend nodes locally...";

        QFile backendfile(path+BackendDataNodes);
        if (!backendfile.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug()<<"BackendData:: Could not open " <<path<<" file to load..";
            return false;
        }
        QByteArray rawData = backendfile.readAll();
        backendfile.close();

        // Parse document
        BackendData::load_nodes(rawData);
    }

    static bool load_edges_locally(){
        BackendData::clear_edges();
        qDebug() <<"Loading backend edges locally...";

        QFile backendfile(path+BackendDataEdges);
        if (!backendfile.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug()<<"BackendData:: Could not open " <<path<<" file to load..";
            return false;
        }
        QByteArray rawData = backendfile.readAll();
        backendfile.close();

        // Parse document
        BackendData::load_edges(rawData);
    }

    static void clear_nodes() {
        QMap<QString, my_list>::iterator i;
        QMap<QString, QMap<QString, my_list> >::iterator j;

        nodes.clear();

        // Clear parameters
        for(i = node_params.begin(); i != node_params.end(); i++)
            i.value().clear();
        node_params.clear();

        // Clear default parameter values
        for(j = node_params_default.begin(); j != node_params_default.end(); j++) {
            for (i = j.value().begin(); i != j.value().end(); i++) {
                i.value().clear();
            }
            j.value().clear();
        }
        node_params_default.clear();

        // Clear states
        for(i = node_states.begin(); i != node_states.end(); i++)
            i.value().clear();
        node_states.clear();
        for(i = node_states_default.begin(); i != node_states_default.end(); i++)
            i.value().clear();
        node_states_default.clear();

        // Clear rest
        QMap<QString, my_2d_list>::iterator ii;
        for(ii = node_fields.begin(); ii != node_fields.end(); ii++){
            for(int j=0; j <ii.value().size(); j++){
                ii.value()[j].clear();
            }
            ii.value().clear();
        }
        node_fields.clear();
        node_units_field.clear();
        node_input_field.clear();
        node_output_field.clear();
        node_model.clear();
    }

    static void clear_edges(){
        QMap<QString, my_list>::iterator i;

        edges.clear();

        // Clear parameters
        for(i = edge_params.begin(); i != edge_params.end(); i++)
            i.value().clear();
        edge_params.clear();
        for(i = edge_params_default.begin(); i != edge_params_default.end(); i++)
            i.value().clear();
        edge_params_default.clear();

        // Clear states
        for(i = edge_states.begin(); i != edge_states.end(); i++)
            i.value().clear();
        edge_states.clear();
        for(i = edge_states_default.begin(); i != edge_states_default.end(); i++)
            i.value().clear();
        edge_states_default.clear();

        // Clear rest
        QMap<QString, my_2d_list>::iterator ii;
        for(ii = edge_fields.begin(); ii != edge_fields.end(); ii++){
            for(int j=0; j <ii.value().size(); j++){
                ii.value()[j].clear();
            }
            ii.value().clear();
        }
        edge_fields.clear();
        edge_input_model.clear();
        edge_output_model.clear();
    }

    // Partial information decomposition <-- paper by Beer and Williams

    static void reload(){
        qDebug() <<"BackendData:: Reloading..";
        BackendData::load_nodes_locally();
        BackendData::load_edges_locally();
    }
};

#endif // BACKENDDATA_H
