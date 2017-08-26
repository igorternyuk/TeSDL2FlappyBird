TEMPLATE = app
CONFIG += c++1z
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lSDL2  -lSDL2_ttf -lSDL2_image -lSDL2_mixer

DEFINES -= DEBUG

SOURCES += main.cpp \
    bird.cpp \
    tube.cpp \
    model.cpp \
    controller.cpp \
    view.cpp \
    resourcemanager.cpp

HEADERS += \
    bird.h \
    tube.h \
    model.h \
    controller.h \
    view.h \
    resourcemanager.h \
    modellistener.h \
    flappy_bird_constants.h
