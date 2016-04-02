#ifndef PYTHONWIDGET_H
#define PYTHONWIDGET_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class PythonWidget;
}

class PythonWidget : public QWidget{
    Q_OBJECT
    
public:
    explicit PythonWidget(QWidget *parent = 0);
    ~PythonWidget();
    
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

protected:
    void keyPressEvent(QKeyEvent * event);

private:
    Ui::PythonWidget *ui;

signals:
    void buildPython_signal(QString);
    void f3_xmlSignal();
    void f4_xmlSignal();
    void f6_xmlSignal();
    void f7_xmlSignal();
};

#endif // PYTHONWIDGET_H
