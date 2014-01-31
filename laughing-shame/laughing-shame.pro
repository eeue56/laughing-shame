TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    helper_methods.cpp

INCLUDEPATH += `pkg-config --cflags opencv`
LIBS += `pkg-config --libs opencv`

HEADERS += \
    helper_methods.h
