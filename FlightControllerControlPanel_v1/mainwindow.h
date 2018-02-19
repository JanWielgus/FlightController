#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "komunikacja.h"
#include <QString>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_PID_tuning_checkBox_clicked(bool checked);

    void on_checkBox_clicked(bool checked);

    void on_writeParamsAuto_checkBox_clicked(bool checked);

    void readSerial();

    void on_searchGS_pushButton_clicked();

    void on_connectGS_pushButton_clicked();

    void on_endConnectGS_pushButton_clicked();

    void timerPIDaction();
    void on_writeParams_pushButton_clicked();

    void on_P_PID_spinBox_valueChanged(double arg1);

    void on_I_PID_spinBox_valueChanged(double arg1);

    void on_D_PID_spinBox_valueChanged(double arg1);

    void on_Imax_PID_spinBox_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    QTimer* timer;

    QString port_name;

    bool needToSendPID = false;

    const QString notConnectedText = "Not connected";
    const QString connectedText = "Connected";
    const QString notFoundText = "Not found";
    const QString foundText = "Found";
    const QString noConnectionText = "No connection";
};

#endif // MAINWINDOW_H
