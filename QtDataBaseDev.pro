QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databasemanager.cpp \
    main.cpp \
    mainwindow.cpp\
    sidemenuwidget.cpp\
    studentmanage.cpp\
    systemmanage.cpp\
    scoremanage.cpp\
    coursemanage.cpp\
    student.cpp\
    
    


HEADERS += \
    databasemanager.h \
    mainwindow.h\
    sidemenuwidget.h\
    studentmanage.h\
    systemmanage.h\
    scoremanage.h\
    coursemanage.h\
    student.h\
    
    

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    layout.txt \
    qtserver.sql


win32: LIBS += -L$$PWD/lib/ -lQXlsx

INCLUDEPATH += $$PWD/lib/header
DEPENDPATH += $$PWD/lib/header

RESOURCES += \
    src.qrc

