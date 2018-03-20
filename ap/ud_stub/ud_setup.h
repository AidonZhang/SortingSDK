/********************************************************************************
 *                              US280A
 *                            Module: USB device
 *                 Copyright(c) 2001-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>            <version >             <desc>
 *      dengtaiping 2013-01-18 16:40     1.0                build this file
 ********************************************************************************/
#ifndef __USOUND_H__
#define __USOUND_H__

#include "typeext.h"
#include "attr.h"

/* bRequest */
#define GET_STATUS                     0x00
#define CLEAR_FEATURE                  0x01
#define SET_FEATURE                    0x03
#define SET_ADDRESS                    0x05
#define GET_DESCRIPTOR                 0x06
#define SET_DSCRPTOR                   0x07
#define GET_CONFIGURATION              0x08
#define SET_CONFIGURATION              0x09
#define GET_INTERFACE                  0x0a
#define SET_INTERFACE                  0x0b

//standard feature
#define EP_HALT                        0x00
#define DEV_RMT_WAKE                   0x01
#define TEST_MODE                      0x02

/* Audio class-specific request codes.  */
#define REQUEST_CODE_UNDEFINED         0x00
#define SET_CUR                        0x01
#define GET_CUR                        0x81
#define SET_MIN                        0x02
#define GET_MIN                        0x82
#define SET_MAX                        0x03
#define GET_MAX                        0x83
#define SET_RES                        0x04
#define GET_RES                        0x84
#define SET_MEM                        0x05
#define GET_MEM                        0x85
#define GET_STAT                       0xFF

/* feature unit control selectors.  */
#define FU_CONTROL_UNDEFINED           0x00
#define MUTE_CONTROL                   0x01
#define VOLUME_CONTROL                 0x02
#define BASS_CONTROL                   0x03
#define MID_CONTROL                    0x04
#define TREBLE_CONTROL                 0x05
#define GRAPHIC_EQUALIZER_CONTROL      0x06
#define AUTOMATIC_GAIN_CONTROL         0x07
#define DELAY_CONTROL                  0x08
#define BASS_BOOST_CONTROL             0x09
#define LOUDNESS_CONTROL               0x0A

#define FEATURE_UNIT_ID1               0x09
#define FEATURE_UNIT_INDEX1            0x0900
#define FEATURE_UNIT_ID2               0x0A
#define FEATURE_UNIT_INDEX2            0x0A00
#define SELECTOR_UNIT_ID               0x0B
#define SELECTOR_UNIT_INDEX            0x0B00

/* channel number.  */
#define MAIN_CHANNEL_NUMBER0           0x00
#define MAIN_CHANNEL_NUMBER1           0x01
#define MAIN_CHANNEL_NUMBER2           0x02

/* volume configuration of max, min, resolution.  */
#define MAXIMUM_VOLUME                 0x0041 //0x7F00
#define MINIMUM_VOLUME                 0x0000
#define RESOTION_VOLUME                0x0100
#define MUTE_LENGTH                    0x01
#define VOLUME_LENGTH                  0x02
#define SAMPLE_FREQUENCY_LENGTH        0x03

/* wValue */
#define VALUE_DEVICE                   0x0100
#define VALUE_CONFIG                   0x0200
#define VALUE_STRING                   0x0300
#define VALUE_QUALIFIER                0x0600
#define VALUE_OTHER_SPEED              0x0700

#define VALUE_STRING_LANGID            0x0300
#define VALUE_STRING_MANUFACTURER      0x0301
#define VALUE_STRING_PRODUCT           0x0302
#define VALUE_STRING_SERIAL_NUMBER     0x0303

/* descriptor type */
#define DEVICE                         0x01
#define CONFIG                         0x02
#define STRING                         0x03
#define INTERFACE                      0x04
#define ENDPOINT                       0x05
#define DEVICE_QUALIFIER               0x06
#define OTHER_SPEED_CONFIGURATION      0x07

#define VALUE_ADDRESS                  0x0001
#define VALUE_SET_CONFIGURATION        0x0001
#define VALUE_ALTERNATE_SETTING0       0x0000
#define VALUE_ALTERNATE_SETTING1       0x0001
#define VALUE_ALTERNATE_SETTING2       0x0002
#define VALUE_ALTERNATE_SETTING3       0x0003
#define INDEX_INTERFACE0               0x0000
#define INDEX_INTERFACE1               0x0001
#define INDEX_INTERFACE2               0x0002
#define INDEX_INTERFACE3               0x0003

/* bRequest */
#define RD_STD_DEV                     0x80
#define RD_STD_IF                      0x81
#define RD_STD_EP                      0x82

#define WR_STD_DEV                     0x00
#define WR_STD_IF                      0x01
#define WR_STD_EP                      0x02

#define SPECIFIC_REQUEST_OUT           0x21
#define SPECIFIC_REQUEST_IN            0xA1
#define SET_ENDPOINT_CONTROL           0x22
#define SAMPLING_FREQ_CONTROL          0x0100

#define MASS_STOR_RSRET                0xff
#define GET_MAX_LUN                    0xfe

#define EP0_MAX_PACKET_SIZE            0x40
#define ISO_MAX_PACKET_SIZE            0xC8
#define MIC_ISO_MAX_PACKET_SIZE        0x16//0x2C
#define SAMPLE_BYTE                    0x02
#define CHANNEL_NUMBER                 0x02

#pragma pack(1)
/* SETUP data for a USB device control request  */
typedef struct
{
    uint8 bmRequestType;
    uint8 bRequest;
    uint16 wValue;
    uint16 wIndex;
    uint16 wLength;
} device_requests_t;

/* descriptor of device */
typedef struct
{
    uint8 bLength;
    uint8 bDescriptorType;

    uint16 bcdUSB;
    uint8 bDeviceClass;
    uint8 bDeviceSubClass;
    uint8 bDeviceProtocol;
    uint8 bMaxPacketSize0;
    uint16 idVendor;
    uint16 idProduct;
    uint16 bcdDevice;
    uint8 iManufacturer;
    uint8 iProduct;
    uint8 iSerialNumber;
    uint8 bNumConfigurations;
} device_descriptor_t;

typedef struct
{
    unsigned char bLength;
    uint8 bDescriptorType;

    uint16 bcdUSB;
    uint8 bDeviceClass;
    uint8 bDeviceSubClass;
    uint8 bDeviceProtocol;
    uint8 bMaxPacketSize0;
    uint8 bNumConfigurations;
    uint8 bRESERVED;
} usb_qualifier_descriptor;

/* descriptor of configuration */
typedef struct
{
    uint8 bLength;
    uint8 bDescriptorType;
    uint16 wTotalLength;
    uint8 bNumInterfaces;
    uint8 bConfigurationValue;
    uint8 iConfiguration;
    uint8 bmAttributes;
    uint8 bMaxPower;
} device_config_descriptor_t;

/* Interface descriptor */
typedef struct
{
    uint8 bLength;
    uint8 bDescriptorType;

    uint8 bInterfaceNumber;
    uint8 bAlternateSetting;
    uint8 bNumEndpoints;
    uint8 bInterfaceClass;
    uint8 bInterfaceSubClass;
    uint8 bInterfaceProtocol;
    uint8 iInterface;
} usb_interface_descriptor_t;

typedef struct
{
    uint8 bLength; //Size of this descriptor, in bytes: 9
    uint8 bDescriptorType; //ENDPOINT descriptor type
    uint8 bEndpointAddress; /*Endpoint The address of the endpoint on the USB device described by this descriptor.
     The address is encoded as follows:
     D7: Direction.0 = OUT endpoint 1 = IN endpoint
     D6..4: Reserved, reset to zero
     D3..0: The endpoint number,determined by the designer. */
    uint8 bmAttributes; /*Bit Map
     D3..2: Synchronization type 01 = Asynchronous 10 = Adaptive 11 = Synchronous
     D1..0: Transfer type 01 = Isochronous
     All other bits are reserved.*/
    uint16 wMaxPacketSize; /*Number Maximum packet size this endpoint is capable of sending or receiving
     when this configuration is selected.
     This is determined by the audio bandwidth constraints of the endpoint.*/
    uint8 bInterval; /*Interval for polling endpoint for data transfers expressed in milliseconds.
     Must be set to 1.*/
} Standard_Endpiont_Descriptor_t;

/* string descriptor */
typedef struct
{
    uint8 bLength;
    uint8 bDescriptorType;
    uint16 wLanguageID;
} string_language_id_t;

typedef struct
{
    uint8 bLength;
    uint8 bDescriptorType;
    uint8 wProductName[32];
} string_product_t;

typedef struct
{
    uint8 bLength;
    uint8 bDescriptorType;
    uint8 wManufacturerName[16];
} string_manufacturer_t;

typedef struct
{
    uint8 bLength;
    uint8 bDescriptorType;
    uint8 wManufacturerName[32];
} serialnum_descriptor_t;

/*dma*/
typedef struct
{
    uint8 *source_address;
    uint8 *destination_address;
    uint16 data_length;
} data_information_for_move_t;

typedef struct
{
    device_config_descriptor_t Configuration_Descriptor;
    usb_interface_descriptor_t Interface0_Descriptor;
    Standard_Endpiont_Descriptor_t Endpiont_Descriptor_Setting_bk_in;
    Standard_Endpiont_Descriptor_t Endpiont_Descriptor_Setting_bk_out;

} device_msc_configuration_t;

#pragma pack() //±£´æ¶ÔÆë×´Ì¬
extern void no_data_control(void);
extern void control_write(uint8* data_address, uint8 data_length, uint8 request_length);
extern void control_read(uint8 *d_addr, uint8 actul_dlen, uint8 rqst_dlen);
extern void stall_endpoint(uint8 ep_num);
extern void recover_stall_endpoint(uint8 ep_num);
extern void rest_endp_toggle(void);
extern void set_usb_descriptor(device_msc_configuration_t* hs_cfg_p, device_msc_configuration_t* others_cfg_p);

void set_bulk_in_ep(uint32 mem_sel);
extern void set_bulk_out_ep(uint32 mem_sel);
extern void reset_in_fifo(uint32 mem_sel);
extern void reset_out_fifo(uint32 mem_sel);

//extern void set_usb_descriptor(void);

extern void deal_setup(void) __FAR__;

#endif

