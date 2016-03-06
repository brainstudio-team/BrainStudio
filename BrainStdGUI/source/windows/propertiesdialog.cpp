#include "propertiesdialog.h"
#include "ui_propertiesdialog.h"

PropertiesDialog::PropertiesDialog(QWidget *parent) : QDialog(parent),
                                                   ui(new Ui::PropertiesDialog){
    ui->setupUi(this);
    ui->workspace_path->setText(UserData::workspace_path);
    ui->backend_path->setText(UserData::backend_path);
    ui->tcpip_hostname->setText(UserData::tcpip_hostname);
    ui->tcpip_port->setValue(UserData::tcpip_port);
    if(UserData::backend_type == UserData::BCKEND_INTERNAL)
        ui->internal_backend_check_box->setChecked(true);
    else
        ui->internal_backend_check_box->setChecked(false);
}

PropertiesDialog::~PropertiesDialog(){
    delete ui;
}

void PropertiesDialog::on_buttonBox_accepted(){
    UserData::workspace_path = ui->workspace_path->text();
    UserData::backend_path = ui->backend_path->text();
    UserData::tcpip_hostname = ui->tcpip_hostname->text();
    UserData::tcpip_port = ui->tcpip_port->value();
    if(ui->internal_backend_check_box->isChecked())
        UserData::backend_type = true;
    else
        UserData::backend_type = false;
    UserData::save();
}
