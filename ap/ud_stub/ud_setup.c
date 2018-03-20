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
#include "ud_setup.h"
#include "ud_intern.h"

const device_descriptor_t Device_Descriptor =
{
    sizeof(device_descriptor_t), //size of descriptor
    DEVICE,     //descriptor type
    0x0200,     //USB release version 2.0
    0x00,       //class code
    0x00,       //sub-class code
    0x00,       //protocol code
    0x40,       //max packet size = 64byte
    0x10d6,     //vendor ID
    0xff00,     //product ID
    0x0100,     //device release number
    0x00,       //index of string descriptor of manufacturer
    0x00,       //index of string descriptor of product
    0x00,       //index of string descriptor of serial number
    0x01,       //number of possible configuration
};

//Qualifier descriptor
const usb_qualifier_descriptor device_qualifier_descriptor = 
{
    sizeof(usb_qualifier_descriptor),
    DEVICE_QUALIFIER,
    0x0200,
    0xff,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
};

//-----------------------------------------------------------
//start of Configuration
//-----------------------------------------------------------
device_msc_configuration_t  high_speed_configuration =
{

    /*device_config_descriptor_t Configuration_Descriptor =*/
    {
        sizeof(device_config_descriptor_t), //size of descriptor
        CONFIG,     //descriptor type
        sizeof(device_msc_configuration_t),  //total length
        0x01,       //number of interface
        0x01,       //configuration value
        0x00,       //configuration string index
        0xC0,       //attribute (bus powered, remote wakeup disable)
        0x96,       //max power (500mA),96h(300mA)
    },

    /*usb_interface_descriptor_t Interface0_Descriptor =*/
    {
        sizeof(usb_interface_descriptor_t), //size of descriptor
        INTERFACE,  //descriptor type
        0x00,       //interface number,
        0x00,       //alternative setting
        0x02,       //number of endpoint
        0xff,       //interface class code
        0xff,       //interface sub-class code
        0xff,       //interface protocol code.
        0x00,       //interface string index, doesn't have a string descriptor describing this iInterface.
    },

    /*Standard_Endpiont_Descriptor1_t Endpiont_Descriptor_Setting2_bk_in =*/
    {
        sizeof(Standard_Endpiont_Descriptor_t), //size of descriptor
        ENDPOINT,   //descriptor type
        0x81,       //bEndpointAddress:-> Direction: in - EndpointID: 1
        0x02,       //bmAttributes:-> Bulk Transfer Type
        0x0200,     //wMaxPacketSize
        0x00,       //bInterval
    },

    /*Standard_Endpiont_Descriptor1_t Endpiont_Descriptor_Setting2_bk_out =*/
    {
        sizeof(Standard_Endpiont_Descriptor_t), //size of descriptor
        ENDPOINT,   //descriptor type
        0x02,       //bEndpointAddress:-> Direction: out - EndpointID: 2
        0x02,       //bmAttributes:-> Bulk Transfer Type
        0x0200,     //wMaxPacketSize
        0x00,       //bInterval
    },
};

device_msc_configuration_t  other_speed_configuration =
{

    /*device_config_descriptor_t Configuration_Descriptor =*/
    {
        sizeof(device_config_descriptor_t), //size of descriptor
        OTHER_SPEED_CONFIGURATION,     //descriptor type
        sizeof(device_msc_configuration_t),  //total length
        0x01,       //number of interface
        0x01,       //configuration value
        0x00,       //configuration string index
        0xC0,       //attribute (bus powered, remote wakeup disable)
        0x96,       //max power (500mA),96h(300mA)
    },

    /*usb_interface_descriptor_t Interface0_Descriptor =*/
    {
        sizeof(usb_interface_descriptor_t), //size of descriptor
        INTERFACE,  //descriptor type
        0x00,       //interface number,
        0x00,       //alternative setting
        0x02,       //number of endpoint
        0xff,       //interface class code.
        0xff,       //interface sub-class code.
        0xff,       //interface protocol code.
        0x00,       //interface string index, doesn't have a string descriptor describing this iInterface.
    },

    /*Standard_Endpiont_Descriptor1_t Endpiont_Descriptor_Setting2_bk_in =*/
    {
        sizeof(Standard_Endpiont_Descriptor_t), //size of descriptor
        ENDPOINT,   //descriptor type
        0x81,       //bEndpointAddress:-> Direction: in - EndpointID: 1
        0x02,       //bmAttributes:-> Bulk Transfer Type
        0x0040,     //wMaxPacketSize
        0x00,       //bInterval
    },

    /*Standard_Endpiont_Descriptor1_t Endpiont_Descriptor_Setting2_bk_out =*/
    {
        sizeof(Standard_Endpiont_Descriptor_t), //size of descriptor
        ENDPOINT,   //descriptor type
        0x02,       //bEndpointAddress:-> Direction: out - EndpointID: 2
        0x02,       //bmAttributes:-> Bulk Transfer Type
        0x0040,     //wMaxPacketSize
        0x00,       //bInterval
    },
};

//-----------------------------------------------------------
//end of Configuration
//-----------------------------------------------------------

const string_language_id_t language_string =
{
    0x04,       /* length*/
    0x03,       /* Second Byte of this descriptor.  */
    0x0409,     /* LANGID code array, US english,0x409 */
};


/******************************************************************************/
/*!
 * \par  Description:
 *   deal endpoint0 data.
 * \param[in]   none
 * \return  none
 * \retval  none
 * \ingroup ud_setup.c
 * \par
 * \note
 *******************************************************************************/
void deal_setup(void)
{
    //uint8 *p_temp;

    device_requests_t setup_data;
    
    //p_temp = &setup_data.bmRequestType;

    /* get setup data.*/
    setup_data.bmRequestType = *(volatile uint8 *) (Setupdat0);
    setup_data.bRequest = *(volatile uint8 *) (Setupdat1);
    setup_data.wValue = *(volatile uint16 *) (Setupdat2);
    setup_data.wIndex = *(volatile uint16 *) (Setupdat4);
    setup_data.wLength = *(volatile uint16 *) (Setupdat6);

    /* judge type of device request.   */
    switch (setup_data.bmRequestType)
    {
        case RD_STD_DEV:
        set_usb_descriptor(&high_speed_configuration, &other_speed_configuration);
        if (setup_data.bRequest == GET_DESCRIPTOR)
        {
            /* get device descriptor. */
            if (setup_data.wValue == VALUE_DEVICE)
            {
                control_read(&Device_Descriptor.bLength, Device_Descriptor.bLength, (uint8) setup_data.wLength);
            }

            /* get device configuration. */
            else if (setup_data.wValue == VALUE_CONFIG)
            {
                //prints("config\n");
                control_read((uint8*) &high_speed_configuration,
                        (uint8) (high_speed_configuration.Configuration_Descriptor.wTotalLength),
                        (uint8) setup_data.wLength);
            }
            else if (setup_data.wValue == VALUE_QUALIFIER)
            {
                control_read((uint8 *) &device_qualifier_descriptor, 0x0a, (uint8) setup_data.wLength);
            }
            else if (setup_data.wValue == VALUE_OTHER_SPEED)
            {
                control_read((uint8*) &other_speed_configuration,
                        (uint8) (other_speed_configuration.Configuration_Descriptor.wTotalLength),
                        (uint8) setup_data.wLength);
            }
            /* get string of language ID. */
            else if (setup_data.wValue == VALUE_STRING_LANGID)
            {
                control_read(&language_string.bLength, language_string.bLength, (uint8) setup_data.wLength);
            }
            /* others error.  */
            else
            {
                ;//do nothing
            }
        }
        else if (setup_data.bRequest == GET_CONFIGURATION)
        {
            control_read((uint8 *) (&config_value), 0x01, (uint8) setup_data.wLength);
        }
        else if (setup_data.bRequest == GET_STATUS)
        {
            control_read((uint8 *) (&device_status), 0x02, (uint8) setup_data.wLength);
        }
        else
        {
            stall_endpoint(EP0);
        }

        break;

        case RD_STD_IF:
        /* get Descriptor. */
        if (setup_data.bRequest == GET_STATUS)
        {
            if (setup_data.wIndex == 0)
            {
                control_read((uint8 *) (&interface_status), 0x02, (uint8) setup_data.wLength);
            }
        }
        else if (setup_data.bRequest == GET_INTERFACE)
        {
            if (setup_data.wIndex == 0)
            {
                control_read((uint8 *) (&alter_setting), 0x01, (uint8) setup_data.wLength);
            }
        }
        else
        {
            ;//do nothing
        }
        break;

        case RD_STD_EP:
        if (setup_data.bRequest == GET_STATUS)
        {
            if (setup_data.wIndex == 0x0081)
            {
                control_read((uint8 *) (&ep1_in_status), 0x02, (uint8) setup_data.wLength);
            }
            else if (setup_data.wIndex == 0x0001)
            {
                control_read((uint8 *) (&ep1_out_status), 0x02, (uint8) setup_data.wLength);
            }
            else
            {
                stall_endpoint(EP0);
            }
        }
        else
        {
            stall_endpoint(EP0);
        }
        break;

        case WR_STD_DEV:
        /* set address. */
        if (setup_data.bRequest == SET_ADDRESS)
        {
            no_data_control();
        }

        /* set configuration. */
        else if (setup_data.bRequest == SET_CONFIGURATION)
        {
            set_bulk_in_ep(g_mem_id);
            set_bulk_out_ep(g_mem_id);
            no_data_control();
            //usb_enum_finish = 1;
            config_value = (uint8) setup_data.wValue;
        }
        else if (setup_data.bRequest == SET_FEATURE)
        {
            //一般在PC机对msc不会有set_feature命令,但test mode特性的设定是由usb逻辑分析仪的掩模工具发送的,此时应该处理
            if (setup_data.wValue == TEST_MODE)
            {
                no_data_control();
            }
            else
            {
                stall_endpoint(EP0);
            }
        }
        else
        {
            stall_endpoint(EP0);
        }
        break;

        case WR_STD_IF:
        /* set interface. */
        if ((setup_data.bRequest == SET_FEATURE) && (setup_data.wValue == 0) && (setup_data.wIndex == 0))
        {
            no_data_control();
        }
        else
        {
            stall_endpoint(EP0);
        }
        break;

        case WR_STD_EP:
        if ((setup_data.bRequest == SET_FEATURE) && (setup_data.wValue == EP_HALT) && (setup_data.wIndex == 0x0081))
        {
            ep1_out_status = 1;
            stall_endpoint(EP1OUT);
            no_data_control();
        }
        else if (((setup_data.bRequest == SET_FEATURE) && (setup_data.wValue == EP_HALT) && (setup_data.wIndex
                == 0x0002)))
        {
            ep1_in_status = 1;
            stall_endpoint(EP1IN);
            no_data_control();
        }
        else
        {
            ;//do nothing
        }

        if ((setup_data.bRequest == CLEAR_FEATURE) && (setup_data.wValue == EP_HALT) && (setup_data.wIndex == 0x0081))
        {
            ep1_out_status = 0;
            recover_stall_endpoint(EP1OUT);
            no_data_control();
        }
        else if (((setup_data.bRequest == CLEAR_FEATURE) && (setup_data.wValue == EP_HALT) && (setup_data.wIndex
                == 0x0002)))
        {
            ep1_out_status = 0;
            recover_stall_endpoint(EP1IN);
            no_data_control();
        }
        else
        {
            stall_endpoint(EP0);
        }
        break;

        //        case SPECIFIC_REQUEST_OUT:
        //            /* set feature unit. */
        //            if (setup_data.bRequest == MASS_STOR_RSRET)
        //            {
        //                rest_endp_toggle();
        //            }
        //            no_data_control();
        //            break;
        //
        //        case SPECIFIC_REQUEST_IN:
        //            /* get feature unit. */
        //            if (setup_data.bRequest == GET_MAX_LUN) //get max lun
        //            {
        //                control_read((uint8 *) &max_lun, 0x01, setup_data.wLength);
        //            }
        //            break;

        default:
        no_data_control();//可能会发set clearfeature到en2in
        break;
    }

}

