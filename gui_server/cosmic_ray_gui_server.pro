#-------------------------------------------------
#
# Project created by QtCreator 2017-03-06T06:45:45
#
#-------------------------------------------------

QT       += core
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cosmic_ray_gui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        cosmic_shower_project_data_manager.cpp \
    mylistview.cpp \
    dragdropstandarditemmodel.cpp \
    dragdropfilesystemmodel.cpp \
    mycounter.cpp \
    settings.cpp \
    tcpthread.cpp \
    tcpserver.cpp

HEADERS  += cosmic_shower_project_data_manager.h \
    dragdropfilesystemmodel.h \
    dragdropstandarditemmodel.h \
    mylistview.h \
    mycounter.h \
    settings.h \
    tcpthread.h \
    tcpserver.h

FORMS    += cosmic_shower_project_data_manager.ui \
    mycounter.ui \
    settings.ui
QMAKE_CXXFLAGS += -std=c++11
