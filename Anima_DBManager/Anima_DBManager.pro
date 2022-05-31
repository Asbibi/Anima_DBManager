QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aaaniminstance.cpp \
    aamesh.cpp \
    aaniagara.cpp \
    aarray.cpp \
    aasound.cpp \
    aasset.cpp \
    aatexture.cpp \
    abool.cpp \
    aenumerator.cpp \
    afloat.cpp \
    aint.cpp \
    ashortstring.cpp \
    astructure.cpp \
    atablestring.cpp \
    attribute.cpp \
    attributeparam.cpp \
    db_manager.cpp \
    enumerator.cpp \
    main.cpp \
    mainwindow.cpp \
    qassetlabel.cpp \
    qassettexture.cpp \
    qattribute.cpp \
    qdatatable.cpp \
    qsstring.cpp \
    qsstringdialog.cpp \
    qstructure.cpp \
    qstructuretable.cpp \
    sstringhelper.cpp \
    sstringitem.cpp \
    sstringtable.cpp \
    structure.cpp \
    structuredb.cpp \
    structuretemplate.cpp

HEADERS += \
    aaaniminstance.h \
    aamesh.h \
    aaniagara.h \
    aarray.h \
    aasound.h \
    aasset.h \
    aatexture.h \
    abool.h \
    aenumerator.h \
    afloat.h \
    aint.h \
    ashortstring.h \
    astructure.h \
    atablestring.h \
    attribute.h \
    attributeparam.h \
    db_manager.h \
    enumerator.h \
    mainwindow.h \
    qassetlabel.h \
    qassettexture.h \
    qattribute.h \
    qdatatable.h \
    qsstring.h \
    qsstringdialog.h \
    qstructure.h \
    qstructuretable.h \
    sstringhelper.h \
    sstringitem.h \
    sstringtable.h \
    structure.h \
    structuredb.h \
    structuretemplate.h

TRANSLATIONS += \
    Anima_DBManager_en_GB.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    qstructuretable.ui
