#include "graphs.h"

Graphs::Graphs(int historyNo, int currentNo, int neuronsNo,
                                                bool _mode6, int maxSpikes = 15,
                                         int position_x = 0,int position_y = 0):
                                                             neurons(neuronsNo),
                                     hGraphsNo(historyNo), cGraphsNo(currentNo),
                                    mode6(_mode6), x(position_x), y(position_y){
    //for(startX=0; startX<hGraphsNo; startX++)
    //    hGraph[startX] = new HistoryGraph("Number Of Spikes (simulation "
    //                                            + QString::number(startX+1) + "):",
    //                                15, x+((startX%2)*520), y+((startX/2)*170),500,150);
    corticalGraphsNo = 0;

    hGraph[0] = new CurrentGraph("Number Of Spikes", maxSpikes, x, y,500,150);
    hGraph[1] = new CurrentGraph("Robot Velocities", 10, x+520, y,500,150);

    startX=hGraphsNo;
    if(cGraphsNo != 6){
        for(j=0; j<cGraphsNo; j++){
            cGraph[j] = new CurrentGraph(
                                   "Membrane Potential (layer " +
                                                    QString::number(j+1) + "):",
                                     150, x+((startX%2)*520), y+((startX/2)*160),500,150);
            startX++;
        }
    }
    else{
        cGraph[0] = new CurrentGraph( "Membrane Potential (layer 1: Excitatory)",
                                     150, x+((startX%2)*520), y+((startX/2)*160),500,150);
        startX++;
        cGraph[1] = new CurrentGraph( "Membrane Potential (layer 2: Excitatory)",
                                     150, x+((startX%2)*520), y+((startX/2)*160),500,150);
        startX++;
        cGraph[2] = new CurrentGraph( "Membrane Potential (layer 3: Excitatory)",
                                     250, x+((startX%2)*520), y+((startX/2)*160),500,150);
        startX++;
        cGraph[3] = new CurrentGraph( "Membrane Potential (layer 4: Excitatory)",
                                     250, x+((startX%2)*520), y+((startX/2)*160),500,150);
        startX++;
        cGraph[4] = new CurrentGraph( "Membrane Potential (layer 5: Inhibitory)",
                                     300, x+((startX%2)*520), y+((startX/2)*160),500,150);
        startX++;
        cGraph[5] = new CurrentGraph( "Membrane Potential (layer 6: Inhibitory)",
                                     300, x+((startX%2)*520), y+((startX/2)*160),500,150);
        startX++;

    }
    if(neurons > 1020 && !mode6){
       if(neurons < 1535)
           sGraph[0] = new RasterPlot("Spikes",neurons,x,y+(((startX+1)/2)*160),
                                                               neurons+100,200);
        else if(neurons < 2556){
           sGraph[0] = new RasterPlot("Workspace Neurons",1535,
                            x,       y+(((startX+1)/2)*160), 1535 + 50, 200);
           sGraph[1] = new RasterPlot("Cortical Neurons",neurons-1535,
                            x, 220 + y+(((startX+1)/2)*160), neurons-1535 + 50, 200);
        }
        else if(neurons < 3580){
           sGraph[0] = new RasterPlot("Workspace Neurons",1535,
                            x,       y+(((startX+1)/2)*160), 1535 + 50, 200);
           sGraph[1] = new RasterPlot("Cortical Column 1 Neurons",1024,
                            x, 220 + y+(((startX+1)/2)*160), 1024 + 50, 200);
           sGraph[2] = new RasterPlot("Cortical Column 2 Neurons",neurons-2556,
                            x, 440 + y+(((startX+1)/2)*160), neurons-2556 + 50, 200);
        }
        else if(neurons < 4604){
           sGraph[0] = new RasterPlot("Workspace Neurons",1535,
                                          x, y+(((startX+1)/2)*160), 1535 + 100,200);
           sGraph[1] = new RasterPlot("Cortical Column 1 Neurons",1024,
                            x, 220 + y+(((startX+1)/2)*160), 1024 + 50, 200);
           sGraph[2] = new RasterPlot("Cortical Column 2 Neurons",1024,
                            x, 440 + y+(((startX+1)/2)*160), 1024 + 50, 200);
           sGraph[3] = new RasterPlot("Cortical Column 3 Neurons",neurons-3580,
                            x, 660 + y+(((startX+1)/2)*160), neurons-3580 + 50, 200);
        }
        else if(neurons < 6000 && !mode6){
           sGraph[0] = new RasterPlot("Workspace Neurons",1535,
                                          x, y+(((startX+1)/2)*160), 1535 + 100,200);
           sGraph[1] = new RasterPlot("Cortical Column 1 Neurons",1024,
                            x, 200 + y+(((startX+1)/2)*160), 1024 + 50, 180);
           sGraph[2] = new RasterPlot("Cortical Column 2 Neurons",1024,
                            x, 380 + y+(((startX+1)/2)*160), 1024 + 50, 180);
           sGraph[3] = new RasterPlot("Cortical Column 3 Neurons",1024,
                            x, 560 + y+(((startX+1)/2)*160), 1024 + 50, 180);
           sGraph[4] = new RasterPlot("",neurons-4604,
                            x, 740 + y+(((startX+1)/2)*160), neurons-4604 + 50, 150);
        }
    }
    else if(!mode6)
       sGraph[0] = new RasterPlot("Spikes", neurons, x, y+(((startX+1)/2)*160),
                                                                     1020, 160);
    else{
       sGraph[0] = new RasterPlot("Workspace Neurons ("+
                                           QString::number(neurons)+")",neurons,
                                     x-100, y+(((startX+1)/2)*160),neurons+50, 200);
        startX++;
    }

}

Graphs::~Graphs(){
    for(i=0; i<hGraphsNo; i++)
        delete hGraph[i];
    for(i=0; i<cGraphsNo; i++)
        delete cGraph[i];
    delete sGraph[0];
    if(neurons >= 1535 && !mode6)
        delete sGraph[1];
    if(neurons >= 2556 && !mode6)
        delete sGraph[2];
    if(neurons >= 3580 && !mode6)
        delete sGraph[3];
    if(neurons >= 4604 && !mode6)
        delete sGraph[4];
}


void Graphs::draw(QPainter *painter){
    for(i=0; i<hGraphsNo; i++)
        hGraph[i]->draw(painter);
    for(i=0; i<cGraphsNo; i++)
        cGraph[i]->draw(painter);
    sGraph[0]->draw(painter);
    if(!mode6){
        if(neurons >= 1535)
            sGraph[1]->draw(painter);
        if(neurons >= 2556)
            sGraph[2]->draw(painter);
        if(neurons >= 3580)
            sGraph[3]->draw(painter);
        if(neurons >= 4604)
            sGraph[4]->draw(painter);
    }
    else{
        for(i=0; i<corticalGraphsNo; i++)
            corticalGraph[i]->draw(painter);
    }
}

void Graphs::add_new_value(int index, int data_num,float data_value){
    hGraph[index]->add_new_value(data_num, data_value);
}

void Graphs::add_new_value_current(int index, int data_num,float data_value){
    cGraph[index]->add_new_value(data_num, data_value);
}

void Graphs::add_spike(int neuronId){
    if(!mode6){
        if(neuronId < 1535)
            sGraph[0]->add_spike(neuronId);
        else if(neuronId < 2556)
            sGraph[1]->add_spike(neuronId-1535);
        else if(neuronId < 3580)
            sGraph[2]->add_spike(neuronId-2556);
        else if(neuronId < 4604)
            sGraph[3]->add_spike(neuronId-3580);
        else
            sGraph[4]->add_spike(neuronId-4604);
    }
    else{ // mode6
        if(neuronId - neurons < 0)
            sGraph[0]->add_spike(neuronId);
        else if(neuronId - neurons < corticalNeurons[0])
            corticalGraph[0]->add_spike(neuronId-neurons);
        else if(neuronId - neurons < corticalNeurons[0] + corticalNeurons[1])
            corticalGraph[1]->add_spike(neuronId-neurons - corticalNeurons[0]);
        else if(neuronId - neurons < corticalNeurons[0] + corticalNeurons[1] +
                                                          corticalNeurons[2])
            corticalGraph[2]->add_spike(neuronId-neurons - corticalNeurons[0] -
                                                           corticalNeurons[1]);
        else if(neuronId - neurons < corticalNeurons[0] + corticalNeurons[1] +
                                                          corticalNeurons[2] +
                                                          corticalNeurons[3])
            corticalGraph[3]->add_spike(neuronId-neurons - corticalNeurons[0] -
                                                           corticalNeurons[1] -
                                                           corticalNeurons[2]);
        /*else if(neuronId - neurons < corticalNeurons[0] + corticalNeurons[1] +
                                                          corticalNeurons[3] +
                                                          corticalNeurons[4])
            corticalGraph[4]->add_spike(neuronId-neurons - corticalNeurons[0] -
                                                           corticalNeurons[1] -
                                                           corticalNeurons[2] +
                                                           corticalNeurons[3]);
        else if(neuronId - neurons < corticalNeurons[0] + corticalNeurons[1] +
                                                          corticalNeurons[3] +
                                                          corticalNeurons[4] +
                                                          corticalNeurons[5])
            corticalGraph[5]->add_spike(neuronId-neurons - corticalNeurons[0] -
                                                           corticalNeurons[1] -
                                                           corticalNeurons[2] +
                                                           corticalNeurons[3] +
                                                           corticalNeurons[4]);
        else if(neuronId - neurons < corticalNeurons[0] + corticalNeurons[1] +
                                                          corticalNeurons[3] +
                                                          corticalNeurons[4] +
                                                          corticalNeurons[5] +
                                                          corticalNeurons[6])
            corticalGraph[6]->add_spike(neuronId-neurons - corticalNeurons[0] -
                                                           corticalNeurons[1] -
                                                           corticalNeurons[2] +
                                                           corticalNeurons[3] +
                                                           corticalNeurons[4] +
                                                           corticalNeurons[5]);
        else if(neuronId - neurons < corticalNeurons[0] + corticalNeurons[1] +
                                                          corticalNeurons[3] +
                                                          corticalNeurons[4] +
                                                          corticalNeurons[5] +
                                                          corticalNeurons[6] +
                                                          corticalNeurons[7])
            corticalGraph[7]->add_spike(neuronId-neurons - corticalNeurons[0] -
                                                           corticalNeurons[1] -
                                                           corticalNeurons[2] +
                                                           corticalNeurons[3] +
                                                           corticalNeurons[4] +
                                                           corticalNeurons[5] +
                                                           corticalNeurons[6]);*/
    }
}

void Graphs::next_step(){
    sGraph[0]->next_step();
    if(mode6)
        for(i=0; i<corticalGraphsNo; i++)
            corticalGraph[i]->next_step();
    else{
        if(neurons >= 1535)
            sGraph[1]->next_step();
        if(neurons >= 2556)
            sGraph[2]->next_step();
        if(neurons >= 3580)
            sGraph[3]->next_step();
        if(neurons >= 4604)
            sGraph[4]->next_step();
    }
}

void Graphs::add_cortical_graph(int neuronsNo){
    corticalGraph[corticalGraphsNo] = new RasterPlot(" Cortical Column " +
                         QString::number(corticalGraphsNo),
                         neuronsNo, x,
                         y+(((startX+1)/2)*180) + 20 + corticalGraphsNo*180,
                         neuronsNo+50,160);

    corticalNeurons[corticalGraphsNo] = neuronsNo;
    startX++;
    corticalGraphsNo++;
}

void Graphs::add_input_graph(int neuronsNo){
    corticalGraph[corticalGraphsNo] = new RasterPlot("Sensory Modules",
                         neuronsNo, x,
                         y+(((startX+1)/2)*180) + 20 + corticalGraphsNo*180,
                         neuronsNo+50,160);

    corticalNeurons[corticalGraphsNo] = neuronsNo;
    startX++;
    corticalGraphsNo++;
}

void Graphs::add_output_graph(int neuronsNo, int inputNeurons){
    corticalGraph[corticalGraphsNo] = new RasterPlot("Motor Modules",
                         neuronsNo, x + inputNeurons + 60,
                         y+(((startX+1)/2)*180) + 20 + (corticalGraphsNo-1)*180,
                         neuronsNo+50,160);

    corticalNeurons[corticalGraphsNo] = neuronsNo;
    startX++;
    corticalGraphsNo++;
}

void Graphs::add_module_graph(int neuronsNo, QString title){
    corticalGraph[corticalGraphsNo] = new RasterPlot(title,
                         neuronsNo, x,
                         y+(((startX+1)/2)*180) + 20 + (corticalGraphsNo-2)*180,
                         neuronsNo+50,160);

    corticalNeurons[corticalGraphsNo] = neuronsNo;
    startX++;
    corticalGraphsNo++;
}




/*1535
2556
3580
4604
6000*/








