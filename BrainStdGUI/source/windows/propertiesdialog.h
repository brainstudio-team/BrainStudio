#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>

#include "../helper/userdata.h"

namespace Ui {
class PropertiesDialog;
}

class PropertiesDialog : public QDialog{
    Q_OBJECT

public:
    explicit PropertiesDialog(QWidget *parent = 0);
    ~PropertiesDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PropertiesDialog *ui;
};

#endif // PROPERTIESDIALOG_H
