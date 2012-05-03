TEMPLATE = app
CONFIG += console
CONFIG -= qt
QMAKE_CXXFLAGS = -std=c++0x
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

QMAKE_LFLAGS_RELEASE -= -O1
QMAKE_LFLAGS_RELEASE += -O3

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
