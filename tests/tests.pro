QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

# Ścieżka do plików źródłowych głównego projektu
INCLUDEPATH += ../asystent

SOURCES += \
    tst_main.cpp \
    ../asystent/utils.cpp \
    ../asystent/taxcalculator.cpp \
    ../asystent/cryptoexchange.cpp \
    ../asystent/llmclient.cpp

HEADERS += \
    ../asystent/utils.h \
    ../asystent/taxcalculator.h \
    ../asystent/cryptoexchange.h \
    ../asystent/llmclient.h
