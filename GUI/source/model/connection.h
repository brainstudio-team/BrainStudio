#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QTextStream>

class Connection{
private:
    QString _source, _target;
    int _preFirst, _preLast, _postFirst, _postLast;
    int _size;
    int _mode;
    QString _type, _file, _distribution;
    bool _inhibitory;
    float _probability;
    QMap<QString,QString> params;
    QMap<QString,QString> states;

    QString _version, _id;

    // ZAF SOS: Used when simulation is loaded to store the first/last id of
    // neurons in the simulation
    int _firstId, _lastId;

public:
    Connection(QString id, QString source, QString target,
               int preFirst, int preLast, int postFirst, int postLast,
               QString type): _id(id), _source(source), _target(target),
               _preFirst(preFirst), _preLast(preLast), _type(type),
               _postFirst(postFirst), _postLast(postLast){
        _probability = 1.0f;
        _distribution = "";
        _file = "";
        _version = "0.001";
        _inhibitory = false;
    }
    ~Connection(){}
    QString version()   const { return _version; }
    QString id()        const { return _id;          }
    QString source()    const { return _source;      }
    QString target()    const { return _target;      }
    int size()          const { return _size;        }
    int mode()          const { return _mode;        }
    QString type()      const { return _type;        }
    QString file()      const { return _file;        }
    int preFirst()      const { return _preFirst;    }
    int preLast()       const { return _preLast;     }
    int postFirst()     const { return _postFirst;   }
    int postLast()      const { return _postLast;    }
    int firstId()       const { return _firstId;     }
    int lastId()        const { return _lastId;      }
    float probability() const { return _probability; }
    bool inhibitory()   const { return _inhibitory; }
    bool isFull()       const { if(_mode==FULL) return true; return false; }
    bool isTopographic()const { if(_mode==TOPOGRAPHIC) return true; return false;}
    bool isProbability()const { if(_mode==PROBABILITY) return true; return false;}
    bool isDistr()      const { if(_mode==DISTRIBUTION) return true; return false;}
    bool isFromFile()   const { if(_mode==FROMFILE) return true; return false;}


    void setSource(const QString &id) { _source = id; }
    void setTarget(const QString &id) { _target = id; }

    void setParam(const QString &key, const QString &value){
        params[key]=value;
        // If the parameter is called weight and it is negative. - Checks auto-
        // matically if the value is a number, because if not, toFloat returns 0.
        if(key == "weight" && value.toFloat() < 0){
            _inhibitory = true;
        }
        else if(key == "__connectivity"){ // Set the mode!
            if(value == "full")  _mode = Connection::FULL;
            else if(value == "topographic") _mode = Connection::TOPOGRAPHIC;
            else if(value == "probability") _mode = Connection::PROBABILITY;
            else if(value == "distribution") _mode = Connection::DISTRIBUTION;
            else if(value == "file") _mode = Connection::FROMFILE;
        }
        else if(key == "__probability"){
            _probability = value.toFloat();
        }
        else if(key == "__file"){
            _file = value.toFloat();
        }
        else if(key == "__distribution"){
            _distribution = value.toFloat();
        }
    }
    void setState(const QString &key, const QString &value){ states[key]=value; }
    void setFirstId(int value){ _firstId = value; }
    void setLastId(int value){ _lastId = value; }
    void setPreFirst(int value){ _preFirst = value; }
    void setPreLast(int value){ _preLast = value; }
    void setPostFirst(int value){ _postFirst = value; }
    void setPostLast(int value){ _postLast = value; }
    void setProbability(float value){ _probability = value; }
    void setVersion(const QString &_value){ _version = _value; }

    void printParams(QTextStream &out){
        QMap<QString, QString>::iterator i;
        for(i=params.begin(); i != params.end(); i++)
            out << "\t\t\t<" << i.key() << ">" << i.value()
                << "</"<< i.key() << ">\n";
    }
    void printStates(QTextStream &out){
        QMap<QString, QString>::iterator i;
        for(i=states.begin(); i != states.end(); i++)
            out << "\t\t\t<" << i.key() << ">" << i.value()
                << "</"<< i.key() << ">\n";
    }

    // connection mode
    enum{FULL, TOPOGRAPHIC, PROBABILITY, DISTRIBUTION, FROMFILE};
};

#endif // CONNECTION_H
