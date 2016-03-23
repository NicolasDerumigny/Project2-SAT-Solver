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

LIBS += -lfl -ly

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
    $$BISONSOURCES \
    $$BISONHEADERS

FLEXSOURCES = Bison/expr.l
BISONSOURCES = Bison/expr.cpp\
    Bison/expr.ypp


BISONHEADERS = Bison/expr.hpp

flexsource.input = FLEXSOURCES
flexsource.output = ${QMAKE_FILE_BASE}.cpp
flexsource.commands = flex --header-file=${QMAKE_FILE_BASE}.hpp -o ${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN}
flexsource.variable_out = SOURCES
flexsource.name = Flex Sources ${QMAKE_FILE_IN}
flexsource.CONFIG += target_predeps

QMAKE_EXTRA_COMPILERS += flexsource

flexheader.input = FLEXSOURCES
flexheader.output = ${QMAKE_FILE_BASE}.hpp
flexheader.commands = @true
flexheader.variable_out = HEADERS
flexheader.name = Flex Headers ${QMAKE_FILE_IN}
flexheader.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += flexheader

bisonsource.input = BISONSOURCES
bisonsource.output = ${QMAKE_FILE_BASE}.cpp
bisonsource.commands = bison -d --defines=${QMAKE_FILE_BASE}.hpp -o ${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN}
bisonsource.variable_out = SOURCES
bisonsource.name = Bison Sources ${QMAKE_FILE_IN}
bisonsource.CONFIG += target_predeps

QMAKE_EXTRA_COMPILERS += bisonsource

bisonheader.input = BISONSOURCES
bisonheader.output = ${QMAKE_FILE_BASE}.hpp
bisonheader.commands = @true
bisonheader.variable_out = HEADERS
bisonheader.name = Bison Headers ${QMAKE_FILE_IN}
bisonheader.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += bisonheader
