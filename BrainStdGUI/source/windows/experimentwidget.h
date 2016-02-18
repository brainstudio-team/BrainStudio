#ifndef EXPERIMENTWIDGET_H
#define EXPERIMENTWIDGET_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class ExperimentWidget;
}

class ExperimentWidget : public QWidget{
    Q_OBJECT
    
public:
    explicit ExperimentWidget(QWidget *parent = 0);
    ~ExperimentWidget();

    void write(const QString &msg);
    void clear();

    void setProgress(const int &percentage);
    void setProgressGeneral(const int &percentage);

protected:
    void keyPressEvent(QKeyEvent * event);

private:
    Ui::ExperimentWidget *ui;

signals:
    void f3_xmlSignal();
    void f4_xmlSignal();
    void f6_xmlSignal();
    void f7_xmlSignal();
};

#endif // EXPERIMENTWIDGET_H
