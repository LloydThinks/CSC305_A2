QT       += core gui widgets

TARGET = cube
TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    cubeGeometry.cpp \
    openGLWidget.cpp \
    scene.cpp


    QT += opengl

    SOURCES +=

    HEADERS +=

    RESOURCES += \
        shaders.qrc \
        textures.qrc


# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    cubeGeometry.h \
    openGLWidget.h \
    SceneObject.h \
    scene.h
