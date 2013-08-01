# Name: c64keys2usb.pro
# Projekt: c64keys2usb
# Author: Thorsten Kattanek
# Erstellt am: 30.07.2013
# Copyright: Thorsten Kattanek
# Vesrion: 0.1
# License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = c64keys2usb
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    c64keys2usb.cpp \
    hiddata.c

HEADERS  += mainwindow.h \
    hiddata.h \
    hidsdi.h \
    c64keys2usb.h

FORMS    += mainwindow.ui \


linux-g++-64{
DESTDIR = "bin/linux_x86_64"
LIBS += -lusb
}

linux-g++-32{
DESTDIR = "bin/linux_i686"
LIBS += -lusb
}

win32-g++-cross{
RC_FILE = c64keys2usb.rc
DESTDIR = "bin/win_x32"
LIBS += -lusb
}

OTHER_FILES += \
    grafik/c64keys2usb.ico \
    c64keys2usb.rc \
    firmware/Makefile \
    firmware/main.c \
    firmware/usbconfig.h \
    doc/Anleitung.pdf \
    doc/Anleitung.odt \
    "doc/Struktogramme der Firmware.pdf" \
    "doc/Struktogramme der Firmware.odt" \
    "doc/Firmware C64Keys2USB-02.nsd" \
    "doc/Firmware C64Keys2USB-01.nsd" \
    doc/Schaltplan.pdf
