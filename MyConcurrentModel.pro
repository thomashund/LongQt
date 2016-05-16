#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T09:57:24
#
#-------------------------------------------------

QT       += core gui concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MyConcurrentModel
TEMPLATE = app

QMAKE_MAC_SDK = macosx10.11

CONFIG += c++11
INCLUDEPATH += ./modellib ./menu_object

linux {
    TARGET = MyConcurrentModel.out

    CXXFLAGS += -g
    DESTDIR = ./build 
    OBJECTS_DIR = ./build/obj
    MOC_DIR = ./build/obj
    RCC_DIR = ./build/obj
    UI_DIR = ./build/obj
}

SOURCES += main.cpp\
        dialog.cpp \
    modellib/grid.cpp \
    modellib/fiber.cpp \
    modellib/cell_kernel.cpp \
    modellib/cell.cpp \
    modellib/kurata08.cpp \
    modellib/gpbatrial.cpp \
    modellib/gridCell.cpp \
    modellib/protocol.cpp \
    modellib/voltageClampProtocol.cpp \
    modellib/currentClampProtocol.cpp \
    modellib/gridProtocol.cpp \
    modellib/measure_kernel.cpp \
    modellib/measure.cpp \
    modellib/iobase.cpp \
    chooseProtoWidget.cpp \
    runWidget.cpp \
    varmenu.cpp \
    heart_cell_sim.cpp \
    myoutput.cpp    \
    qcustomplot.cpp

HEADERS  +=    dialog.h \
    modellib/kurata08.h \
    modellib/tridag.h \
    modellib/gridCell.h \
    modellib/gpbatrial.h \
    modellib/grid.h \
    modellib/fiber.h \
    modellib/node.h \
    modellib/cell_kernel.h \
    modellib/cell.h \
    modellib/protocol.h \
    modellib/voltageClampProtocol.h \
    modellib/currentClampProtocol.h \
    modellib/gridProtocol.h \
    modellib/measure_kernel.h \
    modellib/measure.h \
    modellib/iobase.h \
    chooseProtoWidget.h \
    runWidget.h \
    varmenu.h \
    heart_cell_sim.h \
    myoutput.h  \
    qcustomplot.h

FORMS    += dialog.ui

