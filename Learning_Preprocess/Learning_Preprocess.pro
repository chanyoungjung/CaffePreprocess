#-------------------------------------------------
#
# Project created by QtCreator 2017-04-03T13:33:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Learning_Preprocess
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
        mainwindow.cpp \
    thread/img_thread.cpp \
    thread/img_save_thread.cpp \
    thread/img_infer_thread.cpp \
    thread/img_thread_forinfer.cpp \
    classification.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    lcm_handler.h \
    thread/img_thread.h \
    thread/img_save_thread.h \
    thread/img_infer_thread.h \
    thread/img_thread_forinfer.h \
    classification.hpp \
    qcustomplot.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include   \
    /home/jay/Downloads/caffe-master/include \
    /usr/lib/x86_64-linux-gnu/hdf5/serial/include \
    /usr/local/cuda/include \
    /usr/lib/x86_64-linux-gnu/include

# lcm lib
LIBS += -L/usr/local/lib \
    -llcm

# opencv lib
LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_video \
    -lopencv_videoio \
    -lopencv_imgcodecs

LIBS += -L/home/jay/Downloads/caffe-master/build/lib \
    -lcaffe


LIBS += -L/usr/lib \
    -lglog \
    -lboost_system
