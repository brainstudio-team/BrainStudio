// The file that stores the preferences of the user

#ifndef USERDATA_H
#define USERDATA_H

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QApplication>
#include <QDesktopServices>
#include <QDir>

#define UserDataFolder  "BrainStd"
#define UserDataName    "config.ini"


class UserData{
public:
    static QString workspace_path;
    static QString backend_path;
    static QString path;            // The application settings path.

    static QString tcpip_hostname;
    static int tcpip_port;

    static void init(){
        path= QStandardPaths::writableLocation(QStandardPaths::DataLocation)
                                                         + "/" + UserDataFolder;
        QDir dir(path);
        if(!dir.exists())
            dir.mkpath(".");

        path = path + "/" + UserDataName;

        if(!QFile::exists(path)){
            qDebug() << "Creating file for user data.";
            save();
        }
        else{
            qDebug() << "Loading user data.";
            reload();
        }
    }

    static void save(){
        //#ifdef DEBUGGING_DIR
        //return;
        //#endif
        QFile userfile(path);
        if (!userfile.open(QIODevice::ReadWrite | QIODevice::Text)){
            qDebug() << "UserData:: Could not open " << path << " to save..";
            return;
        }

        QTextStream out(&userfile);
        out << workspace_path <<"\n";   //
        out << tcpip_hostname <<"\n";   //
        out << tcpip_port <<"\n";       //
        out << backend_path <<"\n";   //
        userfile.close();

        qDebug() <<"Saving user data...\n"
                 <<"\tworkspace_path: "<< workspace_path <<"\n"
                 <<"\ttcpip hostname: "<< tcpip_hostname <<"\n"
                 <<"\ttcpip port: "<< tcpip_port <<"\n"
                 <<"\tbackend_path: "<< backend_path <<"\n";
    }

    static void reload(){
        QFile userfile(path);
        if (!userfile.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug() << "UserData:: Could not open " << path<<" file to load..";
            return;
        }
        //while(!file.atEnd()) {
            //QByteArray line = file.readLine();
            //process_line(line);
        //}
        QTextStream in(&userfile);
        in >> workspace_path;        //
        in >> tcpip_hostname;        //
        in >> tcpip_port;            //
        in >> backend_path;          //
        userfile.close();
    }


    static void check(){
        QFile userfile(qApp->applicationDirPath() + "/" + UserDataName);
        userfile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&userfile);
        //out << "Path: " << path<<"\n";
        userfile.close();
    }


};

#endif // USERDATA_H

