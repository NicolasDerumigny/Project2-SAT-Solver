#-------------------------------------------------
#
# Project created by QtCreator 2016-02-06T19:09:23
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Rendu1
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    main.cpp \
    expr.cpp \
    expr.cpp \
    expr.l \
    expr.ypp \
    var.cpp \
    formule.cpp \
    clause.cpp \
    litt.cpp

HEADERS += \
    var.h \
    litt.h \
    clause.h \
    formule.h \
    expr.tab.hpp \
    expr.hpp
