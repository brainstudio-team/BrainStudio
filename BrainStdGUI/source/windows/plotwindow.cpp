#include "plotwindow.h"
#include "ui_plotwindow.h"

PlotWindow::PlotWindow(QString _highlighted, ArchitectureWindow *_schema,
                       Simulator *_A, const int &_my_index,
                       QWidget *parent):highlighted(_highlighted),
                       QDialog(parent), ui(new Ui::PlotWindow){

    my_index = _my_index;
    schema = _schema;
    A = _A;
    timer = 0;
    rasterPlot = NULL;
    memPotGraph = NULL;
    ui->setupUi(this);


    if(highlighted == ""){
        qDebug() << "PlotWindow::PlotWindow: Error: Highlighted =\"\" !!!";
        emit plotWindowDeleteRequest(my_index);
        return;
    }

    this->setWindowTitle(highlighted);


    LFPgraph = new CurrentGraph("", 1, -100, 100, Qt::green, Qt::blue, 500,
                                ui->plotFrame);
    ui->plotLayout->addWidget(LFPgraph);
    LFPgraph->setVisible(false);
    MFRgraph = new CurrentGraph("", ui->channelsSpinBox->value(), 0,1, Qt::blue,
                                Qt::red,500,ui->plotFrame);
    ui->plotLayout->addWidget(MFRgraph);
    MFRgraph->setVisible(false);
    ui->channelsSpinBox->setVisible(false);
    ui->depthSpinBox->setVisible(false);
    ui->updateRateSpinBox->setVisible(false);
    ui->rescaleButton->setVisible(false);
    ui->lineThicknessSpinBox->setVisible(false);
    ui->verticalCheckBox->setVisible(false);

    for(i=0; i<10; i++)
        MFR[i].setDepth(ui->depthSpinBox->value());
    MFRmax = 0.0;

    PPgraph = new PhasePortraitGraph(1,-100,50,-350,1000,15,15,800,400,//this->width(),150,
                                   Qt::blue, Qt::red,200,ui->plotFrame);
    ui->plotLayout->addWidget(PPgraph);
    PPgraph->setVisible(false);

    this->setAutoFillBackground(true);
    this->setBackgroundRole(QPalette::Light);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->on_graphComboBox_currentIndexChanged(0);
}

PlotWindow::~PlotWindow(){
    delete ui;
}


void PlotWindow::resizeEvent(QResizeEvent * event ){
    ui->optionsFrame->setMinimumSize(this->width(), 32);
}


/*void PlotWindow::setHighlighted(QString value){
    if(!active){
        highlighted = value;
        if(value >=0){
            this->setWindowTitle(schema->blocks[value]->getName());
        }
        else{
            this->setWindowTitle("no block");
        }

        if(tab != NULL)
            plot = tab->getPlot();
    }
}*/


void PlotWindow::updateActivePlot(){
    if(ui->graphComboBox->currentIndex() == 0){
        for(int i=0; i<schema->blocks[highlighted]->getNeuronsSize(); i++)
            if(schema->blocks[highlighted]->fired(i))
                rasterPlot->add_spike(i);
        rasterPlot->next_step(schema->blocks[highlighted]->frequencyBlock());
    }
    else if(ui->graphComboBox->currentIndex() == 1){
        for(i=0; i<schema->blocks[highlighted]->getNeuronsSize() &&
                                                           i<MAX_LINES; i++)
            memPotGraph->add_new_value(i, A->getNeuronState(
                     schema->blocks[highlighted]->getFirstNeuronIdx()+i) + 120);
    }
    else if(ui->graphComboBox->currentIndex() == 3){ // Phase portrait
        PPgraph->add_new_value(0,
                A->getRealNeuronStateV(schema->blocks[highlighted]->
                                                       getFirstNeuronIdx()),
                A->getRealNeuronStateU(schema->blocks[highlighted]->
                                                      getFirstNeuronIdx()));
    }
    else if(ui->graphComboBox->currentIndex() == 4){
        if(MFRgraph != NULL){
            if(ui->updateRateSpinBox->value() > 1){
                timer++;
                if(timer >= ui->updateRateSpinBox->value()) timer = 0;
            }
            if(!timer){
                for(i=0; i<ui->channelsSpinBox->value(); i++){
                    SUM = 0.0;
                    channelSize = schema->blocks[highlighted]->getNeuronsSize()
                                                  /ui->channelsSpinBox->value();
                    for(j=0; j<channelSize; j++)
                        if(schema->blocks[highlighted]->fired(i*channelSize+j))
                            SUM += 1.0;
                    // Get the average spikes of this channel
                    SUM/=(double)channelSize;
                    MFR[i].nextFR(SUM);
                    //std::cout << MFR[i].getFR() << std::endl;
                    if(MFRmax < MFR[i].getFR())
                        MFRmax = MFR[i].getFR();
                    MFRgraph->add_new_value(i, MFR[i].getFR());
                }
                MFRgraph->setMaxValue(MFRmax);
            }
        }
        else
            qDebug() << "PlotWindow::updateActivePlot: Warning: "
                        "MFRgraph is NULL!!";
    }
    //update Local Field Potential graph:
    if(LFPgraph != NULL)
        LFPgraph->add_new_value(0, schema->LFPvalue[highlighted]);
    else
        qDebug() << "PlotWindow::updateActivePlot: Warning: LFPgraph is NULL!!";
    this->update();
}

void PlotWindow::keyPressEvent(QKeyEvent * event){
    if(event->key()==Qt::Key_Q){
        emit plotWindowDeleteRequest(my_index);
    }
}


void PlotWindow::on_channelsSpinBox_valueChanged(int arg1){
    // If mean firing rate
    if(ui->graphComboBox->currentIndex() == 4){
        delete MFRgraph;
        MFRgraph = new CurrentGraph("", arg1, 0, 1, Qt::blue, Qt::red, 500,
                                    ui->plotFrame);
        ui->plotLayout->addWidget(MFRgraph);
        MFRgraph->setVisible(true);
    }
    // If membrane potential
    else if(ui->graphComboBox->currentIndex() == 1){
        if(memPotGraph != NULL)
            delete memPotGraph;
        memPotGraph = new CurrentGraph("", arg1, 0, 200, Qt::red, Qt::blue, 500,
                                       ui->plotFrame);
        ui->depthSpinBox->setValue(memPotGraph->getNumberOfPoints());
        ui->plotLayout->addWidget(memPotGraph);
        memPotGraph->setVisible(true);
    }
}

void PlotWindow::on_depthSpinBox_valueChanged(int arg1){
    // If mean firing rate
    if(ui->graphComboBox->currentIndex() == 4){
        for(i=0; i<10; i++)
            MFR[i].setDepth(arg1);
    }
    // If membrane potential
    else if(ui->graphComboBox->currentIndex() == 1){
        memPotGraph->setNumberOfPoints(arg1);
    }

}



void PlotWindow::printToJpeg(){
    if(rasterPlot == NULL){
        qDebug() << "PlotWindow::printToJpeg: Error: rasterPlot = null!";
        return;
    }

    QImage image(100 + rasterPlot->getTime(),
                 400 + rasterPlot->width(),
                 QImage::Format_ARGB32_Premultiplied);

    image.fill(QColor(Qt::white).rgb());
    QPainter painter(&image);
    rasterPlot->print(&painter);

    LFPgraph->setPrintAxes(50,
                           200+rasterPlot->width(),
                           70 +rasterPlot->getTime(),
                           200+rasterPlot->width(),// + 300,
                           50 +rasterPlot->getTime(),
                           50 + 300,
                           50 +rasterPlot->getTime(),
                           220+rasterPlot->width());// + 300);

    LFPgraph->print(&painter);

    QString str = UserData::workspace_path + "/";
    str += highlighted;
    QString file_extension = "0.png";
    int i = 1;
    while(QFile::exists(str+file_extension)){
        file_extension = QString::number(i) + ".png";
        i++;
    }
    image.save(str + file_extension, "PNG");
}

void PlotWindow::on_printButton_clicked(){
    printToJpeg();
}

void PlotWindow::on_graphComboBox_currentIndexChanged(int index){
    int x, y, minX, minY;
    switch(index){
    case 0: // RASTER PLOT
        if(memPotGraph != NULL)
            memPotGraph->setVisible(false);
        LFPgraph->setVisible(false);
        PPgraph->setVisible(false);
        MFRgraph->setVisible(false);
        ui->channelsSpinBox->setVisible(false);
        ui->depthSpinBox->setVisible(false);
        ui->updateRateSpinBox->setVisible(false);
        ui->rescaleButton->setVisible(false);
        ui->lineThicknessSpinBox->setVisible(false);
        ui->verticalCheckBox->setVisible(true);
        if(rasterPlot != NULL)
            delete rasterPlot;
        rasterPlot = new RasterPlot("", schema->blocks[highlighted]->
                                    getNeuronsSize(), Qt::blue, ui->plotFrame);
        ui->plotLayout->addWidget(rasterPlot);
        rasterPlot->setVisible(true);
        x = 400;
        y = 200;
        minX = 330;
        minY = 170;
        break;
    case 1: // MEMBRANE POTENTIAL GRAPH
        LFPgraph->setVisible(false);
        PPgraph->setVisible(false);
        MFRgraph->setVisible(false);
        if(rasterPlot != NULL)
            rasterPlot->setVisible(false);
        ui->channelsSpinBox->setVisible(true);
        ui->channelsSpinBox->setToolTip("Number of neurons");
        ui->depthSpinBox->setVisible(true);
        ui->depthSpinBox->setToolTip("Number of points (depth of graph)");
        ui->updateRateSpinBox->setVisible(false);
        ui->rescaleButton->setVisible(false);
        ui->lineThicknessSpinBox->setVisible(false);
        ui->verticalCheckBox->setVisible(false);
        if(memPotGraph != NULL)
            delete memPotGraph;
        memPotGraph = new CurrentGraph("", schema->blocks[highlighted]->
                                       getNeuronsSize(), 0, 200, Qt::red,
                                       Qt::blue, 500, ui->plotFrame);
        ui->depthSpinBox->setValue(memPotGraph->getNumberOfPoints());
        ui->channelsSpinBox->setValue(memPotGraph->getLines());
        ui->plotLayout->addWidget(memPotGraph);
        memPotGraph->setVisible(true);
        x = 400;
        y = 200;
        minX = 330;
        minY = 170;
        break;
    case 2: // LOCAL FIELD POTENTIAL GRAPH
        if(rasterPlot != NULL)
            rasterPlot->setVisible(false);
        if(memPotGraph != NULL)
            memPotGraph->setVisible(false);
        PPgraph->setVisible(false);
        MFRgraph->setVisible(false);
        ui->channelsSpinBox->setVisible(false);
        ui->depthSpinBox->setVisible(false);
        ui->updateRateSpinBox->setVisible(false);
        ui->rescaleButton->setVisible(false);
        ui->lineThicknessSpinBox->setVisible(false);
        ui->verticalCheckBox->setVisible(false);
        LFPgraph->setVisible(true);
        x = 400;
        y = 200;
        minX = 330;
        minY = 170;
        break;
    case 3: // PHASE PORTRAIT
        if(rasterPlot != NULL)
            rasterPlot->setVisible(false);
        if(memPotGraph != NULL)
            memPotGraph->setVisible(false);
        LFPgraph->setVisible(false);
        MFRgraph->setVisible(false);
        ui->channelsSpinBox->setVisible(false);
        ui->depthSpinBox->setVisible(false);
        ui->updateRateSpinBox->setVisible(false);
        ui->rescaleButton->setVisible(false);
        ui->lineThicknessSpinBox->setVisible(false);
        ui->verticalCheckBox->setVisible(false);
        PPgraph->setVisible(true);
        x = 400;
        y = 200;
        minX = 330;
        minY = 170;
        break;
    case 4: // MEAN FIRING RATE
        if(rasterPlot != NULL)
            rasterPlot->setVisible(false);
        if(memPotGraph != NULL)
            memPotGraph->setVisible(false);
        LFPgraph->setVisible(false);
        PPgraph->setVisible(false);
        MFRgraph->setVisible(true);
        ui->channelsSpinBox->setVisible(true);
        ui->channelsSpinBox->setToolTip("Number of channels in this block "
                                        "(neural ensemble).");
        ui->channelsSpinBox->setValue(MFRgraph->getLines());
        ui->depthSpinBox->setVisible(true);
        ui->depthSpinBox->setToolTip("The depth of the firing rates.");
        ui->depthSpinBox->setValue(MFR[0].getDepth());
        ui->updateRateSpinBox->setVisible(true);
        ui->rescaleButton->setVisible(true);
        ui->lineThicknessSpinBox->setVisible(true);
        ui->verticalCheckBox->setVisible(false);
        x = 600;
        y = 200;
        minX = 600;
        minY = 170;
        break;
    default:
        qDebug() << "PlotWindow::on_graphComboBox_currentIndexChanged: Error: "
                    "index was not recognized!";
    }
    this->setMinimumSize(minX, minY);
    this->resize(x, y);
}



void PlotWindow::on_updateRateSpinBox_valueChanged(int arg1){
    timer = 0;
}

void PlotWindow::on_lineThicknessSpinBox_valueChanged(int arg1){
    MFRgraph->setThickness(arg1);
}

void PlotWindow::on_rescaleButton_clicked(){
    MFRmax = 0;
}

void PlotWindow::on_exitButton_clicked(){
    emit plotWindowDeleteRequest(my_index);
}

void PlotWindow::on_verticalCheckBox_clicked(bool checked){
    if(rasterPlot == NULL)
        return;
    rasterPlot->setOrientation( checked ? BasicGraph::VERTICAL :
                                          BasicGraph::HORIZONTAL);
}



























