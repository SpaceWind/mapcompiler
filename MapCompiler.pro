#-------------------------------------------------
#
# Project created by QtCreator 2014-06-30T21:39:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MapCompiler
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newformdialog.cpp \
    mapsyntaxhighlighter.cpp \
    setupscrollparamsdialog.cpp \
    defines.cpp \
    helpers.cpp \
    newspritedialog.cpp \
    callfunctiondialog.cpp \
    chooseform.cpp \
    newanimate.cpp

HEADERS  += mainwindow.h \
    newformdialog.h \
    mapsyntaxhighlighter.h \
    setupscrollparamsdialog.h \
    defines.h \
    helpers.h \
    newspritedialog.h \
    callfunctiondialog.h \
    chooseform.h \
    newanimate.h

FORMS    += mainwindow.ui \
    newformdialog.ui \
    setupscrollparamsdialog.ui \
    newspritedialog.ui \
    callfunctiondialog.ui \
    chooseform.ui \
    newanimate.ui
