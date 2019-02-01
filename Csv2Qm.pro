QT += core
QT -= gui

CONFIG += c++11

TARGET = Csv2Qm
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Csv.cpp

HEADERS += \
    Csv.h
