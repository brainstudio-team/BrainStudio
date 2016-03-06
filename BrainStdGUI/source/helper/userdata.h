/* The file that stores the preferences of the user
**
**
**/
#ifndef USERDATA_H
#define USERDATA_H

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QApplication>
#include <QDesktopServices>
#include <QDir>

#define UserDataFolder  "BrainStdudio"
#define UserDataConfigFile    "config.ini"


class UserData{
public:
    enum {BCKEND_INTERNAL, BCKEND_SERVER, BCKEND_OTHER};

    // Attributes not shown to the user:
    static QString only_path;       // The application settings path
    static QString file;            // The application settings file
    static QString last_file;       // The most recent file that was loaded

    // Attributes shown to the user and saved in UserDataConfigFile
    static QString workspace_path;  // The path for the main saving folder
    static QString tcpip_hostname;  // The name of the server of the backend
    static int tcpip_port;          // The IP of the server of the backend
    static QString backend_path;    // The path to the backend part of BS
    static int backend_type;        // Type of backend that is currently used

    static void init(){
        file = QStandardPaths::writableLocation(QStandardPaths::DataLocation)
                                                         + "/" + UserDataFolder;
        only_path = file;
        QDir dir(file);
        if(!dir.exists())
            dir.mkpath(".");

        file = file + "/" + UserDataConfigFile;

        if(!QFile::exists(file)){
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
        QFile userfile(file);
        if (!userfile.open(QIODevice::ReadWrite | QIODevice::Text)){
            qDebug() << "UserData:: Could not open " << file << " to save..";
            return;
        }

        QTextStream out(&userfile);
        out << workspace_path <<"\n";   //
        out << tcpip_hostname <<"\n";   //
        out << tcpip_port <<"\n";       //
        out << backend_path <<"\n";     //
        out << backend_type <<"\n";     //
        out << last_file <<"\n";     //
        userfile.close();

        qDebug() <<"Saving user data...\n"
                 <<"\tworkspace_path: "<< workspace_path <<"\n"
                 <<"\ttcpip hostname: "<< tcpip_hostname <<"\n"
                 <<"\ttcpip port: "<< tcpip_port <<"\n"
                 <<"\tbackend_path: "<< backend_path <<"\n"
                 <<"\tbackend_type: "<< backend_type <<"\n"
                 <<"\tlast_file: "<< last_file <<"\n";
    }

    static void reload(){
        QFile userfile(file);
        if (!userfile.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug() << "UserData:: Could not open " << file<<" file to load..";
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
        in >> backend_type;          //
        in >> last_file;             //
        userfile.close();
    }


    static void check(){
        QFile userfile(qApp->applicationDirPath() + "/" + UserDataConfigFile);
        userfile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&userfile);
        //out << "Path: " << path<<"\n";
        userfile.close();
    }


};

#endif // USERDATA_H

