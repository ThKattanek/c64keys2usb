/* Name: main.c
 * Projekt: c64keys2usb
 * Author: Thorsten Kattanek
 * Erstellt am: 30.07.2013
 * Copyright: Thorsten Kattanek
 * Vesrion: 0.1
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
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>   /* benötigt von usbdrv.h */
#include "usbdrv.h"

uint8_t key_buffer[16];

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB Interface ----------------------------- */
/* ------------------------------------------------------------------------- */

PROGMEM const char usbHidReportDescriptor[22] = {    /// USB Report descriptor
    0x06, 0x00, 0xff,              //   USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0xa1, 0x01,                    //   COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x10,                    //   REPORT_COUNT (16)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           //   END_COLLECTION
};
/* Da wir nur ein Feature Report definieren, verwenden wir keine Report-ID's
 * Dies würde das erste Byte des Berichts sein, der Gesamte Bereich besteht aus
 * 16 Festdefinierten Daten Bytes

 0x00 - 0x07    = PortB Eingangswerte bei jew. gesetztem PortA Ausgangswert (C64 Key Matrix)
 0x08           = RESTORE Status (0 = nicht gedrückt; 1 = gedrückt)
 0x09 - 0x0F    = nicht benutzt (reserviert f. nächste Versione) */

/* Diese Variablen steuern die aktuelle Datenübertragung  */
static uchar    currentAddress;
static uchar    bytesRemaining;

/* ------------------------------------------------------------------------- */

/* usbFunctionRead() wird aufgerufen wenn der Host Daten Empfangen möchte */
uchar usbFunctionRead(uchar *data, uchar len)
{
    uint8_t i;

    if(len > bytesRemaining)
        len = bytesRemaining;

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
    return 1;
    /// In dieser Version muss der Host keine Daten senden
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID class request */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* since we have only one report type, we can ignore the report-ID */
            bytesRemaining = 16;
            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionRead() to obtain data */
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
            /* since we have only one report type, we can ignore the report-ID */
            bytesRemaining = 16;
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

    uchar   i;
    // key_buffer löschen
    for(i=0;i<16;i++)
    {
        key_buffer[i] = 0x00;
    }

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
    usbDeviceDisconnect();  // neues auflisten erzwingen, solange der Interrupt deaktiviert ist!
    i = 0;
    while(--i){             // Fake USB Disconnect für > 250 ms
        wdt_reset();
        _delay_ms(1);
    }

    usbDeviceConnect();
    sei();

    for(;;){                // Endlosschleife
        wdt_reset();
        usbPoll();
        keyPoll();
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
