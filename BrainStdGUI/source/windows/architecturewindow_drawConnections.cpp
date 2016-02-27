#include "architecturewindow.h"

/*
 *
 */
void ArchitectureWindow::drawConnections(QPainter &painter){ // SOS: it draws over and over again the same lines..! DOES IT?? TO CHECK!!
    // CONNECTIONS
    if(!show_connections)
        return;

    // If 'firing rates' or 'raster plots' mode - Firing rate connections:
    if(initialized && (mode == Block::modeC || mode == Block::modeRasters) ){
        //painter.setPen(QPen(Qt::gray, LINE_THICKNESS, Qt::SolidLine));
        for(BlockIter bl = blocks.begin(); bl != blocks.end(); bl++){
            for(int j=0; j<bl.value()->getConnectionNo(); j++){
                painter.setPen(QPen(QColor(bl.value()->
                                         getFiringRateBlockColour(j)),
                                         LINE_THICKNESS,Qt::SolidLine));
                if(bl.key() != bl.value()->getConnection(j))
                    drawConnection(painter, bl.key(), bl.value()->getConnection(j));
                else{
                    radius = bl.value()->width();
                    if(radius>bl.value()->height())
                        radius = bl.value()->height();
                    radius = (float)radius/4.0;
                    painter.drawEllipse(QPoint(bl.value()->x()+15,
                                        bl.value()->y()+15), radius, radius);
                }
            }
        }
    }
    // Else - Simple connections:
    else{
        QColor c;
        for(int i=0; i<connections.size(); i++){
            if(blocks.contains(connections[i].source()) &&
               blocks.contains(connections[i].target()) ){
                // Set colour and thickness
                if(connections[i].inhibitory())
                    c.setRgb(0,0,180,150);
                else
                    c.setRgb(100,100,100,180);

                if(connections[i].isFull())
                    painter.setPen(QPen(c, LINE_THICKNESS, Qt::SolidLine));
                else if(connections[i].isTopographic())
                    painter.setPen(QPen(c, LINE_THICKNESS, Qt::DotLine));
                else if(connections[i].isProbability())
                    painter.setPen(QPen(c, LINE_THICKNESS, Qt::SolidLine));
                else if(connections[i].isDistr())
                    painter.setPen(QPen(QColor(0,100,0), LINE_THICKNESS, Qt::DotLine));
                else if(connections[i].isFromFile())
                    painter.setPen(QPen(QColor(0,100,0), LINE_THICKNESS, Qt::DotLine));
                else
                    painter.setPen(QPen(c, LINE_THICKNESS, Qt::SolidLine));
                // Draw the line
                if(connections[i].source() != connections[i].target())
                    drawConnection(painter, connections[i].source(),
                                            connections[i].target());
                else{
                    radius = blocks[connections[i].source()]->width();
                    if(radius > blocks[connections[i].source()]->height())
                        radius = blocks[connections[i].source()]->height();
                    radius = (float)radius/4.0;
                    painter.drawEllipse(QPoint(
                                    blocks[connections[i].source()]->x()+15,
                                    blocks[connections[i].source()]->y()+15),
                                    radius, radius);
                }

            }
        }
    }
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
}


/*
 *
 */
void ArchitectureWindow::drawConnection(QPainter &painter, const QString &bl1, const QString &bl2){

    if( abs(blocks[bl2]->centreX()-blocks[bl1]->centreX()) >
            abs(blocks[bl2]->centreY()-blocks[bl1]->centreY()) ){
        if(blocks[bl2]->centreX() > blocks[bl1]->centreX())
             drawRightToLeft(painter, bl1, bl2);
        else drawLeftToRight(painter, bl1, bl2);
    }
    else{
        if(blocks[bl2]->centreY() > blocks[bl1]->centreY())
             drawDownToTop(painter, bl1, bl2);
        else drawTopToDown(painter, bl1, bl2);
    }
}

void ArchitectureWindow::drawLeftToRight(QPainter &painter, const QString &bl1, const QString &bl2){
    x1 = blocks[bl1]->x() + 1;                          // + 11;
    y1 = blocks[bl1]->y() + blocks[bl1]->height()/2;    // + 10;
    x2 = blocks[bl2]->x() + blocks[bl2]->width();       // + 13;
    y2 = blocks[bl2]->y() + blocks[bl2]->height()/2;    // + 10;
    xA = (x1+x2)/2;        yA = y1;
    xB = (x1+x2)/2;        yB = y2;

    QPainterPath myPath;
    myPath.moveTo(x1, y1);
    myPath.cubicTo(xA, yA, xB, yB, x2, y2);
    painter.drawPath(myPath);
    painter.drawEllipse(QPoint(x2, y2), 5, 5);
}

void ArchitectureWindow::drawRightToLeft(QPainter &painter, const QString &bl1, const QString &bl2){
    x1 = blocks[bl1]->x() + blocks[bl1]->width() - 1;   // + 9;
    y1 = blocks[bl1]->y() + blocks[bl1]->height()/2;    // + 10;
    x2 = blocks[bl2]->x();                              // + 7;
    y2 = blocks[bl2]->y() + blocks[bl2]->height()/2;    // + 10;
    xA = (x1+x2)/2;        yA = y1;
    xB = (x1+x2)/2;        yB = y2;

    QPainterPath myPath;
    myPath.moveTo(x1, y1);
    myPath.cubicTo(xA, yA, xB, yB, x2, y2);
    painter.drawPath(myPath);
    painter.drawEllipse(QPoint(x2, y2), 5, 5);
}

void ArchitectureWindow::drawTopToDown(QPainter &painter, const QString &bl1, const QString &bl2){
    x1 = blocks[bl1]->x() + blocks[bl1]->width()/2;
    y1 = blocks[bl1]->y() + 2;                          // + 12;
    x2 = blocks[bl2]->x() + blocks[bl2]->width()/2;
    y2 = blocks[bl2]->y() + blocks[bl2]->height();      // + 12;
    xA = x1;        yA = (y1+y2)/2;
    xB = x2;        yB = (y1+y2)/2;

    QPainterPath myPath;
    myPath.moveTo(x1, y1);
    myPath.cubicTo(xA, yA, xB, yB, x2, y2);
    painter.drawPath(myPath);
    painter.drawEllipse(QPoint(x2, y2), 5, 5);
}

void ArchitectureWindow::drawDownToTop(QPainter &painter, const QString &bl1, const QString &bl2){
    x1 = blocks[bl1]->x() + blocks[bl1]->width()/2;
    y1 = blocks[bl1]->y() + blocks[bl1]->height() - 2;  // + 7; // 12 - 5
    x2 = blocks[bl2]->x() + blocks[bl2]->width()/2;
    y2 = blocks[bl2]->y();                              // + 7; // 12 - 5
    xA = x1;        yA = (y1+y2)/2;
    xB = x2;        yB = (y1+y2)/2;

    QPainterPath myPath;
    myPath.moveTo(x1, y1);
    myPath.cubicTo(xA, yA, xB, yB, x2, y2);
    painter.drawPath(myPath);
    painter.drawEllipse(QPoint(x2, y2), 5, 5);
}


















/*
 *
 */
void ArchitectureWindow::drawConnection(QPainter &painter, const QString &bl1,
                                        const QPoint &tar){

    if(abs(tar.x()-blocks[bl1]->centreX())>abs(tar.y()-blocks[bl1]->centreY())){
        if(tar.x() > blocks[bl1]->centreX())
             drawRightToTarget(painter, bl1, tar);
        else drawLeftToTarget(painter, bl1, tar);
    }
    else{
        if(tar.y() > blocks[bl1]->centreY())
             drawDownToTarget(painter, bl1, tar);
        else drawTopToTarget(painter, bl1, tar);
    }
}

void ArchitectureWindow::drawLeftToTarget(QPainter &painter, const QString &bl1,
                                          const QPoint &tar){
    x1 = blocks[bl1]->x() + 11;
    y1 = blocks[bl1]->y() + blocks[bl1]->height()/2 + 10;
    xA = (x1+tar.x())/2;        yA = y1;
    xB = (x1+tar.x())/2;        yB = tar.y();

    QPainterPath myPath;
    myPath.moveTo(x1, y1);
    myPath.cubicTo(xA, yA, xB, yB, tar.x(), tar.y());
    painter.drawPath(myPath);
    painter.drawEllipse(QPoint(tar.x(), tar.y()), 5, 5);
}

void ArchitectureWindow::drawRightToTarget(QPainter &painter, const QString &bl1,
                                           const QPoint &tar){
    x1 = blocks[bl1]->x() + blocks[bl1]->width() + 9;
    y1 = blocks[bl1]->y() + blocks[bl1]->height()/2 + 10;
    xA = (x1+tar.x())/2;        yA = y1;
    xB = (x1+tar.x())/2;        yB = tar.y();

    QPainterPath myPath;
    myPath.moveTo(x1, y1);
    myPath.cubicTo(xA, yA, xB, yB, tar.x(), tar.y());
    painter.drawPath(myPath);
    painter.drawEllipse(QPoint(tar.x(), tar.y()), 5, 5);
}

void ArchitectureWindow::drawTopToTarget(QPainter &painter, const QString &bl1,
                                         const QPoint &tar){
    x1 = blocks[bl1]->x() + blocks[bl1]->width()/2;
    y1 = blocks[bl1]->y() + 12;
    xA = x1;        yA = (y1+tar.y())/2;
    xB = tar.x();   yB = (y1+tar.y())/2;

    QPainterPath myPath;
    myPath.moveTo(x1, y1);
    myPath.cubicTo(xA, yA, xB, yB, tar.x(), tar.y());
    painter.drawPath(myPath);
    painter.drawEllipse(QPoint(tar.x(), tar.y()), 5, 5);
}

void ArchitectureWindow::drawDownToTarget(QPainter &painter, const QString &bl1,
                                          const QPoint &tar){
    x1 = blocks[bl1]->x() + blocks[bl1]->width()/2;
    y1 = blocks[bl1]->y() + blocks[bl1]->height() + 7; // 12 - 5
    xA = x1;        yA = (y1+tar.y())/2;
    xB = tar.x();   yB = (y1+tar.y())/2;

    QPainterPath myPath;
    myPath.moveTo(x1, y1);
    myPath.cubicTo(xA, yA, xB, yB, tar.x(), tar.y());
    painter.drawPath(myPath);
    painter.drawEllipse(QPoint(tar.x(), tar.y()), 5, 5);
}