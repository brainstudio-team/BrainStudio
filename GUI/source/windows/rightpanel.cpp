#include "rightpanel.h"
#include "ui_rightpanel.h"

RightPanel::RightPanel(Simulator *_sim, Block *_b, QWidget *parent) :
                     sim(_sim),block(_b),QWidget(parent),ui(new Ui::RightPanel){
    ui->setupUi(this);

    ui->stimulationFrame->setVisible(false);
    ui->actionFrame->setVisible(false);
    this->resize(20,100);

    this->setMouseTracking(true);
}

RightPanel::~RightPanel(){
    delete ui;
}

void RightPanel::mouseMoveEvent(QMouseEvent *event){
    qDebug() << "RightPanel::mouseMoveEvent: event->pos =" << event->pos();
    this->update();
}

void RightPanel::mousePressEvent(QMouseEvent *event){
    qDebug() << "RightPanel::mousePressEvent: event->pos =" << event->pos();
}

void RightPanel::mouseReleaseEvent(QMouseEvent *event){
    qDebug() << "RightPanel::mouseReleaseEvent: event->pos =" << event->pos();
}

void RightPanel::mouseDoubleClickEvent(QMouseEvent *event){
    qDebug() << "RightPanel::mouseDoubleClickEvent: event->pos =" << event->pos();
}

void RightPanel::on_stimulationButton_clicked(){
    ui->actionFrame->setVisible(false);
    if(ui->stimulationFrame->isVisible()){
        ui->stimulationFrame->setVisible(false);
        this->resize(20,100);
    }
    else{
        ui->stimulationFrame->setVisible(true);
        this->resize(331,450);
    }
}

void RightPanel::on_connectionButton_clicked(){
    ui->stimulationFrame->setVisible(false);
    ui->actionFrame->setVisible(false);
    this->resize(20,100);
}

void RightPanel::on_actionsButton_clicked(){
    ui->stimulationFrame->setVisible(false);
    if(ui->actionFrame->isVisible()){
        ui->actionFrame->setVisible(false);
        this->resize(20,100);
    }
    else{
        ui->actionFrame->setVisible(true);
        this->resize(331,450);
    }
}

void RightPanel::on_plotButton_clicked(){
    ui->stimulationFrame->setVisible(false);
    ui->actionFrame->setVisible(false);
    this->resize(20,100);
}

void RightPanel::on_actionExecuteButton_clicked(){
    QString chname = block->getId();
    sim->add_action("{\"type\":\"adjust\",\"node\":\"" +
                    chname.replace(' ', '+') +
                    "\",\"parameter\":\"" + ui->actionParameter->currentText() +
                    "\",\"value\":" +
                    QString::number(ui->actionValue->value()) + "}");


}




















