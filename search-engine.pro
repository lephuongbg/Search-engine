TEMPLATE = app
CONFIG += console
CONFIG -= qt
QMAKE_CXXFLAGS = -std=c++0x

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
    README \
    stopwords
