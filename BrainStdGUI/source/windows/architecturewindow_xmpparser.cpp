#include "architecturewindow.h"


/*       XML terminology:
 *              <aaa>           <-- tag
 *              <aaa>           <-- element (not sure what the difference is..)
 *              <aaa bbb="..">  <-- bbb is an attribute!
 */


void ArchitectureWindow::helper1(QString name,
                                 QList< QMap<QString,QString> > &structure,
                                 QXmlStreamReader &xml){
    // If it's named 'name', parse all structures
    if(xml.name() == name){
        while(!xml.atEnd() && !xml.hasError()){
            xml.readNext();
            if(xml.name() == name){
                break;
            }
            else{
                // If not empty xml element!
                if(xml.name() != ""){
                    // Dig the information from each 'name'.
                    structure.append(this->parseInternalElement(xml));
                }
            }
        }
    }
}

void ArchitectureWindow::parse_actions(QList< QMap<QString,QString> > &structure,
                                       QXmlStreamReader &xml){
    // If it's named 'name', parse all structures
    if(xml.name() == "actions"){
        while(!xml.atEnd() && !xml.hasError()){
            xml.readNext();

            if(xml.name() == "actions"){
                break;
            }
            else{
                // If not empty xml element!
                if(xml.name() != ""){
                    // Dig the information from each action.

                    QMap<QString, QString> data;

                    // Check that we're really at the begining of an element
                    if(xml.tokenType() != QXmlStreamReader::StartElement)
                        continue;

                    data["type"] = xml.name().toString();

                    QXmlStreamAttributes attributes = xml.attributes();
                    foreach(const QXmlStreamAttribute &attr, attributes){
                        data[attr.name().toString()] = attr.value().toString();
                    }

                    structure.append(data);
                }
            }
        }
    }
}



// Open and parse given brn file
bool ArchitectureWindow::load_brn(const QString filename){
    QFile* file = new QFile(filename);

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)){
        //QMessageBox::critical(this, "MainWindow::parseXML", "Couldn't open " +
        //                      filename, QMessageBox::Ok);
        qDebug() << "ArchitectureWindow::parser: Error:"
                 << "Couldn't open" << QString(filename);
        return false;
    }

#ifndef LOGS
    qDebug() <<"ArchitectureWindow::parser: Parsing a new xml file:"<< filename;
#endif
    this->clear();

    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(file);
    QList< QMap<QString,QString> > actions;
    QList< QMap<QString,QString> > nodes;
    QList< QMap<QString,QString> > edges;
    QList< QMap<QString,QString> > textBlocks;

    controls.speed = -1;


    // Parse the XML until the end
    while(!xml.atEnd() && !xml.hasError()){

        // Read next element.
        QXmlStreamReader::TokenType token = xml.readNext();

        // If token is just StartDocument, go to next.
        if(token == QXmlStreamReader::StartDocument){
            continue;
        }

        // If token is StartElement, see if we can read it and process it!
        if(token == QXmlStreamReader::StartElement) {

            // If it's named nodes, parse all nodes
            parse_actions(actions,xml);
            helper1("nodes",nodes,xml);
            helper1("edges",edges,xml);
            helper1("textblocks",textBlocks,xml);

            // Parse controls (attributes of the tag 'schema')
            if(xml.name() == "schema"){
                controls.speed = xml.attributes().value("speed").toInt();
            }


        }
    }

    // Error handling.
    if(xml.hasError()){
        QMessageBox::critical(this, "MainWindow::parseXML", xml.errorString(),
                              QMessageBox::Ok);
        return false;
    }

    // Removes any device() or data from the reader
    // and resets its internal state to the initial state.
    xml.clear();
    this->addActions(actions);
    this->addNodes(nodes);
    this->addEdges(edges);
    this->addTextBlocks(textBlocks);

    // Restore the current mode to blocks (if reloading).
    this->loadFirstLastNeurons();
    this->setBlockMode(mode);
    this->update();

    return true;
}


/*
 *
 */
QMap<QString, QString> ArchitectureWindow::parseInternalElement(QXmlStreamReader& xml) {
    QMap<QString, QString> data;

    // Check that we're really at the begining of an element
    if(xml.tokenType() != QXmlStreamReader::StartElement)
        return data;

    QString my_name = xml.name().toString();
    data["type"] = my_name;

    // -- Get the attributes of the node ---------------------------------------
    QXmlStreamAttributes attributes = xml.attributes();
    if(attributes.value("id").toString() != ""){
        data["my_id"] = attributes.value("id").toString();
    }
    if(attributes.value("x").toString() != ""){
        data["my_x"] = attributes.value("x").toString();
    }
    if(attributes.value("y").toString() != ""){
        data["my_y"] = attributes.value("y").toString();
    }
    if(attributes.value("w").toString() != ""){
        data["my_w"] = attributes.value("w").toString();
    }
    if(attributes.value("h").toString() != ""){
        data["my_h"] = attributes.value("h").toString();
    }
    if(attributes.value("c").toString() != ""){
        data["my_c"] = attributes.value("c").toString();
    }
    // -------------------------------------------------------------------------

    // Next element...
    xml.readNext();

    // Loop over sub-elements (because the order might change) and continue
    // until we hit an EndElement with the same name as the first!
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == my_name) && !xml.atEnd()){
        if(xml.tokenType() == QXmlStreamReader::StartElement){
            this->addElementDataToMap(xml, data);
        }
        xml.readNext();
    }
    return data;
}






void ArchitectureWindow::addElementDataToMap(QXmlStreamReader& xml,
                                             QMap<QString, QString>& map)const {
    // We need a start element, like <neurons>
    if(xml.tokenType() != QXmlStreamReader::StartElement)
        return;

    // Let's read the name...
    QString elementName = xml.name().toString();

    // ...go to the next.
    xml.readNext();

    // This elements needs to contain Characters so we know it's actually data,
    // if it's not we'll leave.
    if(xml.tokenType() != QXmlStreamReader::Characters)
        return;

    // Now we can add it to the map.
    map.insert(elementName, xml.text().toString());
}



void ArchitectureWindow::addNodes(QList< QMap<QString,QString> >& data) {
    while(!data.isEmpty()){
        QMap<QString,QString> node = data.takeFirst();
        QString my_id = node.take("my_id");
        int my_size = node["neurons"].toInt();
        if(blocks.contains(my_id)){
            qDebug() << "ArchitectureWindow::addNodes: Error: Id" << my_id
                     << "exists! Could not parse the brn file!";
            exit(1);
        }
        if(!addBlock( my_id,
                      node.take("type"),
                      node.take("my_x").toInt()-5,
                      node.take("my_y").toInt()-5,
                      node.take("my_w").toInt()+10,
                      node.take("my_h").toInt()+10,
                      node.take("my_c"),
                      my_size)){
            qDebug() << "ArchitectureWindow::addNodes: Exiting..";
            return;
        }

        blocks[my_id]->setVersion(node.take("version"));

        QMap<QString, QString>::iterator i;
        for(i=node.begin(); i != node.end(); i++)
            blocks[my_id]->setParam(i.key(), i.value());

        blocks[my_id]->loadLocationsFromFile();
    }
}

void ArchitectureWindow::addEdges(QList< QMap<QString,QString> >& data){
    while(!data.isEmpty()){
        QMap<QString,QString> edge = data.takeFirst();
        connections.push_front(Connection(edge.take("my_id"),
                                          edge.take("source"),
                                          edge.take("target"),
                                          edge.take("preFirst").toInt(),
                                          edge.take("preLast").toInt(),
                                          edge.take("postFirst").toInt(),
                                          edge.take("postLast").toInt(),
                                          edge.take("type")));

        connections[0].setVersion(edge.take("version"));

        QMap<QString, QString>::iterator i;
        for(i=edge.begin(); i != edge.end(); i++)
            connections[0].setParam(i.key(), i.value());
    }
}

void ArchitectureWindow::addTextBlocks(QList< QMap<QString,QString> >& data){
    while(!data.isEmpty()){
        QMap<QString,QString> map = data.takeFirst();
        textBlocks.append(TextBlock(map["text"],
                                    map["fontSize"].toInt(),
                                    map["x"].toInt(),
                                    map["y"].toInt(),
                                    map["w"].toInt(),
                                    map["h"].toInt(),
                                    map["c"]));
    }
}

void ArchitectureWindow::addActions(QList< QMap<QString,QString> >& data){
    while(!data.isEmpty()){
        QMap<QString,QString> map = data.takeFirst();
        double new_value = 0.0;
        if(map["type"]=="adjust"){
            new_value = map["value"].toDouble();
        }
        else if(map["type"]=="stimulate"){
            new_value = map["current"].toDouble();
        }

        actions.append(Action(map["t"].toInt(),
                              map["type"],
                              map["node"],
                              new_value,
                              map["amplitude"].toDouble(),
                              map["first"].toInt(),
                              map["last"].toInt(),
                              map["parameter"],
                              map["frequency"].toDouble(),
                              map["phase"].toInt()));
    }
}

bool ArchitectureWindow::backup(QString foldername){
    QString filename = "backup_0.brn";
    int i = 1;
    while(QFile::exists(foldername+"/"+filename)){
        filename = "backup_" + QString::number(i) + ".brn";
        i++;
    }
    return this->save_brn(filename);
}


bool ArchitectureWindow::save_brn(const QString filename){
    QString file = filename;

    QFile data(file);
    if(data.open(QFile::WriteOnly | QFile::Truncate)){
        QTextStream out(&data);
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
        out << "<schema speed=\"" << QString::number(controls.speed) << "\">\n";
        out << "\t<actions>\n";
        for(int i=0;i<actions.size();i++){
            out << "\t\t<"+actions[i].type;
            out << " t=\"" << QString::number(actions[i].time) << "\"";
            if(actions[i].type == "stimulate"){
                out << " node=\"" << actions[i].node << "\"";
                if(actions[i].firstNeuron != actions[i].lastNeuron){
                    out <<" first=\""<<QString::number(actions[i].firstNeuron)<<"\"";
                    out <<" last=\""<<QString::number(actions[i].lastNeuron)<<"\"";
                }
                out <<" current=\""<<QString::number(actions[i].new_value)<<"\"";
                if(actions[i].frequency != 0.0)
                    out <<" frequency=\""<<QString::number(actions[i].frequency)<<"\"";
                if(actions[i].phase != 0.0)
                    out <<" phase=\""<<QString::number(actions[i].phase)<<"\"";
            }
            else if(actions[i].type == "adjust"){
                out << " node=\"" << actions[i].node << "\"";
                if(actions[i].firstNeuron != actions[i].lastNeuron){
                    out <<" first=\""<<QString::number(actions[i].firstNeuron)<<"\"";
                    out <<" last=\""<<QString::number(actions[i].lastNeuron)<<"\"";
                }
                out << " parameter=\"" << actions[i].parameter << "\"";
                out << " value=\""<<QString::number(actions[i].new_value)<<"\"";
            }

            out << "/>\n";
        }
        out << "\t</actions>\n";

        // ---------------------------------------------------------------------
        out << "\t<nodes>\n";
        for(BlockIter bl = blocks.begin(); bl != blocks.end(); bl++){
            out << "\t\t<" << bl.value()->getType()
                <<  " id=\"" << bl.key()
                << "\" x=\"" << QString::number(bl.value()->getInitialX())
                << "\" y=\"" << QString::number(bl.value()->getInitialY())
                << "\" w=\"" << QString::number(bl.value()->width()-10)
                << "\" h=\"" << QString::number(bl.value()->height()-10)
                << "\" c=\"" << bl.value()->getColour() << "\">\n";
            out << "\t\t\t<version>" << bl.value()->getVersion() << "</version>\n";
            //out << "\t\t\t<neurons>" << bl.value()->getNeuronsSize() << "</neurons>\n";

            bl.value()->printParams(out);
            bl.value()->printStates(out);

            out << "\t\t</" << bl.value()->getType() << ">\n";
        }
        out << "\t</nodes>\n";

        out << "\t<edges>\n";
        for(int i=(connections.size()-1);i>=0;i--){
            out << "\t\t<" << connections[i].type()
                <<  " id=\"" << connections[i].id() << "\">\n";
            out << "\t\t\t<source>" << connections[i].source() << "</source>\n";
            out << "\t\t\t<target>" << connections[i].target() << "</target>\n";
            out << "\t\t\t<preFirst>" << QString::number(connections[i].preFirst()) << "</preFirst>\n";
            out << "\t\t\t<preLast>" << QString::number(connections[i].preLast()) << "</preLast>\n";
            out << "\t\t\t<postFirst>" << QString::number(connections[i].postFirst()) << "</postFirst>\n";
            out << "\t\t\t<postLast>" << QString::number(connections[i].postLast()) << "</postLast>\n";
            out << "\t\t\t<version>" << connections[i].version() << "</version>\n";

            connections[i].printParams(out);
            connections[i].printStates(out);
            out << "\t\t</" << connections[i].type() << ">\n";
        }
        out << "\t</edges>\n";

        out << "\t<textBlocks>\n";
        for(int i=(textBlocks.size()-1);i>=0;i--){
            out << "\t\t<textBlock>\n";
            out << "\t\t\t<text>" << textBlocks[i].text() << "</text>\n";
            out << "\t\t\t<fontSize>" << QString::number(textBlocks[i].fontSize()) << "</fontSize>\n";
            out << "\t\t\t<x>" << QString::number(textBlocks[i].x()) << "</x>\n";
            out <<       "<y>" << QString::number(textBlocks[i].y()) << "</y>\n";
            out << "\t\t\t<width>" << QString::number(textBlocks[i].width()) << "</width>\n";
            out <<      "<height>" << QString::number(textBlocks[i].height()) << "</height>\n";
            out << "\t\t\t<colour>" << textBlocks[i].colour() << "</colour>\n";
            out << "\t\t</textBlock>\n";
        }
        out << "\t</textBlocks>\n";
        out << "</schema>\n";
        return true;
    }
    else
        return false;
}

/* SOS:
 *   I need to add this every time that I change the size of neurons in a block!
 */
void ArchitectureWindow::loadFirstLastNeurons(){
    int neurons=0;
    for(BlockIter bl = blocks.begin(); bl != blocks.end(); bl++){
        bl.value()->setFirstNeuronIdx(neurons);
        neurons += bl.value()->getNeuronsSize();
        bl.value()->setLastNeuronIdx(neurons);
    }
}











