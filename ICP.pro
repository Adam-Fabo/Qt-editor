QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    blockinscene.cpp \
    builderaddpartdial.cpp \
    builderhelpwindow.cpp \
    appmodel.cpp \
    builderwindow.cpp \
    compiler.cpp \
    editoropendialog.cpp \
    editorwindow.cpp \
    insertconsdialog.cpp \
    inserttextwindow.cpp \
    main.cpp \
    mainview.cpp \
    mainwindow.cpp \
    savecompositedial.cpp \
    scene.cpp \
    texteditnewline.cpp
    mainwindow.cpp \

HEADERS += \
    blockinscene.h \
    builderaddpartdial.h \
    builderhelpwindow.h \
    appmodel.h \
    builderwindow.h \
    compiler.h \
    editoropendialog.h \
    editorwindow.h \
    insertconsdialog.h \
    inserttextwindow.h \
    mainview.h \
    mainwindow.h \
    savecompositedial.h \
    scene.h \
    texteditnewline.h
    mainwindow.h \


FORMS += \
    builderaddpartdial.ui \
    builderhelpwindow.ui \
    builderwindow.ui \
    editoropendialog.ui \
    editorwindow.ui \
    insertconsdialog.ui \
    inserttextwindow.ui \
    mainwindow.ui \
    savecompositedial.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc \
    Resources.qrc

DISTFILES += \
    ICP.pro.user
