#include "qUSBListener.h"

qUSBListener::qUSBListener() : QMainWindow()
{
}

bool qUSBListener::Start()
{
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

    HDEVNOTIFY devNotify = RegisterDeviceNotification((HANDLE)this->winId(),
                                                      &notificationFilter,
                                                      DEVICE_NOTIFY_WINDOW_HANDLE);

    return devNotify != NULL;
}

bool qUSBListener::nativeEvent(const QByteArray & eventType,
                                    void * message,
                                    long * result)
{
    Q_UNUSED(result);
    Q_UNUSED(eventType);

    // MSG structure      -> https://msdn.microsoft.com/en-us/library/windows/desktop/ms644958(v=vs.85).aspx
    // list of msg values -> https://wiki.winehq.org/List_Of_Windows_Messages
    MSG * msg = reinterpret_cast<MSG *>(message);

    // Does this specific message interest us? WM_DEVICE CHANGE -> https://msdn.microsoft.com/en-us/library/aa363480(v=vs.85).aspx
    if(msg->message == WM_DEVICECHANGE) {

        // Possbile wParam values -> https://msdn.microsoft.com/en-us/library/aa363480(v=vs.85).aspx
        if(msg->wParam == DBT_DEVICEARRIVAL) {

            // cast to structure header as described @ https://msdn.microsoft.com/en-us/library/aa363208(v=vs.85).aspx
            // recast the lParam based on device type https://msdn.microsoft.com/en-us/library/aa363246(v=vs.85).aspx
            // we only really care about the device interfaces (POrts might also be interesting, for COM devices
            if(reinterpret_cast<DEV_BROADCAST_HDR*>(msg->lParam)->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
                DEV_BROADCAST_DEVICEINTERFACE* deviceStruct = reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE*>(msg->lParam);
                #ifdef UNICODE
                  emit this->DeviceConnected(QString::fromStdWString(deviceStruct->dbcc_name));
                #else
                  emit this->DeviceConnected(QString::fromStdString(deviceStruct->dbcc_name));
                #endif
            }
            else if(reinterpret_cast<DEV_BROADCAST_HDR*>(msg->lParam)->dbch_devicetype == DBT_DEVTYP_PORT) {
                DEV_BROADCAST_PORT* port = reinterpret_cast<DEV_BROADCAST_PORT*>(msg->lParam);
                #ifdef UNICODE
                  emit this->PortConnected(QString::fromStdWString(port->dbcp_name));
                #else
                  emit this->PortConnected(QString::fromStdString(port->dbcp_name));
                #endif
            }
        }
        else if(msg->wParam == DBT_DEVICEREMOVECOMPLETE) {

            if(reinterpret_cast<DEV_BROADCAST_HDR*>(msg->lParam)->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
                DEV_BROADCAST_DEVICEINTERFACE* deviceStruct = reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE*>(msg->lParam);
                #ifdef UNICODE
                  emit this->DeviceDisconnected(QString::fromStdWString(deviceStruct->dbcc_name));
                #else
                  emit this->DeviceDisconnected(QString::fromStdString(deviceStruct->dbcc_name));
                #endif
            }
            else if(reinterpret_cast<DEV_BROADCAST_HDR*>(msg->lParam)->dbch_devicetype == DBT_DEVTYP_PORT) {
                DEV_BROADCAST_PORT* port = reinterpret_cast<DEV_BROADCAST_PORT*>(msg->lParam);
                #ifdef UNICODE
                  emit this->PortDisconnected(QString::fromStdWString(port->dbcp_name));
                #else
                  emit this->PortDisconnected(QString::fromStdString(port->dbcp_name));
                #endif
            }
        }
    }

    // Qt handles the rest
    return false;
}

