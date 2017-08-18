#include "qUSBListener.h"
#include <QDebug>
#include <QRegularExpression>
#include <QStringList>

/*********************************************************************************************
 * DBCC USB DEVICE CLASS FOR LISTING DEVICES
 *********************************************************************************************/

dbcc_name_usb::dbcc_name_usb() {
    VID       = 0;
    PID       = 0;
    serialNum.clear();
}

dbcc_name_usb::dbcc_name_usb(uint16_t vid, uint16_t pid, QString sn) {
    VID       = vid;
    PID       = pid;
    serialNum = sn;
}

bool dbcc_name_usb::operator==(const dbcc_name_usb &other) const {
    bool vidMatch = other.VID == 0 || this->VID == other.VID;
    bool pidMatch = other.PID == 0 || this->PID == other.PID;
    bool snMatch  = other.serialNum.isEmpty() || this->serialNum == other.serialNum;
    return vidMatch && pidMatch && snMatch;
}

bool dbcc_name_usb::operator!=(const dbcc_name_usb &other) const {
    return !(*this == other);
}

/*********************************************************************************************
 * USB LISTENER CLASS
 *********************************************************************************************/

qUSBListener::qUSBListener() : QMainWindow() {
    devNotify = NULL;
    targetDev = NULL;
}

bool qUSBListener::start(const uint16_t vid, const uint16_t pid, const QString sn) {
    targetDev = new dbcc_name_usb(vid, pid, sn);

    GUID usbGUID[] = {
        { 0xa5dcbf10, 0x6530, 0x11d2, { 0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed } },     // All USB Devices
        { 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },
        { 0x4d1e55b2, 0xf16f, 0x11Cf, { 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },
        { 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } },
        { 0x219d0508, 0x57a8, 0x4ff5, { 0x97, 0xa1, 0xbd, 0x86, 0x58, 0x7c, 0x6c, 0x7e } },
        { 0x4d36e967, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } },     // COM PORTS
        {0x86e0d1e0L, 0x8089, 0x11d0, { 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73 } },
    };

    DEV_BROADCAST_DEVICEINTERFACE notificationFilter;

    memset(&notificationFilter, 0, sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    notificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    notificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    notificationFilter.dbcc_classguid  = usbGUID[0];

    devNotify = RegisterDeviceNotification((HANDLE)this->winId(),
                                           &notificationFilter,
                                           DEVICE_NOTIFY_WINDOW_HANDLE);

    return devNotify != NULL;
}

bool qUSBListener::stop() {
    // clear out target device and pre-laod return value
    if(targetDev != NULL) {
        delete targetDev;
        targetDev = NULL;
    }
    bool success = true;

    // Don't dallocate a NULL notification handle. output any errors.
    if(devNotify != NULL){
        success = UnregisterDeviceNotification(devNotify);
        devNotify = NULL;
        if(!success) {
            DWORD err = GetLastError();     // error from last call
            LPVOID msgBuf;                  // buffer to hold error message

            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                          FORMAT_MESSAGE_FROM_SYSTEM |
                          FORMAT_MESSAGE_IGNORE_INSERTS,
                          NULL, err, 0, (LPTSTR)&msgBuf, 0, NULL);

            qDebug() << "UNREGISTER_DEVICE_NOTIFICATION FAILED WITH ERROR " << err
                     << ": " << QString::fromStdWString((LPTSTR)msgBuf);

            LocalFree(msgBuf);
        }
    }
    return success;
}

bool qUSBListener::nativeEvent(const QByteArray & eventType,
                                    void * message,
                                    long * result)
{
    Q_UNUSED(result);
    Q_UNUSED(eventType);

    /****************************************************************************************
     * NOTE: The order of casting and decisisons is not the most effecient but is
     *       derived from the MSDN documentation to prevent segfaults. Specifically
     *       You can not cast to a specific device type truct until you know what type
     *       of event has arrived. Even thought we only care about two of them, many events
     *       under the WM_DEVICECHANGE message have a null value in lParam
     ****************************************************************************************/

    // MSG structure      -> https://msdn.microsoft.com/en-us/library/windows/desktop/ms644958(v=vs.85).aspx
    // list of msg values -> https://wiki.winehq.org/List_Of_Windows_Messages
    MSG* msg = reinterpret_cast<MSG*>(message);

    // Does this specific message interest us? WM_DEVICE CHANGE -> https://msdn.microsoft.com/en-us/library/aa363480(v=vs.85).aspx
    if(msg->message == WM_DEVICECHANGE) {

        // Possbile wParam values -> https://msdn.microsoft.com/en-us/library/aa363480(v=vs.85).aspx
        if(msg->wParam == DBT_DEVICEARRIVAL) {

            // cast to structure header as described @ https://msdn.microsoft.com/en-us/library/aa363208(v=vs.85).aspx
            // recast the lParam based on device type https://msdn.microsoft.com/en-us/library/aa363246(v=vs.85).aspx
            // we only really care about the device interfaces (Ports might also be interesting, for COM devices)
            if(reinterpret_cast<DEV_BROADCAST_HDR*>(msg->lParam)->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
                dbcc_name_usb newDev;
                if(getDevData(msg->lParam, newDev)) {
                    emit this->USBConnected(newDev);
                }
            }
            else if(reinterpret_cast<DEV_BROADCAST_HDR*>(msg->lParam)->dbch_devicetype == DBT_DEVTYP_PORT) {
                DEV_BROADCAST_PORT* port = reinterpret_cast<DEV_BROADCAST_PORT*>(msg->lParam);
                emit this->PortConnected(getQstring(port->dbcp_name));
            }
        }
        else if(msg->wParam == DBT_DEVICEREMOVECOMPLETE) {

            if(reinterpret_cast<DEV_BROADCAST_HDR*>(msg->lParam)->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
                dbcc_name_usb newDev;
                if(getDevData(msg->lParam, newDev)) {
                    emit this->USBDisconnected(newDev);
                }
            }
            else if(reinterpret_cast<DEV_BROADCAST_HDR*>(msg->lParam)->dbch_devicetype == DBT_DEVTYP_PORT) {
                DEV_BROADCAST_PORT* port = reinterpret_cast<DEV_BROADCAST_PORT*>(msg->lParam);
                emit this->PortDisconnected(getQstring(port->dbcp_name));
            }
        }
    }

    // return false to let Qt handle the rest
    return false;
}

bool qUSBListener::getDevData(LPARAM lParamDev, dbcc_name_usb &newDevice) {
    bool success = false;       // return value
    DEV_BROADCAST_DEVICEINTERFACE* deviceStruct = reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE*>(lParamDev);

    // split windows devicename along #. should give us:
    //      0) Device Type (USB we hope)
    //      1) VID, PID, etc...
    //      2) Serial Number String
    //      3) Class GUID as a string
    // ex: \\?\USB#VID_03EB&PID_2111#ATML1802021800002211#{a5dcbf10-6530-11d2-901f-00c04fb951ed}
    QStringList substr = getQstring(deviceStruct->dbcc_name).split("#");

    // Only deal with USB devices, other things could pop up?
    if(substr.at(0).contains("USB")) {
        // Get serial number & GUID
        newDevice.serialNum  = substr.at(2);
        //newDevice.class_guid = deviceStruct->dbcc_classguid;

        // Get VID and PID
        int vid_idx = substr.at(1).indexOf(QRegularExpression("VID_[\\da-fA-F]{4}"));
        int pid_idx = substr.at(1).indexOf(QRegularExpression("PID_[\\da-fA-F]{4}"));

        if(vid_idx >= 0) {
            newDevice.VID = substr.at(1).mid(vid_idx+4, 4).toUShort(&success, 16);
        }
        if(pid_idx >= 0 && success) {
            newDevice.PID = substr.at(1).mid(pid_idx+4, 4).toUShort(&success, 16);
        }

        if(targetDev != NULL) {
            success = (newDevice == *targetDev);
        }
    }
    return success;
}

inline QString qUSBListener::getQstring(void* stringPtr) {
#ifdef UNICODE
    return QString::fromStdWString((LPCTSTR)stringPtr);
#else
    return QString::fromStdString((std::string)stringPtr);  // ?? not sure if correct
#endif
}
