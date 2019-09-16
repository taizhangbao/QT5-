#-------------------------------------------------
#
# Project created by QtCreator 2019-07-26T19:12:04
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

requires(qtConfig(listwidget))
qtHaveModule(printsupport): QT += printsupport


TARGET = SHOWMAC_LK_TOOL
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        showmaclk.cpp \
        showmacdialog.cpp


HEADERS += \
        showmaclk.h \
        showmacdialog.h


#include(./des/des.prj)
#HEADERS += $$DESHEADERS
#SOURCES += $$DESSOURCES

FORMS += \
        showmaclk.ui

RESOURCES       = SHOWMAC_LK_TOOL.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    des/des.prj


