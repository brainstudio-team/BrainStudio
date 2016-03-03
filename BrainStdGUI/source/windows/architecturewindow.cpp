#include "architecturewindow.h"
#include "ui_architecturewindow.h"

ArchitectureWindow::ArchitectureWindow(QWidget *parent): QWidget(parent),
                                       ui(new Ui::ArchitectureWindow){
    this->setAutoFillBackground(true);
    this->setBackgroundRole(QPalette::Light);

    //this->setCursor(QCursor(Qt::CrossCursor));
    stimWidget = NULL;
    A = NULL;
    mi = NULL;

    selection_box.setRect(-1,-1,0,0);

    x1 = y1 = x2 = y2 = xA = yA = xB = yB = 0;
    menuPos.setX(0);    menuPos.setY(0);
    resize_bottom = resize_right = resize_bottomright = false;

    SIMULATION = false;
    grid = true;
    mode = Block::modeEdit;
    details = true;
    show_connections = true;
    _showActions = true;
    under_modification = false;
    connecting = false;
    highlighted = "";
    con_source = con_target = -1;
    zoom = 100;
    connectingCursor = QPoint(0,0);

    WINDOW_X = WINDOW_Y = GRID_WIDTH = 10;
    GRID_offset_X = GRID_offset_Y = 0;


    // Create actions
    pasteAct = new QAction(QIcon(":/new/prefix1/icons/paste.png"), tr("&Paste"),
                           this);
    pasteAct->setShortcuts(QKeySequence::InsertLineSeparator);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    //connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));
    newBlockAct = new QAction(QIcon(":/new/prefix1/icons/add_32.png"),
                              tr("&New block"),this);
    //newBlockAct->setShortcuts(QKeySequence::Paste);
    newBlockAct->setStatusTip(tr("Create a new block of neurons."));
    connect(newBlockAct, SIGNAL(triggered()),
            this,        SLOT(schemaDoubleClickSlot()));

    newTextBlockAct = new QAction(QIcon(":/new/prefix1/icons/add_32.png"),
                              tr("&New text block"),this);
    newTextBlockAct->setStatusTip(tr("Add new text."));
    connect(newTextBlockAct, SIGNAL(triggered()),
            this,        SLOT(schemaCreateTextBlockSlot()));

    ui->setupUi(this);
}

ArchitectureWindow::~ArchitectureWindow(){
    this->clear();
    delete ui;
}

void ArchitectureWindow::resizeEvent(QResizeEvent * event ){
    if(event->size().width() < event->size().height())
        GRID_SIZE = event->size().height()/GRID_WIDTH;
    else
        GRID_SIZE = event->size().width()/GRID_WIDTH;

}

void ArchitectureWindow::setFiringRateDepth(const int &value){
    for(BlockIter bl = blocks.begin(); bl != blocks.end(); bl++){
        bl.value()->setFiringRateDepth(value);
    }
}

QString ArchitectureWindow::in(int x2, int y2){
    QMap<QString, Block*>::iterator b; // SOS: It needs to be local
    for(b=blocks.begin(); b != blocks.end(); b++){
        if(b.value()->in(x2+WINDOW_X-this->x(), y2+WINDOW_Y-this->y())){
            return b.key();
        }
    }
    return "";
}

/* Updates the firing rates of the figure
 *
 */
void ArchitectureWindow::update_f_rates(unsigned int *spikesArray, int spikes){
    k=0;
    int passedNeurons=0;
    for(BlockIter bl=blocks.begin(); bl !=blocks.end(); bl++){ // For each block
        if(bl.value()->getNeuronsSize() != bl.value()->firingRateNeuron.size()){
            qDebug() << "ArchitectureWidget::update_f_rates: Error: neurons and"
                     << "firingRateNeuron sizes don't match in block" <<bl.key()
                     << "neurons:" << bl.value()->getNeuronsSize()
                     << "firingRates:" << bl.value()->firingRateNeuron.size()
                     << "(returning...)"; return;
        }
        frSUM=0.0f;
        for(int j=0;j<bl.value()->getNeuronsSize(); j++){ // for each neuron
            if( k < spikes && passedNeurons==spikesArray[k]){
                bl.value()->firingRateNeuron[j].nextFR(1);
                k++;
            }
            else{
                bl.value()->firingRateNeuron[j].nextFR(0);
            }
            passedNeurons++;
            frSUM+=bl.value()->getFiringRateNeuron(j);
        }
        bl.value()->firingRateBlock.justSet(frSUM/(float)bl.value()->getNeuronsSize());
    }
}

void ArchitectureWindow::on_horizontalScrollBar_valueChanged(int value){
    WINDOW_X = -value*12;

    for(BlockIter bl = blocks.begin(); bl != blocks.end(); bl++){
        bl.value()->setWINDOW_X(WINDOW_X);
        bl.value()->setVisible(true);
        bl.value()->setEnabled(true);
    }
    for(int i=0;i<textBlocks.size();i++){
        textBlocks[i].setWINDOW_X(WINDOW_X);
    }
    GRID_offset_X = WINDOW_X % GRID_WIDTH;
    this->update();
}

void ArchitectureWindow::on_verticalScrollBar_valueChanged(int value){
    WINDOW_Y = -value*12;
    for(BlockIter bl = blocks.begin(); bl != blocks.end(); bl++){
        bl.value()->setWINDOW_Y(WINDOW_Y);
        bl.value()->setVisible(true);
        bl.value()->setEnabled(true);
    }
    for(int i=0;i<textBlocks.size();i++){
        textBlocks[i].setWINDOW_Y(WINDOW_Y);
    }
    GRID_offset_Y = WINDOW_Y % GRID_WIDTH;
    this->update();
}

void ArchitectureWindow::wheelEvent(QWheelEvent *event){
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    if(event->orientation() == Qt::Horizontal){
        ui->horizontalScrollBar->setValue(ui->horizontalScrollBar->value()
                                          - numSteps);
    }
    else{
        ui->verticalScrollBar->setValue(ui->verticalScrollBar->value()
                                        - numSteps);
    }
    event->accept();
}


void ArchitectureWindow::paintEvent(QPaintEvent * event){
    (void)event;
    QPainter painter(this);

    // GRID
    if(grid){
        painter.setPen(QColor("#d4d4d4"));
        for(int i=0; i<GRID_SIZE; i++){
            painter.drawLine(0,             i*GRID_WIDTH + GRID_offset_Y,
                             this->width(), i*GRID_WIDTH + GRID_offset_Y);
            painter.drawLine(i*GRID_WIDTH + GRID_offset_X, 0,
                             i*GRID_WIDTH + GRID_offset_X, this->height() );
        }
    }

    // SELECTION BOX
    /*painter.setPen(QPen(QColor("#d4d4d4"), 3, Qt::DashLine));
    painter.drawRect(selection_box);*/

    // SCHEMA

    // Connections -------------------------------------------------------------
    if(mode == Block::modeNetwork){
    }
    else{
        drawConnections(painter);
    }
    // -------------------------------------------------------------------------

    // New Connections ---------------------------------------------------------
    if(connecting){
        if(blocks.contains(con_source)){
            painter.setBrush(QBrush(QColor(0,0,0,0)));
            painter.setPen(QPen(QColor(100,100,100,180),
                           LINE_THICKNESS, Qt::SolidLine));
            if(blocks.contains(con_target))
                drawConnection(painter, con_source, con_target);
            else
                drawConnection(painter, con_source, menuPos);
        }
    }

    // Details -----------------------------------------------------------------
    if(details){
        painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
        for(BlockIter bl = blocks.begin(); bl != blocks.end(); bl++){ // for each block
            if(mode == Block::modeStatesPixels || mode == Block::modeStatesPlots){
                painter.drawText(bl.value()->x()+5, bl.value()->y()-20,bl.key());
                painter.drawText(bl.value()->x()+5, bl.value()->y()-5,
                                QString::number(bl.value()->getNeuronsSize()));
            }
            /*else if(mode == Block::modeEdit){
                painter.drawText(bl.value()->x()-15, bl.value()->y()+15, bl.key());
            }*/
            else if(mode == Block::modeC || mode == Block::modeRasters){
                painter.drawText(bl.value()->x()+5, bl.value()->y()-20,
                                 bl.key() +" (" +
                                 bl.value()->getType() + ")");

                freq = bl.value()->frequencyBlock();
                /*oscMode;
                if(freq >= 0.1 && freq < 4.0)
                    oscMode = "(Delta)";                   // (0.1 to 4)
                else if(freq >= 4.0 && freq < 8.0)
                    oscMode = "(Theta)";                   // (4 to 7)
                else if(freq >= 8.0 && freq < 13.0)
                    oscMode = "(Alpha)";                   // (8 to 12)
                else if(freq >= 12.0 && freq < 30.0)
                    oscMode = "(Beta)";                    // (12 to 30)
                else if(freq >= 25.0 && freq < 100.0)
                    oscMode = "(Gamma)";                   //(25 to 100)
                else oscMode = "";*/

                painter.drawText(bl.value()->x()+5, bl.value()->y()-5,
                                 "FR: " + QString::number(
                                 bl.value()->frequencyBlock(), 'f', 1) +
                                 " spikes/sec"/* + oscMode*/);
            }
        }
    }
    // -------------------------------------------------------------------------

    // TRANSFORMATIONS ---------------------------------------------------------
    /*if(zoom != 100)
        painter.scale((qreal)zoom/(qreal)100, (qreal)zoom/(qreal)100);
    if(WINDOW_X || WINDOW_Y)
        painter.translate(WINDOW_X, WINDOW_Y);*/
    // -------------------------------------------------------------------------

    // Text Blocks -------------------------------------------------------------
    for(int i=0;i<textBlocks.size();i++){
        painter.setPen(QPen(QColor(textBlocks[i].colour())));
        painter.setFont(QFont("Arial", textBlocks[i].fontSize()));
        painter.drawText(textBlocks[i].x(), textBlocks[i].y(),
                         textBlocks[i].width(), textBlocks[i].height(),
                         Qt::AlignLeft, textBlocks[i].text());
    }
    // -------------------------------------------------------------------------
}


/* Sets
 *
 */
bool ArchitectureWindow::setHighlighted(QString id){
    qDebug() << "Architecture::setHighlighted: " << id;

    // If not the currently highlighted, change it
    if(highlighted != id){
        // Un-check the previous
        if(blocks.contains(highlighted))
            blocks[highlighted]->setHighlighted(false);

        // Check the current
        if(blocks.contains(id)){
            blocks[id]->setHighlighted(true);
        }
        else{
            id = "";
        }

        highlighted = id;
    }

    this->update();
    return true;
}

void ArchitectureWindow::turnOffStimulation(){
    // Delete the stimulus control widget
    if(stimWidget != NULL){
        delete stimWidget;
        stimWidget = NULL;
    }
}

void ArchitectureWindow::turnOnStimulation(){
    this->turnOffStimulation();

    // Turn on stimulation:
    if(blocks.contains(highlighted)){
        stimWidget = new StimulationWidget(blocks[highlighted], this);
        stimWidget->show();
        stimWidget->move(blocks[highlighted]->x() +
                         blocks[highlighted]->width()+10,
                         blocks[highlighted]->y());


        connect(stimWidget, SIGNAL(setStimulus(QString, int, float)),
                A, SLOT(setStimulus(QString, int, float)));
        connect(stimWidget, SIGNAL(setStimulus(QString, int, int, float)),
                A, SLOT(setStimulus(QString, int, int, float)));
        connect(stimWidget, SIGNAL(setStimulus(QString,int,int,double,double,double,double)),
                A, SLOT(setStimulus(QString,int,int,double,double,double,double)));
        connect(stimWidget, SIGNAL(clearAllStimuli()), A, SLOT(clearAllStimuli()));
        connect(stimWidget, SIGNAL(clearStimulus(QString)),
                A, SLOT(clearStimulus(QString)));
    }
}

void ArchitectureWindow::schemaModifiedSlot(){
    this->setModified();
    this->update();
}

void ArchitectureWindow::startConnectionSlot(QString id){
    if(!blocks.contains(id)){
        qDebug() << "Architecture::startConnectionSlot: Index out of range!";
        return;
    }

    con_source = id;
    con_target = id;

    this->setCursor(QCursor(Qt::SizeVerCursor));
    connecting = true;
    this->setMouseTracking(true);
    this->update();
}

void ArchitectureWindow::deleteAllBlocks(){
    QMap<QString,Block*>::iterator it = blocks.begin();
    while(it != blocks.end()){
        QString name = it.key();
        it++;
        this->deleteBlockSlot(name);
    }
    // Both blocks and connections now should be empty!
    emit modifiedSignal();
}

void ArchitectureWindow::deleteRedundantConnections(){
    for(int i=0; i<connections.size(); i++){
        if(!blocks.contains(connections[i].source()) ||
                !blocks.contains(connections[i].target()))
            connections.removeAt(i);
    }

}

void ArchitectureWindow::deleteBlockSlot(const QString &id){
    qDebug() << "Architecture::deleteBlockSlot: Yo!!";
    if(!blocks.contains(id)){
        qDebug() << "Architecture::deleteBlockSlot: Index out of range!";
        return;
    }

    this->deleteRedundantConnections();

    // Remove block
    Block *temp;
    temp = blocks[id];
    disconnect(temp, SIGNAL(newConnectionSignal(QString)),
               this, SLOT(startConnectionSlot(QString)));
    disconnect(temp, SIGNAL(deleteBlockSignal(QString)),
               this, SLOT(deleteBlockSlot(QString)));
    disconnect(temp, SIGNAL(schemaModified()),
               this, SLOT(schemaModifiedSlot()));
    disconnect(temp, SIGNAL(highlighted_signal(QString)),
               this, SLOT(setHighlighted(QString)));
    disconnect(temp, SIGNAL(init_block_motion_signal(int,int)),
               this, SLOT(init_block_motion_slot(int,int)));
    disconnect(temp, SIGNAL(init_block_resize_signal(QPoint,QString)),
               this, SLOT(init_block_resize_slot(QPoint,QString)));
    disconnect(temp, SIGNAL(doubleClickSignal(QString)),
               this, SLOT(blockDoubleClickSlot(QString)));
    disconnect(temp, SIGNAL(newPlotSignal(QString)),
               this, SLOT(blockNewPlotSlot(QString)));
    disconnect(temp, SIGNAL(idChangedSignal(QString,QString)),
               this, SLOT(blockIdChangedSlot(QString,QString)));
    disconnect(temp, SIGNAL(stimulationSignal()),
               this, SLOT(turnOnStimulation()));

    blocks.remove(id);
    temp->setVisible(false);
    // this is working though it's not perfect. The reason it's working is that
    // during the distruction of 'this' the object in temp will be destroyed as
    // well since it's a child. However, a more correct approach would be to use
    //                      delete temp;
    // ZAF SOS: However this doesn't work and throws an error!!

    update();
    emit modifiedSignal();
}

void ArchitectureWindow::endConnection(Connection edge){
    if(!blocks.contains(edge.source()) || !blocks.contains(edge.target())){
        qDebug() << "Architecture::endConnection: Index out of range!";
        return;
    }

    this->addConnection(edge);

    this->setCursor(QCursor(Qt::ArrowCursor));
    connecting = false;
    for(BlockIter bl = blocks.begin(); bl != blocks.end(); bl++){
        bl.value()->setConnecting(false);
    }
    this->update();
}



void ArchitectureWindow::setBlockMode(int value){
    qDebug() << "Changing mode: "<< value;
    for(BlockIter bl = blocks.begin(); bl != blocks.end(); bl++)
        bl.value()->setMode(value);
}

void ArchitectureWindow::turnSIMULATION(bool on){
    SIMULATION = on;
    if(SIMULATION){
        this->setStyleSheet("QWidget { background-color: white; }");
    }
    else{
        this->setStyleSheet("QWidget { background-color: grey; }");
    }
    /*
    QPalette p(palette());
    // Set background colour to black
    p.setColor(QPalette::Background, Qt::black);
    setPalette(p);
    */
}

bool ArchitectureWindow::addLFPcounter(const QString &index){
    if(blocks.contains(index) && !LFPindex.contains(index)){
        LFPindex.push_back(index);
        return true;
    }
    return false;
}


void ArchitectureWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        qDebug() << "Architecture:" << "Left click";
        setHighlighted("");
        selection_box.setRect(event->x(), event->y(), 0, 0);
    }
    else if(event->button() == Qt::RightButton){
        qDebug() << "Architecture:" << "Right click";
    }
    this->update();
}

void ArchitectureWindow::mouseDoubleClickEvent(QMouseEvent *event){
    qDebug() << "Architecture:" << "Double click";
    if(event->button() == Qt::LeftButton){
        emit schemaDoubleClick(event->pos());
        this->update();
    }
}

bool encloses(const QRect &small, const QRect &big){
    if(big.width() < 0 && big.height() > 0){
        if(small.x() > big.x()+big.width() && small.y() > big.y() &&
           small.width() < big.x() && small.height() < big.height()){
            return true;
        }
    }
    else if(big.width() > 0 && big.height() < 0){
        if(small.x() > big.x() && small.y() > big.y()+big.height() &&
           small.width() < big.width() && small.height() < big.y()){
            return true;
        }
    }
    else if(big.width() < 0 && big.height() < 0){
        if(small.x() > big.x()+big.width() &&
           small.y() > big.y()+big.height() &&
           small.width() < big.x() && small.height() < big.y()){
            return true;
        }
    }
    else{
        if(small.x() > big.x() && small.y() > big.y() &&
           small.width() < big.width() && small.height() < big.height()){
            return true;
        }
    }
    return false;
}

void ArchitectureWindow::mouseMoveEvent(QMouseEvent *event){
    qDebug() << "Architecture::mouseMoveEvent: event->pos =" << event->pos();
    menuPos = event->pos();

    if(selection_box.x() != -1 || selection_box.y() != -1){
        selection_box.setWidth(event->x()-selection_box.x());
        selection_box.setHeight(event->y()-selection_box.y());

        QMap<QString,Block*>::iterator it;
        for(it = blocks.begin(); it != blocks.end(); it++){
            if(encloses(it.value()->geometry(), selection_box)){
                qDebug() << "ZAF" << it.key();
                this->setHighlighted(it.key());
            }
        }
    }
    // Move block
    else if(blocks.contains(highlighted) && !connecting
            && !resize_bottom && !resize_right && !resize_bottomright ){
        this->turnOffStimulation();
        blocks[highlighted]->moveXY(event->pos().x() - xStart,
                                    event->pos().y() - yStart);
        under_modification = true;
        xStart = event->pos().x();
        yStart = event->pos().y();
    }
    // Check for target of connection
    else if(connecting){
        this->turnOffStimulation();
        QString result = in(event->pos().x(), event->pos().y());
        if(blocks.contains(result)){
            con_target = result;
            for(BlockIter bl = blocks.begin(); bl != blocks.end(); bl++){
                if(bl.key() == result)
                    bl.value()->setConnecting(true);
                else if(bl.key() != con_source)
                    bl.value()->setConnecting(false);
            }
        }
        else{
            con_target = -1;
        }
    }
    else if(resize_right && blocks.contains(highlighted)){
        this->turnOffStimulation();
        int X = event->pos().x() - blocks[highlighted]->x();
        if( X >= 150 ){
            blocks[highlighted]->resize(X, blocks[highlighted]->height());
            under_modification = true;
        }
    }
    else if(resize_bottom && blocks.contains(highlighted)){
        this->turnOffStimulation();
        int Y = event->pos().y() - blocks[highlighted]->y();
        if( Y >= 42 ){
            blocks[highlighted]->resize(blocks[highlighted]->width(), Y);
            under_modification = true;
        }
    }
    else if(resize_bottomright && blocks.contains(highlighted)){
        this->turnOffStimulation();
        int X = event->pos().x() - blocks[highlighted]->x();
        int Y = event->pos().y() - blocks[highlighted]->y();
        if( X >= 150 && Y >= 42 ){
            blocks[highlighted]->resize(X, Y);
            under_modification = true;
        }
        else if( X >= 150 && Y < 42 ){
            blocks[highlighted]->resize(X, blocks[highlighted]->height());
            under_modification = true;
        }
        else if( X < 150 && Y >= 42 ){
            blocks[highlighted]->resize(blocks[highlighted]->width(), Y);
            under_modification = true;
        }
    }

    this->update();
}


void ArchitectureWindow::mouseReleaseEvent(QMouseEvent *event){
    qDebug() << "Architecture::mouseReleaseEvent:" << event->pos();

    if(selection_box.width() != 0 || selection_box.height() != 0){
        selection_box.setRect(-1,-1,0,0);
    }

    // If we're currently modifying something with the mouse:
    if(under_modification){
        this->setModified();
    }

    if(connecting && blocks.contains(con_source)
                  && blocks.contains(con_target)){
        qDebug() << "Architecture::mouseReleaseEvent: Adding new connection";
        emit schemaAddingNewConnection(con_source, con_target);
        con_source = -1;
        con_target = -1;
    }
    else if(connecting){
        qDebug() << "Architecture::mouseReleaseEvent: Connection cancelled";
        con_source = -1;
        con_target = -1;
        this->setCursor(QCursor(Qt::ArrowCursor));
        connecting = false;
        for(BlockIter bl = blocks.begin(); bl != blocks.end(); bl++){
            bl.value()->setConnecting(false);
        }
        this->update();
    }
}

void ArchitectureWindow::init_block_motion_slot(int x, int y){
    qDebug() << "Architecture::initialize block motion" << x << y;

    if(blocks.contains(highlighted)){
        resize_bottom = resize_right = resize_bottomright = false;
        xStart = x + blocks[highlighted]->x();
        yStart = y + blocks[highlighted]->y();
    }
}

void ArchitectureWindow::init_block_resize_slot(QPoint point,QString direction){
    qDebug() << "Architecture::initialize block resize" << direction << point;

    if(blocks.contains(highlighted)){
        if(direction == "right"){
            resize_right = true;
            resize_bottom = false;
            resize_bottomright = false;
        }
        else if(direction == "bottom"){
            resize_right = false;
            resize_bottom = true;
            resize_bottomright = false;
        }
        else if(direction == "bottomright"){
            resize_right = false;
            resize_bottom = false;
            resize_bottomright = true;
        }
    }
}

void ArchitectureWindow::blockIdChangedSlot(QString old_id, QString new_id){
    qDebug() << "ArchitectureWindow::blockIdChangedSlot("
             << old_id << ", " << new_id << ")";
    if(old_id == new_id){
        return;
    }
    if(!blocks.contains(old_id)){
        qDebug() << "ArchitectureWindow::blockIdChangedSlot: Error: id"
                 << old_id << "not found!";
        return;
    }

    // If id already exists, tell this to the block
    if(blocks.contains(new_id)){
        emit showMessageSignal("This name already exists in the architecture! "
                               "Please choose another one..", 2000);
        blocks[old_id]->id_exists(old_id);
        return;
    }
    else{
        // Move the block
        blocks[new_id] = blocks[old_id];
        blocks[old_id] = NULL;
        blocks.remove(old_id);
        for(int i=0; i<actions.size(); i++){
            if(actions[i].node == old_id)
                actions[i].node = new_id;
        }
        this->schemaModifiedSlot();
    }
}

void ArchitectureWindow::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu(this);
    menu.addAction(newBlockAct);
    menu.addAction(newTextBlockAct);
    menu.addSeparator();
    menu.addAction(pasteAct);
    menu.exec(event->globalPos());
}

void ArchitectureWindow::addBlock(Block *block){
    qDebug() << "Adding block " << blocks.size() << ". (Model: "
             << block->getType() << ")";
    connect(block, SIGNAL(schemaModified()),
            this,  SLOT(schemaModifiedSlot()));
    connect(block, SIGNAL(newConnectionSignal(QString)),
            this,  SLOT(startConnectionSlot(QString)));
    connect(block, SIGNAL(deleteBlockSignal(QString)),
            this,  SLOT(deleteBlockSlot(QString)));
    connect(block, SIGNAL(highlighted_signal(QString)),
            this,  SLOT(setHighlighted(QString)));
    connect(block, SIGNAL(init_block_motion_signal(int,int)),
            this,  SLOT(init_block_motion_slot(int,int)));
    connect(block, SIGNAL(init_block_resize_signal(QPoint,QString)),
            this,  SLOT(init_block_resize_slot(QPoint,QString)));
    connect(block, SIGNAL(doubleClickSignal(QString)),
            this,  SLOT(blockDoubleClickSlot(QString)));
    connect(block, SIGNAL(newPlotSignal(QString)),
            this,  SLOT(blockNewPlotSlot(QString)));
    connect(block, SIGNAL(idChangedSignal(QString,QString)),
            this,  SLOT(blockIdChangedSlot(QString,QString)));
    connect(block, SIGNAL(stimulationSignal()),
            this,  SLOT(turnOnStimulation()));

    blocks[block->getId()] = block;
    LFPvalue[block->getId()] = 0.0;

    // Reset first/last index - SOS: I need to do that because maps change the
    // order that I am appending new blocks!
    int current_index = 0;
    for(BlockIter it=blocks.begin(); it != blocks.end(); it++){
        it.value()->setFirstNeuronIdx(current_index);
        current_index += it.value()->getNeuronsSize();
        it.value()->setLastNeuronIdx(current_index);
    }
}

// Name, type, X, Y, width, height, colour
bool ArchitectureWindow::addBlock(QString _name, QString _type,
                                  int _x, int _y, int _w, int _h, QString _col,
                                  int _neuronsNo){
    bool spiking = false;
    if(!BackendData::node_model.contains(_type)){
        qDebug() << "ArchitectureWindow::addNodes: "
                    "Error: node model not recognized!";
        return false;
    }
    if(BackendData::node_model[_type] == "spike")
        spiking = true;
    else if(BackendData::node_model[_type] == "rate")
        spiking = false;
    else{
        qDebug() << "ArchitectureWindow::addNodes: "
                    "Error: node model not recognized!";
        return false;
    }

    Block *block = new Block(_name, _type, spiking, _x, _y, _w, _h,
                             _col, _neuronsNo, ui->frame);
    addBlock(block);
    return true;
}


void ArchitectureWindow::restartActions(){
    while(actionsDone.size() > 0){
        actions.push_front(actionsDone.last());
        actionsDone.pop_back();
    }
}



void ArchitectureWindow::setNetworkMode(const int &mode){
    controls.network = mode;
    if(mode == Controls::NET_NONE){
        if(mi != NULL){
            delete mi;
            mi = NULL;
        }
    }
    else if(mode == Controls::NET_MI && mi == NULL){
        mi = new MutualInformation();
    }
    update();
}







