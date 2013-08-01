/* Name: widget_8bit_ledleiste.cpp
 * Projekt: c64keys2usb
 * Author: Thorsten Kattanek
 * Erstellt am: 01.08.2013
 * Copyright: Thorsten Kattanek
 * Vesrion: 0.1
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

#include "widget_8bit_ledleiste.h"
#include "ui_widget_8bit_led_leiste.h"

#include <QDebug>

Widget8BitLedLeiste::Widget8BitLedLeiste(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget8BitLedLeiste)
{
    ui->setupUi(this);
    ui->led_0->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    ui->led_1->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    ui->led_2->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    ui->led_3->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    ui->led_4->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    ui->led_5->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    ui->led_6->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    ui->led_7->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
}

Widget8BitLedLeiste::~Widget8BitLedLeiste()
{
    delete ui;
}

void Widget8BitLedLeiste::SetValue(uint8_t value)
{
    if(value & 0x01) ui->led_0->setStyleSheet("border-image: url(:/grafik/green-on-64.png);");
    else ui->led_0->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    if(value & 0x02) ui->led_1->setStyleSheet("border-image: url(:/grafik/green-on-64.png);");
    else ui->led_1->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    if(value & 0x04) ui->led_2->setStyleSheet("border-image: url(:/grafik/green-on-64.png);");
    else ui->led_2->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    if(value & 0x08) ui->led_3->setStyleSheet("border-image: url(:/grafik/green-on-64.png);");
    else ui->led_3->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    if(value & 0x10) ui->led_4->setStyleSheet("border-image: url(:/grafik/green-on-64.png);");
    else ui->led_4->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    if(value & 0x20) ui->led_5->setStyleSheet("border-image: url(:/grafik/green-on-64.png);");
    else ui->led_5->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    if(value & 0x40) ui->led_6->setStyleSheet("border-image: url(:/grafik/green-on-64.png);");
    else ui->led_6->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
    if(value & 0x80) ui->led_7->setStyleSheet("border-image: url(:/grafik/green-on-64.png);");
    else ui->led_7->setStyleSheet("border-image: url(:/grafik/green-off-64.png);");
}
