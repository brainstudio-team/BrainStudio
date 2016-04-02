#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QDialog>
#include <QStandardPaths>
#include "source/helper/userdata.h"

namespace Ui {
    class WelcomeWindow;
}

class WelcomeWindow : public QDialog{
    Q_OBJECT
private:
    Ui::WelcomeWindow *ui;

public:
    explicit WelcomeWindow(QWidget *parent = 0);
    ~WelcomeWindow();
    
private slots:
    void on_commandLinkButton_clicked();

signals:
    void allDone();
};

#endif // WELCOMEWINDOW_H
