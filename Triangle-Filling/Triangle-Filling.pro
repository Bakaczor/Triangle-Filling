QT += quick core gui concurrent widgets

CONFIG += c++20

SOURCES += \
        Circle.cpp \
        Coloring.cpp \
        ControlPoint.cpp \
        Edge.cpp \
        ImageProvider.cpp \
        Maths.cpp \
        SceneManager.cpp \
        Triangle.cpp \
        main.cpp

resources.files = main.qml 
resources.prefix = /$${TARGET}
RESOURCES += resources

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Circle.h \
    Coloring.h \
    ControlPoint.h \
    Edge.h \
    ImageProvider.h \
    Maths.h \
    SceneManager.h \
    Triangle.h
