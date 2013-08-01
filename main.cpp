/* Name: main.cpp
 * Projekt: c64keys2usb
 * Author: Thorsten Kattanek
 * Erstellt am: 30.07.2013
 * Copyright: Thorsten Kattanek
 * Vesrion: 0.1
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
