#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // attach signals from listener to the terminal window
    connect(&this->usbAlert, &qUSBListener::USBConnected,
            this, &this->USBConnect);
    connect(&this->usbAlert, &qUSBListener::USBDisconnected,
            this, &this->USBDisconnect);
    connect(&this->usbAlert, &qUSBListener::PortConnected,
            this, &this->PortConnect);
    connect(&this->usbAlert, &qUSBListener::PortDisconnected,
            this, &this->PortDisconnect);

    // set the model for the table view and the appearance of table
    ui->tableView->setModel(&deviceList);
    ui->tableView->setShowGrid(false);
    ui->tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
    ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection );
    ui->tableView->setSortingEnabled(true);

    // attach listener signals to the table view
    connect(&this->usbAlert, &qUSBListener::USBConnected,
            &this->deviceList, &UsbDeviceListModel::addDevice);
    connect(&this->usbAlert, &qUSBListener::USBDisconnected,
            &this->deviceList, &UsbDeviceListModel::removeDevice);

    // start detection
    usbAlert.start();
}

MainWindow::~MainWindow() {
    usbAlert.stop();
    delete ui;
}

void MainWindow::scrollWindow() {
    QTextCursor c  = ui->plainTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->plainTextEdit->setTextCursor(c);
}

void MainWindow::USBConnect(usbDevice dev) {
    ui->plainTextEdit->insertPlainText(QString("USB DEVICE CONNECTED: \n"));
    ui->plainTextEdit->insertPlainText(QString("    VID: 0x%1 \n").arg(dev.VID, 4, 16, QLatin1Char('0') ));
    ui->plainTextEdit->insertPlainText(QString("    PID: 0x%1 \n").arg(dev.PID, 4, 16, QLatin1Char('0') ));
    ui->plainTextEdit->insertPlainText("    S\\N: ");
    ui->plainTextEdit->insertPlainText(dev.serialNum);
    ui->plainTextEdit->insertPlainText(QString("\n\n"));

    scrollWindow();
}

void MainWindow::USBDisconnect(usbDevice dev) {
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
