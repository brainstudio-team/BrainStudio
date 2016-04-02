#include "backendterminal.h"
#include "ui_BackEndTerminalDock.h"

BackEndTerminal::BackEndTerminal(QWidget *parent) :
    QDockWidget(tr("Back-end terminal"), parent), ui(new Ui::BackEndTerminal){
    ui->setupUi(this);

    ui->path->setText(UserData::backend_path);
    ui->path->setVisible(false);
}

BackEndTerminal::~BackEndTerminal(){
    delete ui;
}


void BackEndTerminal::write(const QString &text){
    /*if(ui->textEdit->size() > 30) {
        ui->textEdit->r
    }*/
    ui->textEdit->append(text);
    ui->textEdit->verticalScrollBar()->setValue(
                ui->textEdit->verticalScrollBar()->maximum());
}

void BackEndTerminal::on_pushButton_clicked(){
    ui->textEdit->clear();
}

void BackEndTerminal::on_path_editingFinished(){
    UserData::backend_path = ui->path->text();
    UserData::save();
    ui->path->setVisible(true);
    emit restart_backend();
}

void BackEndTerminal::on_pathPushButton_clicked(){
    if(ui->path->isVisible()){
        ui->pathPushButton->setText("Set path");
        ui->frame->setVisible(true);
        ui->path->setVisible(false);
    }
    else{
        ui->pathPushButton->setText("OK");
        ui->frame->setVisible(false);
        ui->path->setVisible(true);
    }
}
