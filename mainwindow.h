/* Name: manwindow.h
 * Projekt: c64keys2usb
 * Author: Thorsten Kattanek
 * Erstellt am: 30.07.2013
 * Copyright: Thorsten Kattanek
 * Vesrion: 0.1
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QMessageBox>
#include "c64keys2usb.h"
#include "widget_8bit_ledleiste.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void TimerFunktion();

private:
    Ui::MainWindow *ui;
    bool C64Keys2USB_Enabled;
    C64Keys2USB *c64keys;
    QTimer *timer1;
};

#endif // MAINWINDOW_H
