#include "aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent),
                                                        ui(new Ui::AboutWindow){
    ui->setupUi(this);
    ui->bugButton->setEnabled(false);
}

AboutWindow::~AboutWindow(){
    delete ui;
}

void AboutWindow::on_closeButton_clicked(){
    this->close();
}
