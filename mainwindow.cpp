#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&this->usbAlert, &qUSBListener::USBConnected,
            this, &this->USBConnect);
    connect(&this->usbAlert, &qUSBListener::USBDisconnected,
            this, &this->USBDisconnect);
    connect(&this->usbAlert, &qUSBListener::PortConnected,
            this, &this->PortConnect);
    connect(&this->usbAlert, &qUSBListener::PortDisconnected,
            this, &this->PortDisconnect);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::scrollWindow() {
    QTextCursor c  = ui->plainTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->plainTextEdit->setTextCursor(c);
}

void MainWindow::USBConnect(dbcc_name_usb dev) {
    ui->plainTextEdit->insertPlainText(QString("USB DEVICE CONNECTED: \n"));
    ui->plainTextEdit->insertPlainText(QString("    VID: 0x%1 \n").arg(dev.VID, 4, 16, QLatin1Char('0') ));
    ui->plainTextEdit->insertPlainText(QString("    PID: 0x%1 \n").arg(dev.PID, 4, 16, QLatin1Char('0') ));
    ui->plainTextEdit->insertPlainText("    S\\N: ");
    ui->plainTextEdit->insertPlainText(dev.serialNum);
    ui->plainTextEdit->insertPlainText(QString("\n\n"));

    scrollWindow();
}

void MainWindow::USBDisconnect(dbcc_name_usb dev) {
    ui->plainTextEdit->insertPlainText(QString("USB DEVICE DISCONNECTED: \n"));
    ui->plainTextEdit->insertPlainText(QString("    VID: 0x%1 \n").arg(dev.VID, 4, 16, QLatin1Char('0') ));
    ui->plainTextEdit->insertPlainText(QString("    PID: 0x%1 \n").arg(dev.PID, 4, 16, QLatin1Char('0') ));
    ui->plainTextEdit->insertPlainText("    S\\N: ");
    ui->plainTextEdit->insertPlainText(dev.serialNum);
    ui->plainTextEdit->insertPlainText(QString("\n\n"));

    scrollWindow();
}

void MainWindow::PortConnect(QString name) {
    ui->plainTextEdit->insertPlainText(QString("CONNECT: "));
    ui->plainTextEdit->insertPlainText(name);
    ui->plainTextEdit->insertPlainText(QString("\n\n"));

    scrollWindow();
}

void MainWindow::PortDisconnect(QString name) {
    ui->plainTextEdit->insertPlainText(QString("DISCONNECT: "));
    ui->plainTextEdit->insertPlainText(name);
    ui->plainTextEdit->insertPlainText(QString("\n\n"));

    scrollWindow();
}

void MainWindow::on_DevListenStart_clicked() {
    usbAlert.start();
}

void MainWindow::on_DevListen_stop_clicked() {
    usbAlert.stop();
}
