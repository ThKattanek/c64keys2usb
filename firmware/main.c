/* Name: main.c
 * Project: c64keys2usb
 * Author: Thorsten Kattanek
 * Creation Date: 2013-07-30
 * Tabsize: 4
 * Copyright: (c) 2013 by ALFSOFT
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

/* Pinbelegung Atmega in PDIP
 * PIN 02 - PD0 = LED        A
 * PIN 03 - PD1 = RESTORE    E
 * PIN 23 - PC0 = Bit 0 Matrix A
 * PIN 24 - PC1 = Bit 1 Matrix A
 * PIN 25 - PC2 = Bit 2 Matrix A
 * PIN 26 - PC3 = Bit 3 Matrix A
 * PIN 06 - PD4 = Bit 4 Matrix A
 * PIN 11 - PD5 = Bit 5 Matrix A
 * PIN 12 - PD6 = Bit 6 Matrix A
 * PIN 13 - PC7 = Bit 7 Matrix A
 * PIN 14 - PB0 = Bit 0 Matrix E
 * PIN 15 - PB1 = Bit 1 Matrix E
 * PIN 16 - PB2 = Bit 2 Matrix E
 * PIN 17 - PB3 = Bit 3 Matrix E
 * PIN 18 - PB4 = Bit 4 Matrix E
 * PIN 19 - PB5 = Bit 5 Matrix E
 * PIN 12 - PC4 = Bit 6 Matrix E
 * PIN 13 - PC5 = Bit 7 Matrix E
*/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/eeprom.h>

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

PROGMEM const char usbHidReportDescriptor[22] = {    /* USB report descriptor */
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x80,                    //   REPORT_COUNT (128)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};
/* Since we define only one feature report, we don't use report-IDs (which
 * would be the first byte of the report). The entire report consists of 128
 * opaque data bytes.
 */

/* The following variables store the status of the current data transfer */
static uchar    currentAddress;
static uchar    bytesRemaining;

/* ------------------------------------------------------------------------- */

/* usbFunctionRead() wird aufgerufen wenn der Host Daten Empfangen möchte */
uchar   usbFunctionRead(uchar *data, uchar len)
{
    if(len > bytesRemaining)
        len = bytesRemaining;
    eeprom_read_block(data, (uchar *)0 + currentAddress, len);
    currentAddress += len;
    bytesRemaining -= len;
    return len;
}

/* usbFunctionWrite() wird aufgerufen wenn der Host Daten Senden möchte */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
    if(bytesRemaining == 0)
        return 1;               /* end of transfer */
    if(len > bytesRemaining)
        len = bytesRemaining;
    eeprom_write_block(data, (uchar *)0 + currentAddress, len);
    currentAddress += len;
    bytesRemaining -= len;
    return bytesRemaining == 0; /* return 1 if this was the last chunk */
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID class request */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* since we have only one report type, we can ignore the report-ID */
            bytesRemaining = 128;
            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionRead() to obtain data */
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
            /* since we have only one report type, we can ignore the report-ID */
            bytesRemaining = 128;
            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionWrite() to receive data from host */
        }
    }else{
        /* ignore vendor type requests, we don't use any */
    }
    return 0;
}

/* ------------------------------------------------------------------------- */

int main(void)
{

uchar   i;

    wdt_enable(WDTO_1S);
    usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    i = 0;
    while(--i){             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();
    sei();
    for(;;){                /* main event loop */
        wdt_reset();
        usbPoll();
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
