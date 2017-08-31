#include "usbdevice.h"

usbDevice::usbDevice(uint16_t vid, uint16_t pid, QString sn) {
    VID       = vid;
    PID       = pid;
    serialNum = sn;
}

bool usbDevice::operator==(const usbDevice &other) const {
    bool vidMatch = other.VID == 0 || this->VID == other.VID;
    bool pidMatch = other.PID == 0 || this->PID == other.PID;
    bool snMatch  = other.serialNum.isEmpty() || this->serialNum == other.serialNum;
    return vidMatch && pidMatch && snMatch;
}

bool usbDevice::operator!=(const usbDevice &other) const {
    return !(*this == other);
}

usbDevice::operator QString() const {
    return QString("0x%1 : 0x%2 : %3").arg(VID, 4, 16, QLatin1Char('0')).arg(PID, 4, 16, QLatin1Char('0')).arg(serialNum);
}
