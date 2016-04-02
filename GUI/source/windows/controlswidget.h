#ifndef CONTROLSWIDGET_H
#define CONTROLSWIDGET_H

#include <QWidget>

#include "source/windows/architecturewindow.h"

namespace Ui {
class ControlsWidget;
}

/* SOS: I need to try to not change anything in schema and A
 *
 */
class ControlsWidget : public QWidget{
    Q_OBJECT
private:
    Ui::ControlsWidget *ui;
    
    ArchitectureWindow *schema;

    // Methods
    void setEverythingFromSchema();

public:
    explicit ControlsWidget(ArchitectureWindow *_schema, QWidget *parent = 0);
    ~ControlsWidget();

    bool experiment();
    bool learning();
    bool updateDetails();
    bool updateSchema();
    bool updateConnections();

    bool modeStatesPixels() const;
    bool modeEdit() const;
    bool modeC() const;
    bool modeRasters() const;
    bool modeStatesPlots() const;
    bool modeNetwork() const;

    void setModeStatesPixels();
    void setModeEdit();
    void setModeC();
    void setModeRasters();
    void setModeStatesPlots();
    void setModeNetwork();
    bool setSchemaMode(const int &index);


signals:
    void setTimeSpeed(int);

private slots:
    void on_speedSlider_valueChanged(int value);
    void on_firingRateSlider_valueChanged();
    void on_experimentCheckBox_clicked(bool checked);

    void on_modeStatesRadioButton_clicked();
    void on_statesType_currentIndexChanged(const QString &arg1);
    void on_modeEditRadioButton_clicked();
    void on_modeCRadioButton_clicked();
    void on_modeRasterRadioButton_clicked();
    void on_modeNetworkRadioButton_clicked();
    void on_pushButton_clicked();
    void on_netAnalysischeckBox_clicked(bool checked);
    void on_networkMode_activated(const QString &arg1);
};

#endif // CONTROLSWIDGET_H

