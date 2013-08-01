/* Name: manwindow.cpp
 * Projekt: c64keys2usb
 * Author: Thorsten Kattanek
 * Erstellt am: 30.07.2013
 * Copyright: Thorsten Kattanek
 * Vesrion: 0.1
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    c64keys = new C64Keys2USB();
    C64Keys2USB_Enabled = c64keys->Open();

    if(!C64Keys2USB_Enabled)
    {
        QMessageBox::critical(this,"C64Keys2USB Adapter ...","Es konnte kein C64Keys2USB Adapter gefunden werden.\nBitte überprüfen Sie ob das Gerät eingesteckt ist,\noder ob Sie über genügend Rechte verfügen.");
        exit(0);
    }

    timer1 = new QTimer(this);
    timer1->setInterval(1);

    connect(timer1,SIGNAL(timeout()),this,SLOT(TimerFunktion()));

    timer1->start();
}

MainWindow::~MainWindow()
{
    delete c64keys;
    delete ui;
}

void MainWindow::TimerFunktion()
{
    if(C64Keys2USB_Enabled)
    {
        c64keys->Update();
        ui->label->setText(QVariant(c64keys->GetKeysRow(0)).toString());
        ui->label_2->setText(QVariant(c64keys->GetKeysRow(1)).toString());
        ui->label_3->setText(QVariant(c64keys->GetKeysRow(2)).toString());
        ui->label_4->setText(QVariant(c64keys->GetKeysRow(3)).toString());
        ui->label_5->setText(QVariant(c64keys->GetKeysRow(4)).toString());
        ui->label_6->setText(QVariant(c64keys->GetKeysRow(5)).toString());
        ui->label_7->setText(QVariant(c64keys->GetKeysRow(6)).toString());
        ui->label_8->setText(QVariant(c64keys->GetKeysRow(7)).toString());
        if(c64keys->GetRestoreStatus()) ui->label_9->setText("RESTORE ON");
        else ui->label_9->setText("RESTORE OFF");

        ui->widget->SetValue(c64keys->GetKeysRow(0));
        ui->widget_2->SetValue(c64keys->GetKeysRow(1));
        ui->widget_3->SetValue(c64keys->GetKeysRow(2));
        ui->widget_4->SetValue(c64keys->GetKeysRow(3));
        ui->widget_5->SetValue(c64keys->GetKeysRow(4));
        ui->widget_6->SetValue(c64keys->GetKeysRow(5));
        ui->widget_7->SetValue(c64keys->GetKeysRow(6));
        ui->widget_8->SetValue(c64keys->GetKeysRow(7));
    }
}
