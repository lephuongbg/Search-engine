QT += core gui
TEMPLATE = app
CONFIG += console

SOURCES += main.cpp \
    document.cpp \
    inode.cpp \
    indexer.cpp \
    inodedata.cpp \
    mainwindow.cpp

HEADERS += \
    document.h \
    inode.h \
    indexer.h \
    inodedata.h \
    mainwindow.h

OTHER_FILES += \
    README \
    stopwords

FORMS += \
    mainwindow.ui
