QT += core gui
TEMPLATE = app
QMAKE_CXXFLAGS = -std=c++0x
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

QMAKE_LFLAGS_RELEASE -= -O1
QMAKE_LFLAGS_RELEASE += -O3

SOURCES += main.cpp \
    document.cpp \
    inode.cpp \
    indexer.cpp \
    inodedata.cpp \
    mainwindow.cpp \
    slistview.cpp

HEADERS += \
    document.h \
    inode.h \
    indexer.h \
    inodedata.h \
    mainwindow.h \
    slistview.h

OTHER_FILES += \
    README \
    stopwords \
    document-new.png \
    folder-new.png \
    window-close.png \
    view-refresh.png \
    view-list-details-symbolic.png \
    CMakeLists.txt \
    search-text.png

FORMS += \
    mainwindow.ui

RESOURCES += \
    icons.qrc
