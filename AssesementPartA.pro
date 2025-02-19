QT += core gui widgets opengl openglwidgets

CONFIG += c++17 static release

TARGET = CubeRotationApp
TEMPLATE = app

SOURCES += \
    linerotationdialog.cpp \
    main.cpp

HEADERS += \
    linerotationdialog.h

unix|windows: LIBS += -L$$PWD/w/ -lopengl32 -lglu32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Documentation.md \
    Documentation.md \
    README.md \
    README.md \
    textures/mine.png \
    textures/mine.png \
    textures/minecraft_logo_icon_168974.png \
    textures/texture.png


INCLUDEPATH += C:/Users/varask/Desktop/Cranfield/Vizualization/libs/glm

RESOURCES += \
    resources.qrc \
    resources.qrc

