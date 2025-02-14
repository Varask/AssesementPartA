QT += core gui widgets opengl openglwidgets

CONFIG += c++17

TARGET = CubeRotationApp
TEMPLATE = app

SOURCES += \
    glwidget.cpp \
    linerotationdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    viewpositiondialog.cpp

HEADERS += \
    glwidget.h \
    linerotationdialog.h \
    mainwindow.h \
    viewpositiondialog.h

unix|windows: LIBS += -L$$PWD/w/ -lopengl32 -lglu32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Documentation.md \
    Documentation.md \
    README.md \
    README.md
