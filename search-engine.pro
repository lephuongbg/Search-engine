TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    document.cpp \
    inode.cpp \
    indexer.cpp

HEADERS += \
    document.h \
    inode.h \
    indexer.h

OTHER_FILES += \
    README
