TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    manager.cpp \
    indexer.cpp \
    document.cpp

HEADERS += \
    manager.h \
    indexer.h \
    document.h

OTHER_FILES += \
    README
