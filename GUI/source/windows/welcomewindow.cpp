#include "welcomewindow.h"
#include "ui_welcomewindow.h"

WelcomeWindow::WelcomeWindow(QWidget *parent): QDialog(parent),
                                                     ui(new Ui::WelcomeWindow){
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    ui->pathLineEdit->setText(
               QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
                + "/workspace");
    ui->backendPathLineEdit->setText(
          QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)
          + "/BrainStudio/backends/BrainStudioBackend");
}

WelcomeWindow::~WelcomeWindow(){
    delete ui;
}

void WelcomeWindow::on_commandLinkButton_clicked(){
    if(ui->stackedWidget->currentIndex() == 0){
        ui->stackedWidget->setCurrentIndex(1);
        ui->commandLinkButton->setText("Start Brain Studio");
    }
    else{
        UserData::workspace_path = ui->pathLineEdit->text();
        UserData::backend_path = ui->backendPathLineEdit->text();
        UserData::save();
        emit allDone();
        this->close();
    }
}
