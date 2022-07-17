QT += core gui openglwidgets
include(../QtGifImage-master/src/gifimage/qtgifimage.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    backend.c \
    graphicengine.cpp \
    main.cpp \
    mainwindow.cpp \
    oglw.cpp

HEADERS += \
    backend.h \
    graphicengine.h \
    mainwindow.h \
    oglw.h

FORMS += \
    mainwindow.ui

#LIBS += -lOpenGL
#LIBS += -lopengl32 #widows

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    shaders.qrc
