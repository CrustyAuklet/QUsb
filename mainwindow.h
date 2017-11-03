#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qUSBListener.h"
#include "usbdevicelistmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void scrollWindow();

private slots:
    void USBConnect(usbDevice dev);
    void USBDisconnect(usbDevice dev);
    void PortConnect(QString name);
    void PortDisconnect(QString name);

private:
    Ui::MainWindow *ui;
    qUSBListener usbAlert;          // class for listening for USB hotplug events
    UsbDeviceListModel  deviceList; // MVC model for USB list
};

#endif // MAINWINDOW_H
