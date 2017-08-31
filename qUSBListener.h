#ifndef QUSBLISTENER_H
#define QUSBLISTENER_H

#include <QMainWindow>
#include <windows.h>
#include <dbt.h>
#include "usbdevice.h"

class qUSBListener : public QMainWindow
{
    Q_OBJECT
public:
    qUSBListener();
    bool start(const uint16_t vid = 0, const uint16_t pid = 0, const QString sn = "");
    bool stop();

signals:
    void USBConnected(usbDevice name);
    void USBDisconnected(usbDevice name);
    void PortConnected(QString portName);
    void PortDisconnected(QString portName);

protected:
    virtual bool nativeEvent(const QByteArray & eventType,
                             void * message,
                             long * result);

    bool getDevData(LPARAM lParamDev, usbDevice &newDevice);
    inline QString getQstring(void* stringPtr);

private:
    HDEVNOTIFY      devNotify;
    usbDevice*  targetDev;
};

#endif // QUSBLISTENER_H
