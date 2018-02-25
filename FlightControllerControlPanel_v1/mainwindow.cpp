#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerPIDaction()));
    timer->start(1000);

    komun.arduino = new QSerialPort(this);
    komun.pSerial = new PacketSerial;
    komun.buforT = new uint8_t[komun.pSerial->MAX_SEND_SIZE];

    komun.initialize();

    // Nie dziala: QObject::connect(komun.arduino, SIGNAL(destroyed(QObject*)), this, SLOT(destroySerial())); // chodzi o rozlaczanie gdy wyjety kabel
    QObject::connect(komun.arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
}

MainWindow::~MainWindow()
{
    if (komun.arduino->isOpen())
    {
        komun.arduino->close(); // Close the serial port if it's open.
    }

    delete ui;
}


void MainWindow::readSerial()
{
    if (ui->connectionStateGS_label->text() == connectedText)
    {
        komun.odbierz();
        qDebug() << "Odebrano jakies dane\n";

        ui->throttle_slider->setValue(komun.pilot.throttle);
        ui->pitch_slider->setValue(komun.pilot.tilt_TB);
        ui->roll_slider->setValue(komun.pilot.tilt_LR);
        ui->yaw_slider->setValue(komun.pilot.rotate);
    }
}


void MainWindow::timerPIDaction()
{
    // Jeśli zmiana w parametrach pid i jest polączony
    if (needToSendPID && ui->writeParamsAuto_checkBox->isChecked() && ui->connectionStateGS_label->text() == connectedText)
    {
        // Wyślij parametry do pilota
        komun.wyslij(komun.KOMUN_RAMKA_PC_PARAMETERS_TYPE);

        needToSendPID = false;
        ui->paramsSendingState_label->setText(sentText);
    }
    else if (ui->paramsSendingState_label->text() == sentText)
        ui->paramsSendingState_label->setText(noChangesText);
}


void MainWindow::on_precisionTuning_checkBox_clicked(bool checked)
{
    if (checked)
    {
        ui->P_PID_spinBox->setSingleStep(0.01);
        ui->I_PID_spinBox->setSingleStep(0.01);
        ui->D_PID_spinBox->setSingleStep(0.01);
    }
    else
    {
        ui->P_PID_spinBox->setSingleStep(0.1);
        ui->I_PID_spinBox->setSingleStep(0.1);
        ui->D_PID_spinBox->setSingleStep(0.1);
    }
}


void MainWindow::on_PID_tuning_checkBox_clicked(bool checked)
{
    ui->PID_tuning_box->setEnabled(checked);
    if (!ui->writeParamsAuto_checkBox->isChecked()) ui->writeParams_pushButton->setEnabled(true);
}

void MainWindow::on_writeParamsAuto_checkBox_clicked(bool checked)
{
    ui->writeParams_pushButton->setEnabled(!checked);
}

void MainWindow::on_searchGS_pushButton_clicked()
{
    ui->listOfDevicesGS_comboBox->clear();

    bool arduino_is_available = false;

    //  For each available serial port
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        //  check if the serialport has both a product identifier and a vendor identifier
        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier())
        {
            port_name = serialPortInfo.portName();
            ui->listOfDevicesGS_comboBox->addItem(port_name);

            //  check if the product ID and the vendor ID match those of the arduino uno
            if((serialPortInfo.productIdentifier() == komun.arduino_uno_product_id) && (serialPortInfo.vendorIdentifier() == komun.arduino_uno_vendor_id))
            {
                arduino_is_available = true; //    arduino uno is available on this port
                ui->gs_arduino_state_label->setText(foundText);
            }
        }
    }

    if (arduino_is_available == false)
        ui->gs_arduino_state_label->setText(notFoundText);
}

void MainWindow::on_connectGS_pushButton_clicked()
{
    // If arduino is not connected, then you can search
    if (ui->connectionStateGS_label->text() == notConnectedText)
    {
        // If something was selected on list of devices
        if (ui->listOfDevicesGS_comboBox->currentText() != "")
        {
            qDebug() << "Found the arduino port...\n";
            komun.arduino->setPortName(port_name);
            komun.arduino->open(QSerialPort::ReadWrite);
            komun.arduino->setBaudRate(QSerialPort::Baud9600); // BAUD RATE !!!!
            komun.arduino->setDataBits(QSerialPort::Data8);
            komun.arduino->setFlowControl(QSerialPort::NoFlowControl);
            komun.arduino->setParity(QSerialPort::NoParity);
            komun.arduino->setStopBits(QSerialPort::OneStop);

            ui->listOfDevicesGS_comboBox->clear();
            ui->gs_arduino_state_label->setText(notFoundText);
            ui->connectionStateGS_label->setText(connectedText);
            ui->GS_ConnectionStatus_label->setText(connectedText); // main toolbar
            ui->connectGS_pushButton->setDisabled(true);
        }
        else
        {
            qDebug() << "You don't have any devices to connect!\n";
            ui->GS_ConnectionStatus_label->setText(noConnectionText);
        }
    }
}

void MainWindow::on_endConnectGS_pushButton_clicked()
{
    // If is connected and could be closed
    if (ui->connectionStateGS_label->text() == connectedText)
    {
        if (komun.arduino->isOpen())
        {
            komun.arduino->close(); // Close the serial port if it's open.
        }

        ui->connectionStateGS_label->setText(notConnectedText);
        ui->GS_ConnectionStatus_label->setText(noConnectionText);
        ui->connectGS_pushButton->setDisabled(false);
    }
}


void MainWindow::on_writeParams_pushButton_clicked()
{
    // Wyślij parametry do pilota
    if (needToSendPID && ui->connectionStateGS_label->text() == connectedText)
    {
        komun.wyslij(komun.KOMUN_RAMKA_PC_PARAMETERS_TYPE);
        ui->paramsSendingState_label->setText(sentText);
        needToSendPID = false;
    }
}

void MainWindow::on_P_PID_spinBox_valueChanged(double arg1)
{
    needToSendPID = true;
    ui->paramsSendingState_label->setText(needToSendText);
    komun.conf.kP_level.value = (float)ui->P_PID_spinBox->value();
}

void MainWindow::on_I_PID_spinBox_valueChanged(double arg1)
{
    needToSendPID = true;
    ui->paramsSendingState_label->setText(needToSendText);
    komun.conf.kI_level.value = (float)ui->I_PID_spinBox->value();
}

void MainWindow::on_D_PID_spinBox_valueChanged(double arg1)
{
    needToSendPID = true;
    ui->paramsSendingState_label->setText(needToSendText);
    komun.conf.kD_level.value = (float)ui->D_PID_spinBox->value();
}

void MainWindow::on_Imax_PID_spinBox_valueChanged(int arg1)
{
    needToSendPID = true;
    ui->paramsSendingState_label->setText(needToSendText);
    komun.conf.I_level_limiter = (uint8_t)ui->Imax_PID_spinBox->value();
}
