#ifndef BACKENDTERMINAL_H
#define BACKENDTERMINAL_H

#include <QDockWidget>
#include <QScrollBar>
#include "source/helper/userdata.h"

namespace Ui {
class BackEndTerminal;
}

class BackEndTerminal : public QDockWidget
{
    Q_OBJECT

public:
    explicit BackEndTerminal(QWidget *parent = 0);
    ~BackEndTerminal();

    void write(const QString &text);
private slots:
    void on_pushButton_clicked();

    void on_path_editingFinished();

    void on_pathPushButton_clicked();

signals:
    void restart_backend();

private:
    Ui::BackEndTerminal *ui;
};

#endif // BACKENDTERMINAL_H
