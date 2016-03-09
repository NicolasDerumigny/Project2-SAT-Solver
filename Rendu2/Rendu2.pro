#-------------------------------------------------
#
# Project created by QtCreator 2016-02-06T19:09:23
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Rendu2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    Sources/main.cpp \
    Bison/expr.cpp \
    Bison/expr.cpp \
    Bison/expr.l \
    Bison/expr.ypp \
    Sources/var.cpp \
    Sources/formule.cpp \
    Sources/clause.cpp \
    Sources/litt.cpp \
    Sources/deduce.cpp \
    Sources/decide.cpp \
    Sources/backtrack.cpp \
    Sources/assignation.cpp \
    Sources/file_open.cpp \
    Sources/cast_arg.cpp \
    Sources/parser.cpp

HEADERS += \
    Header/var.h \
    Header/litt.h \
    Header/clause.h \
    Header/formule.h \
    Bison/expr.hpp \
    Header/assignation.h \
    Header/global_variables.h