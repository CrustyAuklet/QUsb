#ifndef USBDEVICE_H
#define USBDEVICE_H
#include <QString>

class usbDevice {
public:
    usbDevice() : VID(0), PID(0) { }
    usbDevice(uint16_t vid, uint16_t pid, QString sn);
    bool operator==(const usbDevice &other) const;
    bool operator!=(const usbDevice &other) const;
    operator QString() const;

    uint16_t VID;           // the device Vendor ID (16 bit value)
    uint16_t PID;           // the device Product ID (16 bit value)
    QString  serialNum;     // the device serial number
    QString  Manufacturer;  // the device manufacturer name
    QString  Product;       // the device product name
};

#endif // USBDEVICE_H
