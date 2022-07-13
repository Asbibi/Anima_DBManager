QT       += core gui
QT       += multimedia
QT       += openglwidgets

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

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
    areference.cpp \
    ashortstring.cpp \
    astructure.cpp \
    atablestring.cpp \
    attribute.cpp \
    attributeparam.cpp \
    db_manager.cpp \
    enumerator.cpp \
    main.cpp \
    mainwindow.cpp \
    qapmesh.cpp \
    qapsound.cpp \
    qaptexture.cpp \
    qassetlabel.cpp \
    qassetpreview.cpp \
    qassetpreviewdialog.cpp \
    qassettexture.cpp \
    qattribute.cpp \
    qaugmentedlist.cpp \
    qdatatable.cpp \
    qlistwithsearch.cpp \
    qpanelbase.cpp \
    qpanelenum.cpp \
    qpanelstring.cpp \
    qpanelstruct.cpp \
    qrefdialog.cpp \
    qreflabel.cpp \
    qsstring.cpp \
    qsstringdialog.cpp \
    qsstringtable.cpp \
    qstructuretable.cpp \
    sstringhelper.cpp \
    sstringitem.cpp \
    sstringtable.cpp \
    structure.cpp \
    structuredb.cpp \
    templateattribute.cpp \
    templatestructure.cpp

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
    areference.h \
    ashortstring.h \
    astructure.h \
    atablestring.h \
    attribute.h \
    attributeparam.h \
    db_manager.h \
    enumerator.h \
    mainwindow.h \
    qapmesh.h \
    qapsound.h \
    qaptexture.h \
    qassetlabel.h \
    qassetpreview.h \
    qassetpreviewdialog.h \
    qassettexture.h \
    qattribute.h \
    qaugmentedlist.h \
    qdatatable.h \
    qlistwithsearch.h \
    qpanelbase.h \
    qpanelenum.h \
    qpanelstring.h \
    qpanelstruct.h \
    qrefdialog.h \
    qreflabel.h \
    qsstring.h \
    qsstringdialog.h \
    qsstringtable.h \
    qstructuretable.h \
    sstringhelper.h \
    sstringitem.h \
    sstringtable.h \
    structure.h \
    structuredb.h \
    templateattribute.h \
    templatestructure.h

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
