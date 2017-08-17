#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qUSBListener.h"

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
    void USBConnect(usb_id dev);
    void USBDisconnect(usb_id dev);
    void PortConnect(QString name);
    void PortDisconnect(QString name);
    void on_DevListenStart_clicked();
    void on_DevListen_stop_clicked();

private:
    Ui::MainWindow *ui;
    qUSBListener usbAlert;
};

#endif // MAINWINDOW_H
