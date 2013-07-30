#-------------------------------------------------
#
# Project created by QtCreator 2013-07-30T08:13:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = c64keys2usb
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    hiddata.c \
    c64keys2usb.cpp

HEADERS  += mainwindow.h \
    firmware/usbdrv/usbportability.h \
    firmware/usbdrv/usbdrv.h \
    firmware/usbdrv/usbconfig-prototype.h \
    firmware/usbdrv/oddebug.h \
FORMS    += mainwindow.ui \
    hiddata.h \
    hidsdi.h \
    c64keys2usb.h

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
    doc/Struktogramme der Firmware.pdf \
    doc/Struktogramme der Firmware.odt \
    doc/Firmware C64Keys2USB-02.nsd \
    doc/Firmware C64Keys2USB-01.nsd
