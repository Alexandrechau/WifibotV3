
QT       += core gui
QT       += network
QT       +=webkit
QT += webkitwidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wifibotv3
TEMPLATE = app


SOURCES += main.cpp\
        wifibotv3.cpp

HEADERS  += wifibotv3.h

FORMS    += \
    wifibotv3.ui
