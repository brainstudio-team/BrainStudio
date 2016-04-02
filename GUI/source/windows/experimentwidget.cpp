#include "experimentwidget.h"
#include "ui_experimentwidget.h"

ExperimentWidget::ExperimentWidget(QWidget *parent):QWidget(parent),
                                                   ui(new Ui::ExperimentWidget){
    ui->setupUi(this);
}

ExperimentWidget::~ExperimentWidget(){
    delete ui;
}

void ExperimentWidget::write(const QString &msg){
    ui->console->addItem(msg);
}

void ExperimentWidget::clear(){
    ui->console->clear();
}

void ExperimentWidget::setProgress(const int &percentage){
    ui->progressBar->setValue(percentage);
}

void ExperimentWidget::setProgressGeneral(const int &percentage){
    ui->progressBarGeneral->setValue(percentage);
}

void ExperimentWidget::keyPressEvent(QKeyEvent * event){
         if(event->key()==Qt::Key_F3)   emit f3_xmlSignal();
    else if(event->key()==Qt::Key_F4)   emit f4_xmlSignal();
    else if(event->key()==Qt::Key_F6)   emit f6_xmlSignal();
    else if(event->key()==Qt::Key_F7)   emit f7_xmlSignal();
}
