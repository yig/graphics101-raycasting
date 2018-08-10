QT += core
## We need QImage
# QT -= gui

TARGET = raycasting

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

SOURCES += \
    camera.cpp \
    main.cpp \
    mesh.cpp \
    scene.cpp \
    shape.cpp \
    parser.cpp

HEADERS += \
    camera.h \
    light.h \
    material.h \
    scene.h \
    shape.h \
    types.h \
    debugging.h
