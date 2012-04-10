TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    document.cpp \
    inode.cpp \
    indexer.cpp \
    inodedata.cpp

HEADERS += \
    document.h \
    inode.h \
    indexer.h \
    inodedata.h

OTHER_FILES += \
    README
