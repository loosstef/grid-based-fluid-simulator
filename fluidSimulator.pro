#-------------------------------------------------
#
# Project created by QtCreator 2019-03-07T16:34:10
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
LIBS     += -lOpenCL

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fluidSimulator
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

QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += -Ofast
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -Ofast


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        grid.cpp \
    field.cpp \
    simulationfield.cpp \
    simulationviewer.cpp \
    painttool.cpp \
    looper.cpp \
    renderengine.cpp \
    velocitymanipulator.cpp \
    simulationfieldcontroller.cpp \
    renderenginecontroller.cpp \
    painttoolcontroller.cpp \
    gpucontroller.cpp \
    io.cpp \
    painthandler.cpp

HEADERS += \
        mainwindow.h \
        grid.h \
        field.h \
        looper.h \
    simulationfield.h \
    simulationviewer.h \
    painttool.h \
    renderengine.h \
    velocitymanipulator.h \
    vector2D.h \
    simulationfieldcontroller.h \
    renderenginecontroller.h \
    painttoolcontroller.h \
    gpucontroller.h \
    io.h \
    painthandler.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    simulation_steps.cl

RESOURCES += \
    resources.qrc
