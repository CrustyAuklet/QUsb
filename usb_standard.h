/**
 * @file usb_standard.h
 * @brief Data types and Defines for USB standard
 *
 * This header provides structs, enums, and defines that
 * can be used to meet USB specifications and standards.
 *
 * @author Ethan Slattery
 * @date 25 AUG 2017
 * @copyright MIT
 */

#ifndef USB_STANDARD_h__
#define USB_STANDARD_h__

#include <stdint.h>

//struct usbDev_t {
//    uint16_t VID;
//    uint16_t PID;
//    QString  serialNum;
//    QString  Manufacturer;
//    QString  Product;
//};

typedef enum {
  USB_SPEED_LOW,
  USB_SPEED_FULL,
  USB_SPEED_HIGH,
} USB_Speed;

/******************************
 * DEFINES FOR SETUP PACKETS
 ******************************/
// Masks for bit-fields of the bmRequestType
#define USB_REQTYPE_DIRECTION_MASK   (0x80)
#define USB_REQTYPE_TYPE_MASK        (0x60)
#define USB_REQTYPE_RECIPIENT_MASK   (0x1F)

/**
 * @brief USB request types and recipient codes (bmRequestType)
 */
#define USB_REQTYPE_STANDARD        (0 << 5) //!< Standard request
#define USB_REQTYPE_CLASS           (1 << 5) //!< Class-specific request
#define USB_REQTYPE_VENDOR          (2 << 5) //!< Vendor-specific request
#define USB_REQ_TYPE_MASK           (3 << 5) //!< Mask

#define USB_RECIPIENT_DEVICE        (0 << 0) //!< Recipient device
#define USB_RECIPIENT_INTERFACE     (1 << 0) //!< Recipient interface
#define USB_RECIPIENT_ENDPOINT      (2 << 0) //!< Recipient endpoint
#define USB_RECIPIENT_OTHER         (3 << 0) //!< Recipient other
#define USB_REQ_RECIP_MASK          ( 0x1F ) //!< Mask

/*********************************************
 * DEFINES FOR MICROSOFT SETUP WCID PACKETS
 *********************************************/
#define STRINGDSC_MICROSOFT_OS          (0xEE)
#define EXTENDED_COMPAT_ID_DESCRIPTOR   (0x0004)
#define EXTENDED_PROPERTIES_DESCRIPTOR  (0x0005)
#define MS_WCID_WINSUB                  "WINUSB\0\0"
#define MS_WCID_LIBUSB                  "LIBUSB0\0"
#define MS_WCID_LIBUSBK                 "LIBUSBK\0"

/**
 * @brief USB request data transfer direction (bmRequestType)
 */
#define  USB_IN             (0x80) //!< Device to host
#define  USB_OUT            (0x00) //!< Host to Device
#define  USB_DIR_MASK       (1<<7) //!< Mask

/**
 * @brief Standard USB requests (bRequest)
 */
typedef enum {
  USB_REQ_GetStatus        = 0x00,  // target is device, interface, or endpoint
  USB_REQ_ClearFeature     = 0x01,  // target is device, interface, or endpoint
  USB_REQ_SetFeature       = 0x03,  // target is device, interface, or endpoint
  USB_REQ_SetAddress       = 0x05,	// Host setting device address.
  USB_REQ_GetDescriptor    = 0x06,	// usually first received. Host getting info for device
  USB_REQ_SetDescriptor    = 0x07,  // target is device
  USB_REQ_GetConfiguration = 0x08,  // target is device
  USB_REQ_SetConfiguration = 0x09,  // target is device
  USB_REQ_GetInterface     = 0x0A,  // target is interface
  USB_REQ_SetInterface     = 0x0B,  // target is interface
  USB_REQ_SynchFrame       = 0x0C,  // target is endpoint
  USB_REQ_SetSel           = 0x30,  // target is device (isoc only)
  USB_REQ_SetIsocDelay     = 0x31   // target is device (isoc only)
} bRequest_t;

/**
 * @brief Standard USB device feature flags (wValue in SET/CLEAR feature)
 * @note valid for SetFeature request.
 */
enum usb_device_feature {
    USB_DEV_FEATURE_REMOTE_WAKEUP         = 1,  //!< Remote wakeup enabled
    USB_DEV_FEATURE_TEST_MODE             = 2,  //!< USB test mode
	USB_DEV_FEATURE_OTG_B_HNP_ENABLE      = 3,
	USB_DEV_FEATURE_OTG_A_HNP_SUPPORT     = 4,
	USB_DEV_FEATURE_OTG_A_ALT_HNP_SUPPORT = 5
};

/**
 * @brief Standard USB endpoint feature/status flags
 */
enum usb_endpoint_feature {
	USB_EP_FEATURE_HALT = 0,
};

/**
 * @brief  Descriptor Type  - used in wValue of get descitor or bDescriptorType in Descriptor Header
 */
typedef enum {
  USB_DTYPE_Device               = 0x01,  // First descriptor needed to get packet length
  USB_DTYPE_Configuration        = 0x02,  // gets descriptor of one config (specified by index in wValue)
  USB_DTYPE_String               = 0x03,  // Gets a manf/priduct/ser# string
  USB_DTYPE_Interface            = 0x04,
  USB_DTYPE_Endpoint             = 0x05,
  USB_DTYPE_DeviceQualifier      = 0x06,
  USB_DTYPE_OtherSpeedConfig     = 0x07,
  USB_DTYPE_InterfacePower       = 0x08,
  USB_DTYPE_InterfaceAssociation = 0x0B,
  USB_DTYPE_CSInterface          = 0x24,
  USB_DTYPE_CSEndpoint           = 0x25,
} USB_dtype;

/**
 * @brief Standard USB Device class codes
 */
typedef enum {
  USB_NoDeviceClass = 0x00,
  USB_NoDeviceSubclass = 0x00,
  USB_NoDeviceProtocol = 0x00,
  USB_VendorSpecificClass = 0xFF,
  USB_VendorSpecificSubclass = 0xFF,
  USB_VendorSpecificProtocol = 0xFF,
  USB_IADDeviceClass = 0xEF,
  USB_IADDeviceSubclass = 0x02,
  USB_IADDeviceProtocol = 0x01,
} USB_cscp;

/**
 * @brief Standard USB endpoint transfer types
 */
enum usb_ep_type {
	USB_EP_TYPE_CONTROL     = 0x00,
	USB_EP_TYPE_ISOCHRONOUS = 0x01,
	USB_EP_TYPE_BULK        = 0x02,
	USB_EP_TYPE_INTERRUPT  = 0x03,
	USB_EP_TYPE_MASK       = 0x03,
};

/*** ENUM to describe the state of the USB device, which effects which control requests it should respond to ***/
typedef enum
{
    USB_Attached,   /* device is attached to the bus but not powered ( ex. host detected over current, also used to indicate unattached state) */
	USB_Powered,    /* device is connected to a host, but enumeration has not begun. VBUS is active and device can draw up to 100mA. */
	USB_Default,    /* device's USB bus has been reset by the host and is waiting for the host to begin the enumeration process */
	USB_Addressed,  /* device has been addressed by the USB Host, but is not configured. */
	USB_Configured, /* device has been enumerated by the host and is ready for USB communications to begin */
	USB_Suspended,  /* USB bus has been suspended by the host, and the device should power down to a minimal power level until the bus is resumed. */
} USB_Device_States;

/**
 * @brief Standard USB language IDs for string descriptors
 */
enum usb_langid {
	USB_LANGUAGE_EN_US = 0x0409, // English (United States)
    USB_LANGUAGE_DE    = 0x0407  // German Standard
};

/**
 * @brief USB standard version for device descriptor
 */
enum usb_bcd_version {
	USB_BCD_VER_11          = 0x0110,
    USB_BCD_VER_20          = 0x0200,
    /* BOS descriptor required for any version above 2.0! */
    USB_BCD_VER_21          = 0x0210,
    USB_BCD_VER_30          = 0x0300,
    USB_BCD_VER_31          = 0x0310,
};

#define USB_CONFIG_BUS_MA(mA)            (((mA) + 1) / 2) //!< Max power in mA
#define USB_STRING_LENGTH(c)             (sizeof(USB_DescriptorHeader) + ((c) * 2))

#define  USB_CONFIG_NO_STRING            (0x00)   // no string descriptor defined
#define  USB_CONFIG_ATTR_MUST_SET        (1 << 7) //!< Must always be set
#define  USB_CONFIG_ATTR_BUS_POWERED     (0 << 6) //!< Bus-powered
#define  USB_CONFIG_ATTR_SELF_POWERED    (1 << 6) //!< Self-powered
#define  USB_CONFIG_ATTR_REMOTE_WAKEUP   (1 << 5) //!< remote wakeup supported

#define ENDPOINT_DESCRIPTOR_DIR_IN       (0x80)
#define ENDPOINT_DESCRIPTOR_DIR_OUT      (0x00)

#define ENDPOINT_ATTR_NO_SYNC            (0 << 2)
#define ENDPOINT_ATTR_ASYNC              (1 << 2)
#define ENDPOINT_ATTR_ADAPTIVE           (2 << 2)
#define ENDPOINT_ATTR_SYNC               (3 << 2)

#define ENDPOINT_USAGE_DATA              (0 << 4)
#define ENDPOINT_USAGE_FEEDBACK          (1 << 4)
#define ENDPOINT_USAGE_IMPLICIT_FEEDBACK (2 << 4)

/******************************************************************/
/*               STRUCTS TO DESCRIBE DESCRIPTORS                  */
/* reference: http://www.beyondlogic.org/usbnutshell/usb5.shtml   */
/******************************************************************/
// struct describing setup packet structure
// ref: http://www.beyondlogic.org/usbnutshell/usb6.shtml
typedef struct  {
    uint8_t bmRequestType;	    // bitmap of fields described above
    uint8_t bRequest;			// Request (on of standard or a custom)
    uint16_t wValue;			// Value
    uint16_t wIndex;			// Index
    uint16_t wLength;			// number of bytes to transfer if there is a data phase
} __attribute__ ((packed)) USB_SetupPacket;

typedef struct {
  uint8_t bLength;			// size of descriptor in bytes
  uint8_t bDescriptorType;	// type of descriptor
} __attribute__ ((packed)) USB_DescriptorHeader;

// Device descriptor that describes entire device and all interfaces (tree structure)
// reference: https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__device__descriptor.html
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint16_t bcdUSB;
  uint8_t bDeviceClass;
  uint8_t bDeviceSubClass;
  uint8_t bDeviceProtocol;
  uint8_t bMaxPacketSize0;
  uint16_t idVendor;
  uint16_t idProduct;
  uint16_t bcdDevice;
  uint8_t iManufacturer;
  uint8_t iProduct;
  uint8_t iSerialNumber;
  uint8_t bNumConfigurations;
} __attribute__ ((packed)) USB_DeviceDescriptor;

// Device Descriptor to decribe device operation at high speed
// reference: https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__device__qualifier__descriptor.html
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint16_t bcdUSB;
  uint8_t bDeviceSubClass;
  uint8_t bMaxPacketSize0;
  uint8_t bNumConfigurations;
  uint8_t bReserved;
} __attribute__ ((packed)) USB_DeviceQualifierDescriptor;

// contains information about the device power requirements and the number of interfaces it can support
// https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__configuration__descriptor.html
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint16_t wTotalLength;
  uint8_t bNumInterfaces;
  uint8_t bConfigurationValue;
  uint8_t iConfiguration;
  uint8_t bmAttributes;
  uint8_t bMaxPower;
} __attribute__ ((packed)) USB_ConfigurationDescriptor;

// defines the collection of endpoints.
// This interface supports a group of pipes that are suitable for a particular task.
// Each configuration can have multiple interfaces.
// reference: https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__interface__descriptor.html
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bInterfaceNumber;
  uint8_t bAlternateSetting;
  uint8_t bNumEndpoints;
  uint8_t bInterfaceClass;
  uint8_t bInterfaceSubClass;
  uint8_t bInterfaceProtocol;
  uint8_t iInterface;
} __attribute__ ((packed)) USB_InterfaceDescriptor;

// USB 2.0 descriptor to group interfaces together (needed for audio/video/BLE applications)
// ref 1 : http://www.usb.org/developers/docs/whitepapers/iadclasscode_r10.pdf
// ref 2 : https://msdn.microsoft.com/en-us/library/windows/hardware/ff540054(v=vs.85).aspx
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bFirstInterface;
  uint8_t bInterfaceCount;
  uint8_t bFunctionClass;
  uint8_t bFunctionSubClass;
  uint8_t bFunctionProtocol;
  uint8_t iFunction;
} __attribute__ ((packed)) USB_InterfaceAssociationDescriptor;

// specifies the transfer type, direction, polling interval, and maximum packet size for each endpoint
// Endpoint 0 (zero), the default endpoint, is always assumed to be a control endpoint and never has a descriptor.
// ref: https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__endpoint__descriptor.html
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bEndpointAddress;
  uint8_t bmAttributes;
  uint16_t wMaxPacketSize;
  uint8_t bInterval;
} __attribute__ ((packed)) USB_EndpointDescriptor;

// optional and add human readable information to the other descriptors.
// If a device does not support string descriptors, all references to string descriptors within device, configuration, and interface descriptors must be set to zero.
// ref: https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__string__descriptor.html
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    int16_t bString[];
} __attribute__ ((packed)) USB_StringDescriptor;

// Microsoft WCID descriptor for automatic driver installation
// Ref: https://github.com/pbatard/libwdi/wiki/WCID-Devices
typedef struct {
  uint8_t bFirstInterfaceNumber;
  uint8_t reserved1;
  uint8_t compatibleID[8];		// "WINUSB\0\0"	or "LIBUSB0\0" or "LIBUSBK\0"
  uint8_t subCompatibleID[8];
  uint8_t reserved2[6];
} __attribute__((packed)) USB_MicrosoftCompatibleDescriptor_Interface;

typedef struct {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    int16_t  bString[7];        // unicode string "MSFT100"
    uint8_t  venderCode;
    uint8_t  reserved;
} __attribute__((packed)) USB_MicrosoftCompatibleDescriptor_String;

typedef struct {
  uint32_t dwLength;
  uint16_t bcdVersion;
  uint16_t wIndex;
  uint8_t bCount;
  uint8_t reserved[7];
  USB_MicrosoftCompatibleDescriptor_Interface interfaces[];
} __attribute__((packed)) USB_MicrosoftCompatibleDescriptor;

// structs used for WCID properties descriptors
typedef struct
{
    uint16_t                               Length;
    int16_t                                Value[];
} USB_OSPropertiesName;

typedef struct
{
    uint32_t                               Length;
    int16_t                                Value[];
} USB_OSPropertiesData;

// typedef struct
// {
//     uint32_t                               Length;
//     uint32_t                               Type;
//     USB_OSPropertiesName                   Name;
//     USB_OSPropertiesData                   Data;
// } USB_OSPropertiesSection;
// 
// typedef struct
// {
//     uint32_t                               TotalLength;
//     uint16_t                               Version;
//     uint16_t                               Index;
//     uint16_t                               TotalSections;
// } USB_OSPropertiesHeader;
// 
// typedef struct
// {
//     USB_OSPropertiesHeader               Header;
//     USB_OSPropertiesSection              Property[];
// } USB_OSPropertiesDescripto

#endif /* USB_STANDARD_h__ */
