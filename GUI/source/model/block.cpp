#include "block.h"

Block::Block(QString _id, QString _type, bool _spiking,
             int _x, int _y, int _w, int _h, QString _col,
             int _size, QWidget *parent ): QWidget(parent), old_id(_id),
             stimulus(0, _size), highlighted(false), mode(modeEdit),
             initial_x(_x), initial_y(_y), WINDOW_X(0), WINDOW_Y(0),
             connecting(false), spiking(_spiking){

    state.resize(_size);
    firingRateNeuron.resize(_size);

    param_for_neurons = "neurons";                                              // ZAF SOS: TODO: This is not general!

#ifndef LOGS
    qDebug() <<"Block::Block: "<<_type <<" created";
#endif
    this->move(_x, _y);
    this->resize(_w, _h);

    graph = NULL;
    plotLayout = new QVBoxLayout(this);
    this->setLayout(plotLayout);

    plotFrame = new QFrame(this);
    plotFrame->show();

    version = "0.001";
    LFP = 0;
    LFPcalculated = false;
    colour = _col;
    firstNeuronIdx = lastNeuronIdx = 0;

    if(!this->isVisible())
        this->setVisible(true);

    // Create actions
    cutAct = new QAction(QIcon(":/new/prefix1/icons/cut.png"), tr("Cu&t"),this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut this block to the clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct=new QAction(QIcon(":/new/prefix1/icons/copy.png"),tr("&Copy"),this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current block to the clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/new/prefix1/icons/paste.png"), tr("&Paste"),
                           this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Replace the current selection with the "
                              "clipboard's contents"));
    //connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    newPlotAct=new QAction(QIcon(":/new/prefix1/icons/addPlot.png"),
                          tr("New plo&t"),this);
    newPlotAct->setStatusTip(tr("Add a new plot for this block"));
    connect(newPlotAct, SIGNAL(triggered()), this, SLOT(newPlotSlot()));

    stimulationAct=new QAction(QIcon(":/new/prefix1/icons/injection.png"),
                          tr("&Stimulation"),this);
    stimulationAct->setStatusTip(tr("Stimulate the block with external "
                                    "current."));
    connect(stimulationAct, SIGNAL(triggered()), this, SLOT(stimulationSlot()));

    propertiesAct=new QAction(QIcon(":/new/prefix1/icons/addSynapse.png"),
                          tr("&Properties"),this);
    propertiesAct->setStatusTip(tr("View and change the properties of this "
                                   "block"));
    connect(propertiesAct, SIGNAL(triggered()), this, SLOT(propertiesSlot()));

    newConAct=new QAction(QIcon(":/new/prefix1/icons/addSynapse.png"),
                          tr("&New connection"),this);
    newConAct->setStatusTip(tr("Create a new synaptic connection from this "
                               "block"));
    connect(newConAct, SIGNAL(triggered()), this, SLOT(newConnectionSlot()));
    deleteAct=new QAction(QIcon(":/new/prefix1/icons/delete_32.png"),
                          tr("&delete block"),this);
    //deleteAct->setShortcuts(QKeySequence::Delete);
    deleteAct->setStatusTip(tr("Delete the current block and all its "
                               "connections"));
    connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteBlockSlot()));


    // Controls
    idLineEdit = new QLineEdit(_id, this);
    idLineEdit->setStyleSheet("QLineEdit:!focus"
                                "{ border: 1px solid transparent;"
                                "  font: bold 14px;"
                                "  color: black;"
                                "  background: transparent; }"
                                "QLineEdit:focus"
                                "{ background: white; }");
    idLineEdit->move(8, 7);
    idLineEdit->setMinimumWidth(this->width()-20);
    idLineEdit->setVisible(true);
    connect(idLineEdit, SIGNAL(editingFinished()),
            this, SLOT(idChanged()));

    sizeLineEdit = new QLineEdit(QString::number(state.size()), this);
    sizeLineEdit->setValidator( new QIntValidator(0, 1000000, this) );
    sizeLineEdit->setStyleSheet("QLineEdit:!focus"
                                "{ border: 1px solid transparent;"
                                "  font: 14px;"
                                "  color: black;"
                                "  background: transparent; }"
                                "QLineEdit:focus"
                                "{ background: white; }");
    sizeLineEdit->move(48, 26);
    sizeLineEdit->resize(60, 20);
    connect(sizeLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(sizeChanged(QString)));
    sizeLineEdit->setVisible(true);

    type = _type;
    /*typeComboBox = new QComboBox(this);
    typeComboBox->setStyleSheet(
                "QComboBox:!focus"
                "{ border: transparent;"
                "  font: 14px;"
                "  color: black;"
                "padding: 1px 0px 1px 3px;" // SOS: This makes text colour work
                "  background: transparent; }"
                "QComboBox::drop-down { border: 0px; }"
                "QComboBox:focus { background: white; }");
    typeComboBox->move(47, 43);
    typeComboBox->setVisible(true);
    typeComboBox->resize(200, typeComboBox->height());

    int current_index = -1, temp_idx=0;
    QMap<QString, BackendData::my_list>::iterator i;
    for(i = BackendData::node_params.begin();
        i != BackendData::node_params.end(); i++){
        typeComboBox->addItem(i.key());
        if(i.key() == _type)
            current_index = temp_idx;
        temp_idx++;
    }
    if(current_index == -1){
        qDebug() << "Block::Block: Problem with the type: "
                    "not found in BackendData!";
        typeComboBox->setCurrentText(_type);
    }
    else{
        typeComboBox->setCurrentIndex(current_index);
    }
    connect(typeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(typeChanged(int)));*/
}

Block::~Block(){
    //this->setMode(-1); <-- This should be wrong since graph is a child qwidget
}

void Block::paintEvent(QPaintEvent * event){
    QPainter painter(this);

    if(connecting){
        painter.setPen(QPen(Qt::black,1));
        painter.setBrush(QBrush(QColor(100,100,100,180)));
        painter.drawRect(0, 0, width(), height());
    }
    if(highlighted){
        painter.setPen(QPen(QColor(80,80,80),3,Qt::DotLine));
        painter.setBrush(QBrush(QColor(0,0,0,0)));
        painter.drawRect(2, 2, width()-4, height()-4);
    }

    if(mode == modeStatesPixels){
        if(state.size()>0){
            int xx,yy,nn,j;
            float ww,hh;
            double sqrtSize;

            sqrtSize = sqrt((double)state.size());
            nn = (int)sqrtSize;
            if( (float)nn != sqrtSize ) // If sqrt is not an integer!
                nn++;

            if(nn*(nn-1) >= state.size()) // A little hack
                ww=(float)(width()-10)/(float)(nn-1);
            else
                ww=(float)(width()-10)/(float)nn;
            hh=(float)(height()-10)/(float)nn;

            for(j=0; j<state.size(); j++){ // for each neuron
                xx = 5 + ((float)(j/nn))*ww;
                yy = 5 + ((float)(j%nn))*hh;
                painter.setBrush(QBrush(QColor(getNeuronColour(j))));
                painter.setPen(QPen(QColor(getNeuronColour(j))));
                painter.drawRect(xx,yy,ww,hh);
            }
        }

        painter.setBrush(QBrush(QColor(0,0,0,0)));
        painter.setPen(QPen(Qt::black));
        painter.drawRect(5, 5, width()-10, height()-10);
    }
    else if(mode == modeEdit){
        painter.setPen(QPen(Qt::black));
        painter.setBrush(QBrush(QColor(colour)));
        painter.drawRect(5, 5, width()-10, height()-10);

        //painter.drawText(10, 20, id);
        painter.drawText(10, 41, "Units: ");//+QString::number(state.size()));
        if(this->height() >= 70){
            painter.drawText(10, 58, "Type: " + getType());
        }

        // Controls
        if(highlighted){
            QPixmap pixmap(":/new/prefix1/icons/add_con.png");
            painter.drawPixmap(this->width()-35, this->height()-35, 30, 30, pixmap);

            QPixmap pixmap2(":/new/prefix1/icons/injection.png");
            painter.drawPixmap(this->width()-35, this->height()-70, 30, 30, pixmap2);
        }

    }
    else if(mode == modeC){
        // nn = real part of the sqrt of the number of neurons
        if(state.size()>0){
            int xx,yy,nn,j;
            float ww,hh;
            double sqrtSize;

            if(params.contains("spatiallyEmbedded") &&
                    params["spatiallyEmbedded"].toInt()){
                ww = (float)(width()-10) / (float)state.size();
                hh = (float)(height()-10);
                yy = 5;

                for(j=0; j<state.size(); j++){ // for each neuron
                    xx = (int)(5 + ((float)j)*ww);
                    painter.setBrush(QBrush(QColor(getFiringRateNeuronColour(j))));
                    painter.setPen(    QPen(QColor(getFiringRateNeuronColour(j))));
                    painter.drawRect(xx, yy, (int)ww, (int)hh);
                }
            }
            else{
                sqrtSize = sqrt((double)state.size());
                nn = (int)sqrtSize;
                if( (float)nn != sqrtSize ) // If sqrt is not an integer!
                    nn++;

                if(nn*(nn-1) >= state.size()) // A little hack
                    ww=(float)(width()-10)/(float)(nn-1);
                else
                    ww=(float)(width()-10)/(float)nn;
                hh=(float)(height()-10)/(float)nn;

                for(j=0; j<state.size(); j++){ // for each neuron
                    xx = 5 + ((float)(j/nn))*ww;
                    yy = 5 + ((float)(j%nn))*hh;
                    painter.setBrush(QBrush(QColor(getFiringRateNeuronColour(j))));
                    painter.setPen(QPen(QColor(getFiringRateNeuronColour(j))));
                    painter.drawRect(xx, yy, (int)ww, (int)hh);
                }
            }
        }

        painter.setBrush(QBrush(QColor(0,0,0,0)));
        painter.setPen(QPen(Qt::black));
        painter.drawRect(5, 5, width()-10, height()-10);
    }
    else if(mode == modeRasters){
        painter.setPen(QPen(Qt::black));
        if(this->spiking)
            painter.setBrush(QBrush(QColor("#ffffbb")));
        else
            painter.setBrush(QBrush(QColor("#bbffbb")));
        painter.drawRect(5, 5, width()-10, height()-10);
    }
    else if(mode == modeStatesPlots){ // Current plots
        painter.setPen(QPen(Qt::black));
        painter.setBrush(QBrush(QColor(colour)));
        painter.drawRect(5, 5, width()-10, height()-10);

        //graph->draw(&painter);
    }

    // Controls
    if(highlighted){
        painter.setPen(QPen(Qt::black));
        painter.setBrush(QBrush(QColor(0,0,0)));
        painter.drawRect(this->width()-7, this->height()-7, 7, 7);
        painter.drawRect(this->width()-7, this->height()/2, 7, 7);
        painter.drawRect(this->width()/2, this->height()-7, 7, 7);
    }

}


void Block::setFiringRateDepth(const int &value){
    for(i=0;i<firingRateNeuron.size();i++){
        firingRateNeuron[i].setDepth(value);
    }
    firingRateBlock.setDepth(value);
}


#define MAX_COL_VAL 150.0f
#define MIN_COL_VAL -100.0f

QColor Block::getNeuronColour(int indx){
    if(indx<state.size()){
        int t=(int)(255.0f*(state[indx]-MIN_COL_VAL)/(MAX_COL_VAL-MIN_COL_VAL));
        if(t>255) t=255;
        else if(t<0) t=0;
        return QColor(t,0,255-t);
    }
    else
        return QColor(0,0,0);
}

#define COL_SCALING_FACTOR 7.0f

QColor Block::getFiringRateNeuronColour(int indx){
    if(indx<firingRateNeuron.size()){
        int t = (int)(255.0f*COL_SCALING_FACTOR*firingRateNeuron[indx].getFR());
        if(t>255) t=255;
        return QColor(t,0,255-t);
    }
    else
        return QColor(0,0,0);
}

QColor Block::getFiringRateBlockColour(const int &targetBlock){
    int t = (int)(255.0f*COL_SCALING_FACTOR*firingRateBlock.getFR());
    if(t>255) t=255;
    if(blockConnectionAverageWeight[targetBlock] > 0)
        return QColor(t,0,0,t);
    else
        return QColor(0,0,t,t);
}

QList<float> Block::getWindowedFiringRate(){
    return firingRateBlock.getLastValues();
}

void Block::setMode(int value){
    if(value == mode){
        qDebug() << "Block::setMode: The mode already " << value
                 << " (Returning..)";
        return;
    }
    if(graph != NULL){
        plotLayout->removeWidget(graph);
        delete graph;
        graph = NULL;
    }

    if(value == modeEdit){
        idLineEdit->setVisible(true);
        sizeLineEdit->setVisible(true);
        //typeComboBox->setVisible(true);
    }
    else{
        idLineEdit->setVisible(false);
        sizeLineEdit->setVisible(false);
        //typeComboBox->setVisible(false);
    }

    if(value == modeStatesPlots){
        graph = new CurrentGraph("", state.size(), 0, 200, 5, 5, width()-10,
                                 height()-10, Qt::red, Qt::blue, 500, this);
        plotLayout->addWidget(graph);
        graph->setVisible(true);
    }
    else if(value == modeRasters){
        if(this->spiking){
            graph = new RasterPlot("", state.size(), Qt::black, plotFrame);
            plotLayout->addWidget(graph);
            graph->setVisible(true);
        }
        else{
            graph = new Plot2D("", state.size(), 0, 0.5, 5, 5, width()-10,
                               height()-10, Qt::red, Qt::blue, 500, plotFrame);
            plotLayout->addWidget(graph);
            graph->setVisible(true);
        }
    }
    mode = value;
}


void Block::updateMe(){
    LFPcalculated = false;
    if(graph != NULL){
        if(mode == modeStatesPlots){
            for(up_i=0; up_i<state.size(); up_i++)
                graph->add_new_value(up_i, state[up_i]+120);
        }
        else if(!spiking && mode == modeRasters){
            for(up_i=0; up_i<state.size(); up_i++)
                graph->add_new_value(up_i, state[up_i]);
        }
    }
}


void Block::updateRaster(){
    if(graph == NULL){
        qDebug() << "Block::updateRaster: Error: graph is NULL!";
        return;
    }
   for(i=0; i<getNeuronsSize(); i++)
        if(fired(i))
            graph->add_spike(i);
    //graph->next_step(frequencyBlock());
   graph->next_step();
}


void Block::setSpikeArray(unsigned *spikesArray, const int spikes){
    if(spikesArray == NULL && spikes == -1){
        qDebug() << "firstNeur: " << firstNeuronIdx
                 << " Ptr: " << spikesArray;
        return;
    }

    for(up_i=0; up_i<spikes; up_i++){
        //graph->add_spike(spikesArray[up_i]-firstNeuronIdx);
        int temp = spikesArray[up_i]-firstNeuronIdx;

        if(0 <= temp && temp < state.size()) // SOS!! WRONG!!
            graph->add_spike(temp);
        else{
            qDebug()<<"SOS: WRONG!!("<<temp<<")\tAdding spike: "
                    <<spikesArray[up_i] - firstNeuronIdx
                    <<" spikesArray[ " << spikes << "]: " << spikesArray[up_i]
                    <<" up_i: " << up_i << " firstNeuronIdx: " << firstNeuronIdx
                    <<" Pointer: " << spikesArray;
            for(int i=0;i<spikes;i++)qDebug()<<i<<")"<<spikesArray[i];

        }
    }
    graph->next_step();
}

void Block::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu(this);
    menu.addAction(newPlotAct);
    menu.addAction(propertiesAct);
    menu.addAction(stimulationAct);
    menu.addSeparator();
    menu.addAction(newConAct);
    menu.addSeparator();
    menu.addAction(cutAct);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.addSeparator();
    menu.addAction(deleteAct);
    menu.exec(event->globalPos());
}


void Block::loadLocationsFromFile(){
    if(neuronsLoc.empty() && params.contains("fileLocations")
            && params["fileLocations"] != "")
        loadLocations(params["fileLocations"]);
}

void Block::loadLocations(QVector<NeuronUnit> *locations){
    if(locations != NULL){
        for(int i=0; i<locations->size(); i++)
            neuronsLoc.append(locations->value(i));
    }
}

void Block::loadLocations(const QString &filename){
    if(filename != ""){
        QFile file(/*FOLDERNAME + "/" + */filename);
        if(!file.open(QIODevice::ReadOnly)){
            qDebug() << "Cannot open " << filename
                     << " for reading: " << qPrintable(file.errorString());
            //if(!this->isVisible())  this->setVisible(true);
            return;
        }
        QTextStream in(&file);

        float xx,yy,zz;
        while(!in.atEnd()){
            in >> xx >> yy >> zz;
            neuronsLoc.append(NeuronUnit(xx, yy, zz));
        }
        file.close();
    }
}









/*      current = max*t + min(1-t)
        current = max*t + min - min*t
        current = (max-min)*t + min
        current-min = (max-min)*t

             current-min
        t = --------------
               max-min
*/





void Block::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        qDebug() << "Block:" << "Left click";

        // If the user pressed the 'new connection' button
        if( (mode == modeEdit) && this->highlighted
                                     && event->pos().x() >= this->width() - 35
                                     && event->pos().y() >= this->height()- 35
                                     && event->pos().x() <  this->width() - 5
                                     && event->pos().y() <  this->height()- 5){
            this->newConnectionSlot();
        }
        // If the user pressed the 'new connection' button
        else if( (mode == modeEdit) && this->highlighted
                                     && event->pos().x() >= this->width() - 35
                                     && event->pos().y() >= this->height()- 70
                                     && event->pos().x() <  this->width() - 5
                                     && event->pos().y() <  this->height()- 40){
            emit stimulationSignal();
        }
        else if( this->highlighted   && event->pos().x() >= this->width() - 7
                                     && event->pos().y() >= this->height()- 7
                                     && event->pos().x() <  this->width()
                                     && event->pos().y() <  this->height()){
            emit init_block_resize_signal(event->pos(), "bottomright");
        }
        else if( this->highlighted   && event->pos().x() >= this->width() - 7
                                     && event->pos().y() >= this->height()/2
                                     && event->pos().y() <  this->height()/2+7){
            emit init_block_resize_signal(event->pos(), "right");
        }
        else if( this->highlighted   && event->pos().x() >= this->width()/2
                                     && event->pos().y() >= this->height()- 7
                                     && event->pos().x() <  this->width()/2+7){
            emit init_block_resize_signal(event->pos(), "bottom");
        }
        else{
            emit highlighted_signal(this->idLineEdit->text());
            emit init_block_motion_signal(event->pos().x(),event->pos().y());
        }
    }
    else if(event->button() == Qt::RightButton){
        qDebug() << "Block:" << "right click";
    }
    this->update();
}

void Block::mouseDoubleClickEvent(QMouseEvent *event){
    qDebug() << "Block:" << "Double click";
    if(event->button() == Qt::LeftButton){
        emit doubleClickSignal(this->idLineEdit->text());
    }
}






















