#include "pythonwidget.h"
#include "ui_pythonwidget.h"

PythonWidget::PythonWidget(QWidget *parent) : QWidget(parent),
                                                       ui(new Ui::PythonWidget){
    ui->setupUi(this);
}

PythonWidget::~PythonWidget(){
    delete ui;
}

void PythonWidget::on_pushButton_2_clicked(){
    this->close();
}

void PythonWidget::on_pushButton_clicked(){
    emit buildPython_signal(ui->comboBox->currentText());
}

void PythonWidget::keyPressEvent(QKeyEvent * event){
         if(event->key()==Qt::Key_Escape) emit this->close();
    else if(event->key()==Qt::Key_F3)     emit f3_xmlSignal();
    else if(event->key()==Qt::Key_F4)     emit f4_xmlSignal();
    else if(event->key()==Qt::Key_F6)     emit f6_xmlSignal();
    else if(event->key()==Qt::Key_F7)     emit f7_xmlSignal();
}
