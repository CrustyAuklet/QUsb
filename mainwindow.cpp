#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    usbAlert.Start();

    connect(&this->usbAlert, &qUSBListener::DeviceConnected,
            this, &this->devConnect);
    connect(&this->usbAlert, &qUSBListener::DeviceDisconnected,
            this, &this->devDisconnect);
    connect(&this->usbAlert, &qUSBListener::PortConnected,
            this, &this->devConnect);
    connect(&this->usbAlert, &qUSBListener::PortDisconnected,
            this, &this->devDisconnect);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::devConnect(QString name) {
    ui->plainTextEdit->insertPlainText(QString("CONNECT: "));
    ui->plainTextEdit->insertPlainText(name);
    ui->plainTextEdit->insertPlainText(QString("\n\n"));

    QTextCursor c  = ui->plainTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->plainTextEdit->setTextCursor(c);
}

void MainWindow::devDisconnect(QString name) {
    ui->plainTextEdit->insertPlainText(QString("DISCONNECT: "));
    ui->plainTextEdit->insertPlainText(name);
    ui->plainTextEdit->insertPlainText(QString("\n\n"));

    QTextCursor c  = ui->plainTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->plainTextEdit->setTextCursor(c);
}
