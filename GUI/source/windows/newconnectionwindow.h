#ifndef NEWCONNECTIONWINDOW_H
#define NEWCONNECTIONWINDOW_H

#include <QtGui>
#include "architecturewindow.h"
#include "source/helper/backendData.h"


namespace Ui {
    class NewConnectionWindow;
}

class NewConnectionWindow : public QDialog
{
    Q_OBJECT
private:
    Ui::NewConnectionWindow *ui;

protected:
    ArchitectureWindow *schema;
    QString source, target;
    QMap<QString, QString> params;

    // Fields
    QGridLayout *fieldsLayout;
    QList<QWidget*> fieldsWidget;
    QList<QLabel*> fieldsLabel;

    // Parameters
    QGridLayout *parsLayout;
    QList<QLineEdit*> parsLineEdit;
    QList<QLabel*> parsLabel;

    // States
    QGridLayout *statesLayout;
    QList<QLineEdit*> statesLineEdit;
    QList<QLabel*> statesLabel;

    void updateConnectionModels();
    void updateParameters(QString model);

    void createEdge();

public:
    explicit NewConnectionWindow(ArchitectureWindow *_schema,
                                 const QString _source, const QString _target,
                                 QWidget *parent = 0);
    ~NewConnectionWindow(){ delete ui; }

private slots:
    void on_addButton_clicked();
    void on_cancelButton_clicked(){ this->close(); }

    void on_fullRadioButton_clicked();
    void on_topographicRadioButton_clicked();
    void on_pRadioButton_clicked();
    void on_formulaRadioButton_clicked();
    void on_fileRadioButton_clicked();
    void on_connectionModelComboBox_currentTextChanged(const QString &arg1);
    void on_preLastSpinBox_valueChanged(int arg1);
    void on_postLastSpinBox_valueChanged(int arg1);
};

#endif // NEWCONNECTIONWINDOW_H
