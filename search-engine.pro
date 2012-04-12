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
    stopwords \
    document-new.png \
    folder-new.png \
    window-close.png \
    view-refresh.png \
    view-list-details-symbolic.png

FORMS += \
    mainwindow.ui

RESOURCES += \
    icons.qrc
