QT += core gui widgets opengl openglwidgets

CONFIG += c++17

TARGET = CubeRotationApp
TEMPLATE = app

SOURCES += \
    dialogs.cpp \
    main.cpp

HEADERS += \
    dialogs.h

unix|windows: LIBS += -L$$PWD/w/ -lopengl32 -lglu32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Documentation.md \
    README.md \
    textures/mine.png \
    textures/texture.png


INCLUDEPATH += C:/Users/varask/Desktop/Cranfield/Vizualization/libs/glm

RESOURCES += \
    resources.qrc

