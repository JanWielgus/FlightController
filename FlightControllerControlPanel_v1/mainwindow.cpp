#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PID_tuning_checkBox_clicked(bool checked)
{
    ui->PID_tuning_box->setEnabled(checked);
    if (!ui->writeParamsAuto_checkBox->isChecked()) ui->writeParams_pushButton->setEnabled(true);
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    if (checked)
    {
        ui->P_PID_spinBox->setSingleStep(0.01);
        ui->I_PID_spinBox->setSingleStep(0.01);
        ui->D_PID_spinBox->setSingleStep(0.01);
        ui->Imax_PID_spinBox->setSingleStep(0.1);
    }
    else
    {
        ui->P_PID_spinBox->setSingleStep(0.1);
        ui->I_PID_spinBox->setSingleStep(0.1);
        ui->D_PID_spinBox->setSingleStep(0.1);
        ui->Imax_PID_spinBox->setSingleStep(1.0);
    }
}

void MainWindow::on_writeParamsAuto_checkBox_clicked(bool checked)
{
    ui->writeParams_pushButton->setEnabled(!checked);
}
