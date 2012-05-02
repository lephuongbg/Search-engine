cmake_minimum_required(VERSION 2.4)
PROJECT(search-engine)
FIND_PACKAGE(Qt4 REQUIRED)

SET(CMAKE_CXX_COMPILER g++)
SET(CMAKE_CXX_FLAGS -std=c++0x)
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE release)
ENDIF(NOT CMAKE_BUILD_TYPE)
SET(search-engine_SOURCES main.cpp indexer.cpp inode.cpp inodedata.cpp document.cpp slistview.cpp mainwindow.cpp)
SET(search-engine_HEADERS  slistview.h mainwindow.h)
SET(search-engine_FORMS mainwindow.ui)
SET(search-engine_RESOURCES icons.qrc)

QT4_WRAP_CPP(search-engine_HEADERS_MOC ${search-engine_HEADERS})
QT4_WRAP_UI(search-engine_FORMS_HEADERS ${search-engine_FORMS})
QT4_ADD_RESOURCES(search-engine_RESOURCES_RCC ${search-engine_RESOURCES})

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(search-engine
                    ${search-engine_SOURCES}
                    ${search-engine_HEADERS_MOC}
                    ${search-engine_FORMS_HEADERS}
                    ${search-engine_RESOURCES_RCC})

TARGET_LINK_LIBRARIES(search-engine ${QT_LIBRARIES})

configure_file(${CMAKE_SOURCE_DIR}/stopwords ${CMAKE_BINARY_DIR}/stopwords COPYONLY)
