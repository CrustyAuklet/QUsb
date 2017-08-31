#ifndef USBDEVICELISTMODEL_H
#define USBDEVICELISTMODEL_H

#include <QAbstractTableModel>
#include <QDebug>
#include "usbdevice.h"

enum usbDev_Items {
    USB_DEV_VID,
    USB_DEV_PID,
    USB_DEV_SERIAL_NUMBER_STR,
    USB_DEV_MANUFACTURER_STR,
    USB_DEV_PRODUCT_STR,
    USB_DEV_ITEM_COUNT
};

class UsbDeviceListModel : public QAbstractTableModel {
    Q_OBJECT
public:
    /*** Table View model functions ***/
    UsbDeviceListModel(QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /// gets the device at a specific index
    usbDevice getDevice(const int row);

signals:
    /// pleads someone, anyone, for an update. usually timer based.
    void updateMePlease();

public slots:
    /// updates the data in the model by erasing the old data and replacing it
    void updateData(QVector<usbDevice> devList);

private:
    QVector<usbDevice> deviceList;  // list of USB devices
    int                timerID;     // ID for refresh timer
};

#endif // USBDEVICELISTMODEL_H
