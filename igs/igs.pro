QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../assimp-source/assimp/bin/release/ -lassimp.5.0.1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../assimp-source/assimp/bin/debug/ -lassimp.5.0.1
else:unix: LIBS += -L$$PWD/../../assimp-source/assimp/bin/ -lassimp.5.0.1

INCLUDEPATH += $$PWD/../../assimp-source/assimp/include
DEPENDPATH += $$PWD/../../assimp-source/assimp/include
