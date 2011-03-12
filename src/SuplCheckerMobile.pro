#-------------------------------------------------
#
# Project created by QtCreator 2011-03-12T09:37:00
#
#-------------------------------------------------
QT       += core gui webkit network
TARGET = SuplChecker
TEMPLATE = app
VERSION = 0.60.2
SOURCES+= main.cpp\
          mainwindow.cpp \
          parser.cpp \
          nastaveni.cpp
HEADERS+= mainwindow.h \
          parser.h \
          nastaveni.h
FORMS  += mainwindow.ui \
          nastaveni.ui
RESOURCES += icons.qrc

ICON = icons/SuplCheckerMobile.svg
TARGET.CAPABILITY += NetworkServices
TARGET.UID3 = 0xE334B602
TARGET.EPOCHEAPSIZE = 0x20000 0x2000000
