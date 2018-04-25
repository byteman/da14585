TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    param.c \
    state.c \
    adc.c \
    err_handle.c \
    channel.c \
    scaler.c \
    Filter_KMedian.c \
    Weighing.c \
    Timer.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    param.h \
    state.h \
    adc.h \
    err_handle.h \
    channel.h \
    scaler.h \
    platform.h \
    Filter_KMedian.h \
    Weighing.h \
    Timer.h

LIBS+=-lKernel32
