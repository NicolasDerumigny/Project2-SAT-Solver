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

LIBS += -lfl

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += Header/ \
        Bison/

SOURCES += \
    Sources/main.cpp \
    Sources/var.cpp \
    Sources/formule.cpp \
    Sources/clause.cpp \
    Sources/litt.cpp \
    Sources/deduce.cpp \
    Sources/decide.cpp \
    Sources/backtrack.cpp \
    Sources/assignation.cpp \
    Sources/file_open.cpp \
    Sources/cast_arg.cpp

HEADERS += \
    Header/var.h \
    Header/litt.h \
    Header/clause.h \
    Header/formule.h \
    Header/assignation.h \
    Header/global_variables.h \
    Header/deduce.h \
    Header/decide.h \
    Header/backtrack.h \
    Header/file_open.h \
    Header/global_variable_extern.h \
    Header/cast_arg.h

OTHER_FILES +=  \
    $$FLEXSOURCES \
    $$BISONSOURCES

FLEXSOURCES = Bison/expr.l
BISONSOURCES = Bison/expr.cpp\
    Bison/expr.ypp


flexsource.input = FLEXSOURCES
flexsource.output = ${QMAKE_FILE_BASE}.cpp
flexsource.commands = flex -w -o ../Rendu2/Bison/${QMAKE_FILE_BASE}.yy.cpp -d ${QMAKE_FILE_IN}
flexsource.variable_out = SOURCES
flexsource.name = Flex Sources ${QMAKE_FILE_IN}
flexsource.CONFIG += target_predeps

QMAKE_EXTRA_COMPILERS += flexsource

bisonsource.input = BISONSOURCES
bisonsource.output = ${QMAKE_FILE_BASE}.cpp
bisonsource.commands = bison --report=all -o ../Rendu2/Bison/${QMAKE_FILE_BASE}.tab.cpp ${QMAKE_FILE_IN}
bisonsource.variable_out = SOURCES
bisonsource.name = Bison Sources ${QMAKE_FILE_IN}
bisonsource.CONFIG += target_predeps

QMAKE_EXTRA_COMPILERS += bisonsource
