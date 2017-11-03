#include <algorithm>
#include "usbdevicelistmodel.h"


UsbDeviceListModel::UsbDeviceListModel(QObject *parent)
    : QAbstractTableModel(parent)
{ }

int UsbDeviceListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return deviceList.size();
}

int UsbDeviceListModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return USB_DEV_ITEM_COUNT;
}

QVariant UsbDeviceListModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        QString returnVal;
        const usbDevice row = deviceList.at(index.row());
        switch(index.column()) {
            case USB_DEV_VID :
                returnVal = QString("0x%1").arg(row.VID, 4, 16, QLatin1Char('0'));
                break;
            case USB_DEV_PID :
                returnVal = QString("0x%1").arg(row.PID, 4, 16, QLatin1Char('0'));
                break;
            case USB_DEV_SERIAL_NUMBER_STR :
                returnVal = row.serialNum;
                break;
            case USB_DEV_MANUFACTURER_STR :
                returnVal = row.Manufacturer;
                break;
            case USB_DEV_PRODUCT_STR :
                returnVal = row.Product;
                break;
            default:
                returnVal = "OUT OF RANGE COLUMN!";
                break;
        }
        return returnVal;
    }
    else if(role == Qt::FontRole) {

    }
    else if(role == Qt::BackgroundRole) {

    }
    else if(role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter + Qt::AlignVCenter;
    }
    else if(role == Qt::CheckStateRole) {

    }
    return QVariant();
}

QVariant UsbDeviceListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        QString returnVal;

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case USB_DEV_VID :
                    returnVal = "VID";
                    break;
                case USB_DEV_PID :
                    returnVal = "PID";
                    break;
                case USB_DEV_SERIAL_NUMBER_STR :
                    returnVal = "Serial Number";
                    break;
                case USB_DEV_MANUFACTURER_STR :
                    returnVal = "Manufacturer";
                    break;
                case USB_DEV_PRODUCT_STR :
                    returnVal = "Product";
                    break;
                default:
                    returnVal = "OUT OF RANGE COLUMN!";
                    break;
            }
        }
//        else if(orientation == Qt::Vertical) {
//            returnVal = QString("DEV %1").arg(section);
//        }
        return returnVal;
    }
    return QVariant();
}

void UsbDeviceListModel::sort(int column, Qt::SortOrder order) {
    emit layoutAboutToBeChanged();

    switch(column) {
    case USB_DEV_VID:
        if(order == Qt::DescendingOrder) {
            std::sort(deviceList.begin(), deviceList.end(), [](const usbDevice &l, const usbDevice &r){return l.VID > r.VID;});
        }
        else {
            std::sort(deviceList.begin(), deviceList.end(), [](const usbDevice &l, const usbDevice &r){return l.VID < r.VID;});
        }
        break;

    case USB_DEV_PID:
        if(order == Qt::DescendingOrder) {
            std::sort(deviceList.begin(), deviceList.end(), [](const usbDevice &l, const usbDevice &r){return l.PID > r.PID;});
        }
        else {
            std::sort(deviceList.begin(), deviceList.end(), [](const usbDevice &l, const usbDevice &r){return l.PID < r.PID;});
        }
        break;

    case USB_DEV_SERIAL_NUMBER_STR:
        if(order == Qt::DescendingOrder) {
            std::sort(deviceList.begin(), deviceList.end(), [](const usbDevice &l, const usbDevice &r){return l.serialNum > r.serialNum;});
        }
        else {
            std::sort(deviceList.begin(), deviceList.end(), [](const usbDevice &l, const usbDevice &r){return l.serialNum < r.serialNum;});
        }
        break;

    case USB_DEV_MANUFACTURER_STR:
        if(order == Qt::DescendingOrder) {
            std::sort(deviceList.begin(), deviceList.end(), [](const usbDevice &l, const usbDevice &r){return l.Manufacturer > r.Manufacturer;});
        }
        else {
            std::sort(deviceList.begin(), deviceList.end(), [](const usbDevice &l, const usbDevice &r){return l.Manufacturer < r.Manufacturer;});
        }
        break;

    case USB_DEV_PRODUCT_STR:
        if(order == Qt::DescendingOrder) {
            std::sort(deviceList.begin(), deviceList.end(), [](const usbDevice &l, const usbDevice &r){return l.Product > r.Product;});
        }
        else {
            std::sort(deviceList.begin(), deviceList.end(), [](const usbDevice &l, const usbDevice &r){return l.Product < r.Product;});
        }
        break;

    default:
        break;
    }

    emit layoutChanged();
}

void UsbDeviceListModel::replaceData(QVector<usbDevice> devList) {
    deviceList = devList;
    emit dataChanged(index(0,0), index(USB_DEV_ITEM_COUNT-1, deviceList.count()-1) );
}

void UsbDeviceListModel::addDevice(usbDevice attachedDev) {
    if(!deviceList.contains(attachedDev)) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        deviceList.append(attachedDev);
        endInsertRows();
    }
}

void UsbDeviceListModel::removeDevice(usbDevice removedDev) {
    int index = deviceList.indexOf(removedDev);
    if(index >= 0) {
        beginRemoveRows(QModelIndex(), index, index);
        deviceList.remove(index);
        endRemoveRows();
    }
}

usbDevice UsbDeviceListModel::getDevice(const int row) {
    return deviceList.at(row);
}
