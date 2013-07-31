/* Name: main.c
 * Project: c64keys2usb
 * Author: Thorsten Kattanek
 * Creation Date: 2013-07-31
 * Tabsize: 4
 * Copyright: (c) 2013 by ALFSOFT
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

/* Pinbelegung Atmega8 in PDIP
 * PIN 02 - PD0 = LED        A
 * PIN 03 - PD1 = RESTORE    E
 * PIN 23 - PC0 = Matrix Col 0 A
 * PIN 24 - PC1 = Matrix Col 1 A
 * PIN 25 - PC2 = Matrix Col 2 A
 * PIN 26 - PC3 = Matrix Col 3 A
 * PIN 06 - PD4 = Matrix Col 4 A
 * PIN 11 - PD5 = Matrix Col 5 A
 * PIN 12 - PD6 = Matrix Col 6 A
 * PIN 13 - PD7 = Matrix Col 7 A
 * PIN 14 - PB0 = Matrix Row 0 E
 * PIN 15 - PB1 = Matrix Row 1 E
 * PIN 16 - PB2 = Matrix Row 2 E
 * PIN 17 - PB3 = Matrix Row 3 E
 * PIN 18 - PB4 = Matrix Row 4 E
 * PIN 19 - PB5 = Matrix Row 5 E
 * PIN 12 - PC4 = Matrix Row 6 E
 * PIN 13 - PC5 = Matrix Row 7 E
*/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/eeprom.h>

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"

uint8_t key_buffer[10];

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
    0x95, 0x0A,                    //   REPORT_COUNT (10)
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
uchar usbFunctionRead(uchar *data, uchar len)
{
    uint8_t i;

    if(len > bytesRemaining)
        len = bytesRemaining;
    //eeprom_read_block(data, (uchar *)0 + currentAddress, len);

    for(i=0;i<len;i++)
    {
        data[i] = key_buffer[i+currentAddress];
    }
    currentAddress += len;
    bytesRemaining -= len;
    return len;
}

/* ------------------------------------------------------------------------- */

/* usbFunctionWrite() wird aufgerufen wenn der Host Daten Senden möchte */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
    /*
    if(bytesRemaining == 0)
        return 1;               // end of transfer
    if(len > bytesRemaining)
        len = bytesRemaining;
    eeprom_write_block(data, (uchar *)0 + currentAddress, len);
    currentAddress += len;
    bytesRemaining -= len;
    return bytesRemaining == 0; / return 1 if this was the last chunk
    */
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID class request */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* since we have only one report type, we can ignore the report-ID */
            bytesRemaining = 10;
            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionRead() to obtain data */
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
            /* since we have only one report type, we can ignore the report-ID */
            bytesRemaining = 10;
            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionWrite() to receive data from host */
        }
    }else{
        /* ignore vendor type requests, we don't use any */
    }
    return 0;
}

/* ------------------------------------------------------------------------- */

/// Bezeichnug PORTA und PORTB kommt vom C64 ;) ///

void setPORTA(uint8_t value)
{
    PORTC = (PORTC & 0xF0) | (~value & 0x0F);
    PORTD = (PORTD & 0x0F) | (~value & 0xF0);
}

/* ------------------------------------------------------------------------- */

uint8_t getPORTB()
{
    return ~((PINB & 0x3F) | ((PINC & 0x30) << 2));
}

/* ------------------------------------------------------------------------- */

void LEDOn()
{
    PORTD |= 0x01;
}

/* ------------------------------------------------------------------------- */

void LEDOff()
{
    PORTD &= 0xFE;
}

/* ------------------------------------------------------------------------- */

void keyPoll(void)
{
    uint8_t BitMask = 0x01;
    uint8_t i = 0;

    setPORTA(0xFF);
    _delay_us(1);

    if(getPORTB() != 0x00)
    {
        /// Mind. eine Taste der 8x8 Matrix ist gedrückt ///
        LEDOn();

        /// Prüfen auf RESTORE ///
        if((~PIND & 0x02) == 0x02)
        {
            PORTD |= 0x01;
            /// RESTORE ist gedrückt ///
            key_buffer[8] = 0x01;
        }
        else
        {
            /// RESTORE ist nicht gedrückt ///
            key_buffer[8] = 0x00;
        }
    }
    else
    {
        /// Prüfen auf RESTORE ///
        if((~PIND & 0x02) == 0x02)
        {
            LEDOn();
            /// RESTORE ist gedrückt ///
            key_buffer[8] = 0x01;
        }
        else
        {
            LEDOff();
            /// RESTORE ist nicht gedrückt ///
            key_buffer[8] = 0x00;
        }
    }

    for(i=0;i<8;i++)
    {
        setPORTA(BitMask);
        _delay_us(10);
        key_buffer[i] = getPORTB();
        BitMask <<= 1;
    }
}

/* ------------------------------------------------------------------------- */

int main(void)
{
    DDRB = 0x00;    // Datenrichtung Port B setzen
    DDRC = 0x0F;    // Datenrichtung Port C setzen
    DDRD = 0xF1;    // Datenrichtung Port D setzen

    PORTB = 0x3F;   // Interne Pullup Widerstände aktivieren für PIN 0 - 5
    PORTC = 0x30;   // Interne Pullup Widerstände aktivieren für PIN 4 und 5
    PORTD = 0x02;   // Interne Pullup Widerstände aktivieren für PIN 1

    // ReserveByte auf 0x88 setzen
    key_buffer[9] = 0x88;

    uchar   i;

    // Startmeldung mittels Blinken 5x
    for(i=0;i<5;i++)
    {
        LEDOn();
        _delay_ms(40);
        LEDOff();
        _delay_ms(80);
    }

    wdt_enable(WDTO_1S);
    usbInit();
    usbDeviceDisconnect();  // enforce re-enumeration, do this while interrupts are disabled!
    i = 0;
    while(--i){             // fake USB disconnect for > 250 ms
        wdt_reset();
        _delay_ms(1);
    }

    usbDeviceConnect();
    sei();

    for(;;){                /* main event loop */
        wdt_reset();
        usbPoll();
        keyPoll();
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
