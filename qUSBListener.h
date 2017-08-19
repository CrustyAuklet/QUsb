#ifndef QUSBLISTENER_H
#define QUSBLISTENER_H

#include <QMainWindow>
#include <windows.h>
#include <dbt.h>

class dbcc_name_usb {
public:
    dbcc_name_usb();
    dbcc_name_usb(uint16_t vid, uint16_t pid, QString sn);
    bool operator==(const dbcc_name_usb &other) const;
    bool operator!=(const dbcc_name_usb &other) const;

    uint16_t VID;
    uint16_t PID;
    QString  serialNum;
    //GUID     class_guid;
};

class qUSBListener : public QMainWindow
{
    Q_OBJECT

public:
    qUSBListener();
    bool start(const uint16_t vid = 0, const uint16_t pid = 0, const QString sn = "");
    bool stop();

signals:
    void USBConnected(dbcc_name_usb name);
    void USBDisconnected(dbcc_name_usb name);
    void PortConnected(QString portName);
    void PortDisconnected(QString portName);

protected:
    virtual bool nativeEvent(const QByteArray & eventType,
                             void * message,
                             long * result);

    bool getDevData(LPARAM lParamDev, dbcc_name_usb &newDevice);
    inline QString getQstring(void* stringPtr);

private:
    HDEVNOTIFY      devNotify;
    dbcc_name_usb*  targetDev;
};

#endif // QUSBLISTENER_H
