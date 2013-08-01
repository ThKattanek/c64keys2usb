/* Name: widget_8bit_ledleiste.h
 * Projekt: c64keys2usb
 * Author: Thorsten Kattanek
 * Erstellt am: 01.08.2013
 * Copyright: Thorsten Kattanek
 * Vesrion: 0.1
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

#ifndef WIDGET_8BIT_LEDLEISTE_H
#define WIDGET_8BIT_LEDLEISTE_H

#include <QWidget>
#include <QStyle>
#include <stdint.h>


namespace Ui {
class Widget8BitLedLeiste;
}

class Widget8BitLedLeiste : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget8BitLedLeiste(QWidget *parent = 0);
    ~Widget8BitLedLeiste();
    void SetValue(uint8_t value);
    
private:
    Ui::Widget8BitLedLeiste *ui;
};

#endif // WIDGET_8BIT_LEDLEISTE_H
