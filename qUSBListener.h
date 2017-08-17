#ifndef QUSBLISTENER_H
#define QUSBLISTENER_H

#include <QMainWindow>
#include <windows.h>
#include <dbt.h>

struct usb_id {
    uint16_t VID;
    uint16_t PID;
    GUID dev_guid;
    GUID class_guid;
};

class qUSBListener : public QMainWindow
{
    Q_OBJECT

public:
    qUSBListener();
    bool Start();

signals:
    void DeviceConnected(QString name);
    void DeviceDisconnected(QString name);
    void PortConnected(QString portName);
    void PortDisconnected(QString portName);

protected:
    virtual bool nativeEvent(const QByteArray & eventType,
                             void * message,
                             long * result);
};

#endif // QUSBLISTENER_H
