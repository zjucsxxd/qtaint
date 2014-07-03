#-------------------------------------------------
#
# Project created by QtCreator 2014-07-05T22:51:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtaint
TEMPLATE = app

OTHER_FILES += LICENSE \
               examples/LICENSE_EXAMPLES \
               examples/dice.png

# OpenCV libs
LIBS += -lopencv_core \
        -lopencv_highgui

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

RESOURCES += \
    examples.qrc
