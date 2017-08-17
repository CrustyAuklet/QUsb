#ifndef QUSBLISTENER_H
#define QUSBLISTENER_H

#include <QMainWindow>
#include <windows.h>
#include <dbt.h>

struct usb_id {
    uint16_t VID;
    uint16_t PID;
    QString  serialNum;
    GUID     class_guid;
};

class qUSBListener : public QMainWindow
{
    Q_OBJECT

public:
    qUSBListener();
    bool Start();
    bool Stop();

signals:
    void USBConnected(usb_id name);
    void USBDisconnected(usb_id name);
    void PortConnected(QString portName);
    void PortDisconnected(QString portName);

protected:
    virtual bool nativeEvent(const QByteArray & eventType,
                             void * message,
                             long * result);

    bool getDevData(LPARAM lParamDev, usb_id &newDevice);
    inline QString getQstring(void* stringPtr);

private:
    HDEVNOTIFY devNotify;
};

#endif // QUSBLISTENER_H
