TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

CONFIG += c++11

HEADERS += \
    autoptr.h \
    scopedptr.h \
    sharedptr.h
