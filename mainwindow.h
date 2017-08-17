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

private slots:
    void devConnect(QString name);
    void devDisconnect(QString name);

private:
    Ui::MainWindow *ui;
    qUSBListener usbAlert;
};

#endif // MAINWINDOW_H
