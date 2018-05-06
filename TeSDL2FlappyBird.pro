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
    resourcemanager.cpp \
    sdl2initializer.cpp

HEADERS += \
    bird.hpp \
    controller.hpp \
    flappy_bird_constants.hpp \
    model.hpp \
    modellistener.hpp \
    resourcemanager.hpp \
    tube.hpp \
    view.hpp \
    sdl2initializer.hpp
