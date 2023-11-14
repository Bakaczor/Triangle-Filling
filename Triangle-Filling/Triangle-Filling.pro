QT += quick core gui concurrent widgets

SOURCES += \
        Coloring.cpp \
        ControlPoint.cpp \
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
    Coloring.h \
    ControlPoint.h \
    ImageProvider.h \
    Maths.h \
    SceneManager.h \
    Triangle.h
